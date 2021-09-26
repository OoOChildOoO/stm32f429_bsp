/*
 * @Author: your name
 * @Date: 2021-09-17 11:10:19
 * @LastEditTime: 2021-09-25 17:19:50
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\file_oper\fil_edit.h
 */
/*******************************************************************************
Copyright	:	JACK SEWING MACHINE CO.,LTD
File name	:	file_edit.h
Description	:	�ļ��༭h�ļ�
History		:	Date		Version	Author	note
				2019/08/21	v1.0.0	chenff	��ʼ�汾
*******************************************************************************/

#ifndef FILE_EDIT_H__
#define FILE_EDIT_H__



extern int opendxf(char *filepath);
extern void FileMakeTest(FIL fil);
extern int makeBinaryFile(char *filepath);
extern uint8_t Format(TCHAR *memory);

extern char first_line[400];//����
extern char second_line[400];//��ֵ
extern Array *av;

//extern FIL fil;        /* File object */
//extern FIL fil1;        /* File object */

extern Array *openPatternFile(char *fil_path);
extern RES_t savePatternFile(char *fil_path, Array *arr);
/******************************************************************************/
//�������ݽ���ģ���ڲ�ʹ��(ͬ��.c�ļ�)
#ifdef FILE_EDIT_C__
/******************************************************************************/


/******************************************************************************/
#endif
/******************************************************************************/

#endif
