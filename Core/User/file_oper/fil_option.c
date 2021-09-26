/*******************************************************************************
Copyright	:	JACK SEWING MACHINE CO.,LTD
File name	:	fil_option.c
Description	:	�ļ�����c�ļ�
History		:	Date		Version	Author	note
				2019/08/07	v1.0.0	xiongll	��ʼ�汾
*******************************************************************************/
#define __FIL_OPTION_C__
/******************************************************************************/
#include "fil_option.h"
/******************************************************************************/
FATFS *fs[FF_VOLUMES]; //�߼����̹�����.
FIL *file;			   //�ļ�1
FIL *ftemp;			   //�ļ�2
UINT br, bw;		   //��д����
FILINFO fileinfo;	   //�ļ���Ϣ
DIR dir;			   //Ŀ¼

uint8_t *fatbuf; //SD�����ݻ�����
/******************************************************************************/

/**
 * @description: ��ʼ���ļ�ϵͳ���ָ��
 * @param {*}
 * @return {*}
 */
uint8_t fs_FilFuns_Init(void)
{
	uint8_t i;
	for (i = 0; i < FF_VOLUMES; i++)
	{
		fs[i] = (FATFS *)malloc(sizeof(FATFS)); //Ϊ����i�����������ڴ�
		if (!fs[i])
			break;
	}
	file = (FIL *)malloc(sizeof(FIL));	//Ϊfile�����ڴ�
	ftemp = (FIL *)malloc(sizeof(FIL)); //Ϊftemp�����ڴ�
	fatbuf = (uint8_t *)malloc(512);	//Ϊfatbuf�����ڴ�

	//������һ��ʧ��,��ʧ��.
	if (i == FF_VOLUMES && file && ftemp && fatbuf)
		return 0;
	else
		return 1;
}

/**
 * @description: �ļ�����
 * @param {uint8_t} *pSrcFile Դ�ļ�·��(���ļ���)
 * @param {uint8_t} *pDstFile Ŀ���ļ�·��(���ļ���)
 * @param {uint8_t} fwmode 0:������,�½� 1:����
 * @param {uint8_t} cpmode 0:����Դ�ļ� 1:ɾ��Դ�ļ�
 * @return {*}
 */
uint8_t fs_FileCopy(uint8_t *pSrcFile, uint8_t *pDstFile, uint8_t fwmode, uint8_t cpmode)
{
	FRESULT fr;	 // FatFs�����������ش���
	UINT br, bw; // �ļ���д����
	FIL *fsrc = 0;
	FIL *fdst = 0;
	uint8_t *buffer = 0;
	fsrc = (FIL *)malloc(sizeof(FIL)); //�ļ����������ڴ�
	fdst = (FIL *)malloc(sizeof(FIL));
	buffer = (uint8_t *)malloc(1024); // �����ļ�������

	if (fwmode == 0)
		fwmode = FA_CREATE_NEW; //������
	else
		fwmode = FA_CREATE_ALWAYS; //���Ǵ��ڵ��ļ�

	// ���������ϴ�Դ�ļ�
	fr = f_open(fsrc, (const TCHAR *)pSrcFile, FA_READ);
	if (fr)
		return (uint8_t)fr;
	// ���������ϴ���Ŀ���ļ�
	fr = f_open(fdst, (const TCHAR *)pDstFile, FA_WRITE | fwmode);
	if (fr)
		return (uint8_t)fr;
	// ��Դ�ļ����Ƶ�Ŀ���ļ���
	for (;;)
	{
		// ��Դ�ļ���
		fr = f_read(fsrc, buffer, sizeof buffer, &br);
		// �������eof
		if (fr || br == 0)
			break;
		// д��Ŀ���ļ�
		fr = f_write(fdst, buffer, br, &bw);
		// ������ߴ�������
		if (fr || bw < br)
			break;
	}
	// �ر��ļ�
	f_close(fsrc);
	f_close(fdst);
	// ���ƺ��Ƿ���Դ�ļ�
	if (cpmode)
		f_unlink((const TCHAR *)pSrcFile);
	// �ͷ��ڴ�
	free(fsrc);
	free(fdst);
	free(buffer);
	return (uint8_t)fr;
}

/**
 * @description: ɨ��ָ��·��(�����ļ���)
 * @param {uint8_t} *path ·��
 * @return {*}
 */
uint8_t fs_ScanDir(uint8_t *path)
{
	FRESULT res;
	res = f_opendir(&dir, (const TCHAR *)path); //��һ��Ŀ¼
	if (res == FR_OK)
	{
		printf("\r\n");
		while (1)
		{
			res = f_readdir(&dir, &fileinfo);			//��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || fileinfo.fname[0] == 0) //������/��ĩβ��,�˳�
				break;
			//if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
			printf("%s/", path);			  //��ӡ·��
			printf("%s\r\n", fileinfo.fname); //��ӡ�ļ���
		}
	}
	return res;
}

/**
 * @description: ����ȡ�ļ����ϼ�·����(�����ļ���)
 * @param {uint8_t} *path
 * @return {*}
 */
uint8_t *fs_GetPathDir(uint8_t *path)
{
	uint16_t temp = 0;
	while (*path != 0)
	{
		path++;
		temp++;
	}
	if (temp < 4)
		return 0;
	temp++;
	while ((*path != 0x5c) && (*path != 0x2f)) //׷����������һ��"\"����"/"��
	{
		path--;
	}
	return ++path;
}

/**
 * @description: ��ȡ�ļ���С
 * @param {uint8_t} *path
 * @return {*}
 */
uint32_t fs_GetFileSize(uint8_t *path)
{
	FRESULT fr; // FatFs�����������ش���
	uint32_t file_size = 0;
	FIL *fsrc = 0;

	fsrc = (FIL *)malloc(sizeof(FIL)); //�ļ����������ڴ�

	fr = f_open(fsrc, (const TCHAR *)path, FA_READ);
	//printf("open file %s \r\n", path);
	if (fr == FR_OK)
	{
		file_size = f_size(fsrc);
		//printf("file %s size: %d Byte", path, file_size);
	}
	f_close(fsrc);

	free(fsrc);
	return file_size;
}

/**
 * @description: ��ȡĿ¼�������ļ���С
 * @param {uint8_t} *path
 * @return {*} 100���ڴ�����ʧ�� 101���ڴ�����ʧ��
 */
uint32_t fs_GetDirSize(uint8_t *path)
{
	uint16_t MAX_PATHNAME_DEPTH = 512 + 1; //���Ŀ���ļ�·��+�ļ������
	uint8_t res = 0;
	DIR *fddir = 0;		   //Ŀ¼
	FILINFO *finfo = 0;	   //�ļ���Ϣ
	uint8_t *pathname = 0; //Ŀ���ļ���·��+�ļ���
	uint16_t pathlen = 0;  //Ŀ��·������
	uint32_t fdsize = 0;

	fddir = (DIR *)malloc(sizeof(DIR)); //�����ڴ�
	finfo = (FILINFO *)malloc(sizeof(FILINFO));
	if (fddir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		pathname = malloc(MAX_PATHNAME_DEPTH);
		if (pathname == NULL)
			res = 101;
		if (res == 0)
		{
			pathname[0] = 0;
			strcat((char *)pathname, (const char *)path); //����·��
			res = f_opendir(fddir, (const TCHAR *)path);  //��ԴĿ¼
			if (res == 0)								  //��Ŀ¼�ɹ�
			{
				while (res == 0)
				{
					res = f_readdir(fddir, finfo); //��ȡĿ¼�µ�һ���ļ�
					if (res != FR_OK || finfo->fname[0] == 0)
						break; //������/��ĩβ��,�˳�
					if (finfo->fname[0] == '.')
						continue;			   //�����ϼ�Ŀ¼
					if (finfo->fattrib & 0X10) //����Ŀ¼(�ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;)
					{
						pathlen = strlen((const char *)pathname);			  //�õ���ǰ·���ĳ���
						strcat((char *)pathname, (const char *)"/");		  //��б��
						strcat((char *)pathname, (const char *)finfo->fname); //Դ·��������Ŀ¼����
						fdsize += fs_GetDirSize(pathname);						  //�õ���Ŀ¼��С,�ݹ����
						pathname[pathlen] = 0;								  //���������
					}
					else
						fdsize += finfo->fsize; //��Ŀ¼,ֱ�Ӽ����ļ��Ĵ�С
				}
			}
			//#ifdef DEBUG
			printf("file %s size: %d Byte \r\n", path, fdsize);
			//#endif
			//f_closedir(fddir);
			free(pathname);
		}
	}
	free(fddir);
	free(finfo);
	if (res)
		return 0;
	else
		return fdsize;
}

/**
 * @description: ɾ��ָ���ļ���
 * @param {uint8_t} *path
 * @return {*}
 */
uint8_t fs_DelDir(uint8_t *path)
{
	uint16_t i, j;
	uint8_t res;
	DIR *dir = 0;
	FILINFO *finfo = 0; //�ļ���Ϣ

	dir = (DIR *)malloc(sizeof(DIR)); //�����ڴ�
	finfo = (FILINFO *)malloc(sizeof(FILINFO));
	if (dir == NULL || finfo == NULL)
		res = 100;
	res = f_opendir(dir, (const TCHAR *)path);
	if (res == FR_OK)
	{
		for (i = 0; path[i]; i++)
			;
		path[i++] = '/';
		for (;;)
		{
			res = f_readdir(dir, finfo);
			if (res != FR_OK || !finfo->fname[0])
				break;
			if (finfo->fname[0] == '.')
				continue;
			j = 0;
			do
				path[i + j] = finfo->fname[j];
			while (finfo->fname[j++]);
			if (finfo->fattrib & AM_DIR)
			{
				res = fs_DelDir(path);
				if (res != FR_OK)
					break;
			}
			res = f_unlink((const TCHAR *)path);
			if (res != FR_OK)
				break;
		}
		path[--i] = '\0';
		f_closedir(dir);
	}

	return res;
}

/**
 * @description: ����ָ��·�������ļ�
 * @param {uint8_t} *pSrcFile Դ�ļ�·��(�����ļ���)
 * @param {uint8_t} *pDstFile Ŀ���ļ�·��(�����ļ���)
 * @param {uint8_t} fwmode 0:������,�½� 1:����
 * @param {uint8_t} cpmode 0:����Դ�ļ� 1:ɾ��Դ�ļ�
 * @return {*}
 */
uint8_t fs_DirCopy(uint8_t *pSrcDir, uint8_t *pDstDir, uint8_t fwmode, uint8_t cpmode)
{
	uint16_t MAX_PATHNAME_DEPTH = 512 + 1; //���Ŀ���ļ�·��+�ļ������
	uint8_t res = 0;
	DIR *srcdir = 0;	//ԴĿ¼
	DIR *dstdir = 0;	//ԴĿ¼
	FILINFO *finfo = 0; //�ļ���Ϣ
	uint8_t *fn = 0;	//���ļ���

	uint8_t *dstpathname = 0; //Ŀ���ļ���·��+�ļ���
	uint8_t *srcpathname = 0; //Դ�ļ���·��+�ļ���

	uint16_t dstpathlen = 0; //Ŀ��·������
	uint16_t srcpathlen = 0; //Դ·������

	srcdir = (DIR *)malloc(sizeof(DIR)); //�����ڴ�
	dstdir = (DIR *)malloc(sizeof(DIR));
	finfo = (FILINFO *)malloc(sizeof(FILINFO));

	if (srcdir == NULL || dstdir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		// ��������ļ���ȿռ�
		dstpathname = malloc(MAX_PATHNAME_DEPTH);
		srcpathname = malloc(MAX_PATHNAME_DEPTH);
		fn = malloc(MAX_PATHNAME_DEPTH);
		if (dstpathname == NULL || srcpathname == NULL)
			res = 101;
		dstpathname[0] = 0;
		srcpathname[0] = 0;
		strcat((char *)srcpathname, (const char *)pSrcDir); //����ԭʼԴ�ļ�·��
		strcat((char *)dstpathname, (const char *)pDstDir); //����ԭʼĿ���ļ�·��
		res = f_opendir(srcdir, (const TCHAR *)pSrcDir);	//��ԴĿ¼
		if (res == 0)										//��Ŀ¼�ɹ�
		{
			fn = fs_GetPathDir(pSrcDir);						// �ж��Ƿ��Ǹ�Ŀ¼
			strcat((char *)dstpathname, (const char *)"/"); //����б��
			if (fn == 0)									//��꿽��
			{
				dstpathlen = strlen((const char *)dstpathname);
				dstpathname[dstpathlen] = pSrcDir[0]; //��¼���
				dstpathname[dstpathlen + 1] = 0;	  //������
				printf("dir:%s\r\n", dstpathname);
			}
			else
			{
				strcat((char *)dstpathname, (const char *)fn); //���ļ���
				printf("dir:%s\r\n", dstpathname);
			}
		}
		res = f_mkdir((const TCHAR *)dstpathname); //����ļ����Ѿ�����,�Ͳ�����.��������ھʹ����µ��ļ���.
		if (res == FR_EXIST)
			res = 0;
		while (res == 0) //��ʼ�����ļ������������
		{
			res = f_readdir(srcdir, finfo); //��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || finfo->fname[0] == 0)
				break; //������/��ĩβ��,�˳�
			if (finfo->fname[0] == '.')
				continue; //�����ϼ�Ŀ¼

			fn = (uint8_t *)finfo->fname;					//�õ��ļ���
			dstpathlen = strlen((const char *)dstpathname); //�õ���ǰĿ��·���ĳ���
			srcpathlen = strlen((const char *)srcpathname); //�õ�Դ·������

			strcat((char *)srcpathname, (const char *)"/"); //Դ·����б��
			if (finfo->fattrib & 0X10)						//����Ŀ¼�ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;
			{
				strcat((char *)srcpathname, (const char *)fn);			 //Դ·��������Ŀ¼����
																		 //				printf("\r\ncopy folder %s to %s\r\n",srcpathname,dstpathname);//�����ļ�
				res = fs_DirCopy(srcpathname, dstpathname, fwmode, cpmode); //�����ļ���
			}
			else //��Ŀ¼
			{
				strcat((char *)dstpathname, (const char *)"/");		//Ŀ��·����б��
				strcat((char *)dstpathname, (const char *)fn);		//Ŀ��·�����ļ���
				strcat((char *)srcpathname, (const char *)fn);		//Դ·�����ļ���
																	//				printf("\r\ncopy file %s to %s\r\n",srcpathname,dstpathname);//�����ļ�
				fs_FileCopy(srcpathname, dstpathname, fwmode, cpmode); //�����ļ�
			}
			srcpathname[srcpathlen] = 0; //���������
			dstpathname[dstpathlen] = 0; //���������
		}
		free(dstpathname);
		free(srcpathname);
		free(fn);
	}
	free(srcdir);
	free(dstdir);
	free(finfo);
	// �Ƿ���Դ�ļ���
	if (cpmode)
		fs_DelDir(pSrcDir);
	return res;
}


void read_write_flie_test(void)
{
	FIL fileTmp;

	UINT fnum;					 /* �ļ��ɹ���д���� */
	BYTE ReadBuffer[1024] = {0}; /* �������� */
	BYTE WriteBuffer[] =		 /* д������*/
		"��ӭʹ��Ұ��STM32 F429������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";
	FRESULT result; //�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");
	result = f_open(&fileTmp, "2:test.txt\0", FA_CREATE_ALWAYS | FA_WRITE);
	if (result == FR_OK)
	{
		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
		/* ��ָ���洢������д�뵽�ļ��� */
		result = f_write(&fileTmp, WriteBuffer, sizeof(WriteBuffer), &fnum);
		if (result == FR_OK)
		{
			printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n", fnum);
			printf("�����ļ�д�������Ϊ��\r\n%s\r\n", WriteBuffer);
		}
		else
		{
			printf("�����ļ�д��ʧ�ܣ�(%d)\n", result);
		}
		/* ���ٶ�д���ر��ļ� */
		f_close(&fileTmp);
	}
	else
	{
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}

	/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	result = f_open(&fileTmp, "2:test.txt\0", FA_OPEN_EXISTING | FA_READ);
	if (result == FR_OK)
	{
		printf("�����ļ��ɹ���\r\n");
		result = f_read(&fileTmp, ReadBuffer, sizeof(ReadBuffer), &fnum);
		if (result == FR_OK)
		{
			printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
			printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);
		}
		else
		{
			printf("�����ļ���ȡʧ�ܣ�(%d)\n", result);
		}
	}
	else
	{
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fileTmp);
}

void test_CopyVoiceData(void)
{
  static uint8_t time = 0;
  FIL fileTmp;
  UINT fnum;                      /* �ļ��ɹ���д���� */
  BYTE ReadBuffer[4096] = {0};    /* �������� */
  BYTE ReadBufferCmp[4096] = {0}; /* �������� */
  FRESULT result;                 //�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���
  uint32_t fileSize;
  uint16_t SectorSum = 0;
  uint32_t NumOfSingle = 0;

  if (time == 1)
    return;
  time = 1;
  fileSize = fs_GetFileSize("2:test.bin\0");
  printf("���ļ���С %d byte\r\n", fileSize);
  SectorSum = fileSize / ExFlashTypeList[FLASH_MSC].sectorSize + 1;
  NumOfSingle = fileSize % ExFlashTypeList[FLASH_MSC].sectorSize;
  if (NumOfSingle == 0)
  {
    NumOfSingle = ExFlashTypeList[FLASH_MSC].sectorSize;
  }
  result = f_open(&fileTmp, "2:test.bin\0", FA_READ);
  if (result == FR_OK)
  {
    SPI_Flash_WAKEUP(FLASH_MSC);
    printf("�����ļ��ɹ���\r\n");
    for (uint16_t i = 0; i < SectorSum; i++)
    {
      memset(ReadBuffer, 0xFF, 4096);
      f_lseek(&fileTmp, i * 4096);
      if (i == SectorSum - 1)
      {
        result = f_read(&fileTmp, ReadBuffer, NumOfSingle, &fnum);
      }
      else
      {
        result = f_read(&fileTmp, ReadBuffer, sizeof(ReadBuffer), &fnum);
      }

      if (result == FR_OK)
      {
        uint8_t TransferStatus1;
        if (i == SectorSum - 1)
        {
          SPI_FLASH_BufferWrite(FLASH_MSC, ReadBuffer, i * 4096, NumOfSingle);
          SPI_FLASH_BufferRead(FLASH_MSC, ReadBufferCmp, i * 4096, NumOfSingle);
          TransferStatus1 = Buffercmp(ReadBuffer, ReadBufferCmp, NumOfSingle);
        }
        else
        {
          SPI_FLASH_BufferWrite(FLASH_MSC, ReadBuffer, i * 4096, 4096);
          SPI_FLASH_BufferRead(FLASH_MSC, ReadBufferCmp, i * 4096, 4096);
          TransferStatus1 = Buffercmp(ReadBuffer, ReadBufferCmp, 4096);
        }

        if (TransferStatus1 == 0)
        {
          printf("��������ʧ��! ����%d\n\r", i);
          break;
        }
        if (i == SectorSum - 1)
        {
          printf("������д���Գɹ�!\n\r");
        }
      }
      else
      {
        printf("�����ļ���ȡʧ�ܣ�(%d)\n", result);
        break;
      }
    }
  }
  else
  {
    printf("�������ļ�ʧ�ܡ�\r\n");
  }
  /* ���ٶ�д���ر��ļ� */
  f_close(&fileTmp);
}

/******************************** end of fileTmp *********************************/
