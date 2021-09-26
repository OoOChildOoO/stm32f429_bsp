/*
 * @Author: your name
 * @Date: 2021-09-14 17:35:03
 * @LastEditTime: 2021-09-25 17:19:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\file_oper\fil_edit.c
 */
/*******************************************************************************
Copyright	:	JACK SEWING MACHINE CO.,LTD
File name	:	file_edit.c
Description	:	�ļ��༭c�ļ�
History		:	Date		Version	Author	note
				2019/08/21	v1.0.0	chenff	��ʼ�汾
*******************************************************************************/
#define FILE_EDIT_C__

#include "fil_edit.h"

Array *av;


//static void  ClearPicCell(Array *icell);
static int   CheckPicCell(Array *icell);
void FileMakeTest(FIL fil);
int makeBinaryFile(char *filepath);
int opendxf(char *filepath);

uint8_t Format(TCHAR *memory)
{
	uint8_t res;
	
	printf("NAND Disk Formatting... \r\n");//��ʽ��NAND
	
	if(strcmp(memory, "0:") == 0)
	{
		res=f_mkfs("0:",FM_ANY,0,fatbuf,FF_MAX_SS); //��ʽ��FLASH,2,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"0:SDCARD"); //����Flash���̵�����Ϊ��NANDDISK
			printf("SDCARD Format Finish \r\n");  //��ʽ�����
		}
		else
		{
			printf("SDCARD Format Error \r\n"); //��ʽ��ʧ��
		}
	}
	else if(strcmp(memory, "2:") == 0)
	{
		res=f_mkfs("2:",FM_ANY,0,fatbuf,FF_MAX_SS); //��ʽ��FLASH,2,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"2:NANDDISK"); //����Flash���̵�����Ϊ��NANDDISK
			printf("NAND Disk Format Finish \r\n");  //��ʽ�����
		}
		else
		{
			printf("NAND Disk Format Error \r\n"); //��ʽ��ʧ��
		}
	}
	else if(strcmp(memory, "3:") == 0)
	{
		res=f_mkfs("3:",FM_ANY,0,fatbuf,FF_MAX_SS); //��ʽ��FLASH,2,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"3:USB"); //����Flash���̵�����Ϊ��NANDDISK
			printf("USB Format Finish \r\n");  //��ʽ�����
		}
		else
		{
			printf("USB Format Error \r\n"); //��ʽ��ʧ��
		}
	}
	
	
	
	return res;
}
/*******************************************************************************
*  ��	 ��:
*  ����ԭ��:
*  ��ڲ���:
*  ���ڲ���:
*  ������Ϣ:
*******************************************************************************/
Array *loadPicCellFromFile(uint8_t *buf, uint32_t length)
{
	uint32_t i;
	uint32_t j;
	uint32_t size;
	uint32_t point;
	Array *arr;
	PIC_CELL_t *cell;
	PIC_CELL_t *pic_cell;
	Array *ndl;
	Array *needle;
	uint8_t *vp;
	
	if ((NULL == buf) || (0 == length)){
		return NULL;
	}

	arr = array_creat(picDeleteCell);											//����Array�ṹ��
	if (NULL == arr){
		return NULL;
	}

	point = sizeof(Array);
	for (i=0; i<((Array*)buf)->size; i++){
		if (point >= length){
			return NULL;
		}
		cell = (PIC_CELL_t*)(buf+point);
		if (NULL == cell){
			return NULL;
		}
		pic_cell = picCreatCell(0, CEL_TP_D);									//��ͼԪ ����Ḳ��
		if (NULL == pic_cell){
			return NULL;
		}
		size = sizeof(PIC_CELL_t);
		memcpy(pic_cell, cell, size);											//����ͼԪ�ṹ��
		point += size;
		if (0 != cell->num){
			size = sizeof(I_POINT)*cell->num;
			pic_cell->ip = MY_MALLOC(size);
			if (NULL == pic_cell->ip){
				return NULL;
			}
			memcpy(pic_cell->ip, buf+point, size);								//���ƹ��ɵ�
			point += size;
		}
		if (NULL != cell->ndl){
			needle = array_creat(NULL);											//�����뼣��array�ṹ��
			if (NULL == needle){
				return NULL;
			}
			ndl = (Array*)(buf+point);
			if (NULL == ndl){
				return NULL;
			}
			point += sizeof(Array);
			if (0 != ndl->size){
				size = sizeof(File_Needle_Data)*ndl->size;
				vp = MY_MALLOC(size);
				if (NULL == vp){
					return NULL;
				}
				memcpy(vp, buf+point, size);									//�����뼣������
				point += size;
				for (j=0; j<ndl->size; j++){
					array_add(needle, (__int32*)(vp+sizeof(File_Needle_Data)*j));//����뼣������
				}
			}
			pic_cell->ndl = needle;
		}
		array_add(arr, (__int32*)pic_cell);										//���ͼԪ
	}

	return arr;
}
/*******************************************************************************
*  ��	 ��:
*  ����ԭ��:
*  ��ڲ���:
*  ���ڲ���:
*  ������Ϣ:
*******************************************************************************/
RES_t savePatternFile(char *fil_path, Array *arr)
{
	uint32_t i;
	uint32_t j;
	uint32_t bw;
	uint32_t size;
	FIL fil;
	FRESULT err;
	PIC_CELL_t *cell;
	File_Needle_Data *ndl;
	
	if ((NULL == fil_path) || (NULL == arr)){
		return RES_ERR;
	}
	//�½��ļ�
	err = f_open (&fil, (const TCHAR*)fil_path, FA_WRITE|FA_CREATE_NEW);
	if (FR_OK != err){
		err = f_open (&fil, (const TCHAR*)fil_path, FA_OPEN_EXISTING);
		if (FR_OK != err){														//�����ڴ�ʧ��
			showWarning(0);
			return RES_ERR;
		}else{																	//�ļ�����
			if (RES_DONE == showWarning(1)){
				err = f_close(&fil);
				err = f_unlink ((const TCHAR*)fil_path);
				err = f_open (&fil, (const TCHAR*)fil_path, FA_WRITE|FA_CREATE_NEW);
				if (FR_OK != err){
					return RES_ERR;
				}
			}else{
				f_close(&fil);
				return RES_DONE;
			}
		}
	}
	//��������
	size = sizeof(Array);
	if (FR_OK != f_write(&fil, arr, size, &bw)){									//д��Array�ṹ��
		f_close(&fil);
		return RES_ERR;
	}
	for (i = 0; i<arr->size; i++)
	{
		cell = (PIC_CELL_t*)array_at(arr, i);
		if (NULL == cell){
			f_close(&fil);
			return RES_ERR;
		}
		size = sizeof(PIC_CELL_t);
		if (FR_OK != f_write(&fil, cell, size, &bw)){							//д��ͼԪ�ṹ��
			f_close(&fil);
			return RES_ERR;
		}
		if (cell->num){
			size = sizeof(I_POINT)*cell->num;
			if (FR_OK != f_write(&fil, cell->ip, size, &bw)){					//д�빹�ɵ�
				f_close(&fil);
				return RES_ERR;
			}
		}
		if (NULL != cell->ndl){
			size = sizeof(Array);
			if (FR_OK != f_write(&fil, cell->ndl, size, &bw)){					//д���뼣��array�ṹ��
				f_close(&fil);
				return RES_ERR;
			}
			size = sizeof(File_Needle_Data);
			for (j=0; j<cell->ndl->size; j++){
				ndl = (File_Needle_Data*)array_at(cell->ndl, j);
				if (FR_OK != f_write(&fil, ndl, size, &bw)){					//д���뼣������
					f_close(&fil);
					return RES_ERR;
				}
			}
			//if (cell->ndl->size){
			//	size = sizeof(File_Needle_Data)*cell->ndl->size;
			//	ndl = (File_Needle_Data*)array_at(cell->ndl, 0);
			//	if (FR_OK != f_write(&fil, ndl, size, &bw)){						//д���뼣������
			//		f_close(&fil);
			//		return RES_ERR;
			//	}
			//}
		}
	}
	
	f_close(&fil);
	return RES_DONE;
}
/*******************************************************************************
*  ��	 ��:
*  ����ԭ��:
*  ��ڲ���:
*  ���ڲ���:
*  ������Ϣ:
*******************************************************************************/
Array *openPatternFile(char *fil_path)
{
	uint32_t br;
	uint32_t size;
	FIL fil;
	FRESULT err;
	uint8_t *buf;
	
	Array *arr;
	
	if (NULL == fil_path){
		return NULL;
	}
	
	err = f_open(&fil, (const TCHAR*)fil_path, FA_READ);
	if(FR_OK != err){
		return NULL;
	}

	size = f_size(&fil);
	buf = (uint8_t*)MY_MALLOC(size);
	if (NULL == buf){
		return NULL;
	}

	err = f_read(&fil, buf, size, &br);
	f_close(&fil);
	if(FR_OK != err){
		MY_FREE(buf);
		return NULL;
	}
	
	arr = loadPicCellFromFile(buf, size);
	
	MY_FREE(buf);
	
	return arr;
}

static int CheckPicCell(Array *icell)
{
	int i;
	if(icell == NULL)
	{
		return 1;
	}
	if(icell->size == 0)
	{
		return 2;
	}
	for(i=0; i<icell->size; i++)
	{
		if(*(icell->array + i) == NULL)
		{
			return 3;
		}
	}
	return 0;
}

int makeBinaryFile(char *filepath)
{
	FIL fil;        /* File object */
	FRESULT fr;     /* FatFs return code */

	fr = f_open(&fil, filepath, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(fr)
	{
		f_close(&fil);
		printf("open failed! fr = %d\r\n", fr);
		return (int)fr;
	}

	if (CheckPicCell(av) == 0)
	{
//		savePatternFile(&fil, av);
	}

	f_close(&fil);

	return fr;
}

int opendxf(char *filepath)
{	
	FIL fil;        /* File object */
	FRESULT fr;     /* FatFs return code */

	FIL fil1;        /* File object */
	FRESULT fr1;     /* FatFs return code */
	printf("filepath:%s\r\n", filepath);
    fr = f_open(&fil, filepath, FA_READ);
	if(fr)
	{
		f_close(&fil);
		printf("open failed! fr = %d\r\n", fr);
		return (int)fr;
	}
	//printf("fr:%d\r\n", fr);
	
	fr1 = f_open(&fil1, filepath, FA_READ);
	if(fr1)
	{
		f_close(&fil1);
		printf("open failed! fr = %d\r\n", fr);
		return (int)fr;
	}
	//printf("fr1:%d\r\n", fr1);

	arrayClr(av);
	av=array_creat(FREE_FUN);
	CAD_dxf_Analize(av, &fil, &fil1);
	f_close(&fil);
	
	return fr+fr1;
}


