/*
 * @Author: your name
 * @Date: 2021-09-14 17:36:41
 * @LastEditTime: 2021-09-25 17:17:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \simple_7inches_v2\Core\User\file_oper\fil_option.h
 */
#ifndef __FIL_OPTION_H__
#define __FIL_OPTION_H__
/******************************************************************************/
#include "ff.h"
#include "usbh_conf.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include <stdlib.h>
#include <string.h>
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
extern FATFS *fs[FF_VOLUMES];
extern FIL *file;
extern FIL *ftemp;
extern UINT br, bw;
extern FILINFO fileinfo;
extern DIR dir;
extern uint8_t *fatbuf; //SD卡数据缓存区

uint8_t fs_FilFuns_Init(void);
uint8_t fs_FileCopy(uint8_t *pSrcFile, uint8_t *pDstFile, uint8_t fwmode, uint8_t cpmode);
uint8_t *fs_GetPathDir(uint8_t *path);
uint32_t fs_GetFileSize(uint8_t *path);
uint32_t fs_GetDirSize(uint8_t *path);
uint8_t fs_DelDir(uint8_t *path);
uint8_t fs_DirCopy(uint8_t *pSrcDir, uint8_t *pDstDir, uint8_t fwmode, uint8_t cpmode);
uint8_t fs_ScanDir(uint8_t *path);

void read_write_flie_test(void);

/******************************************************************************/
//以下内容仅供模块内部使用(同名.c文件)
#ifdef __FIL_OPTION_C__
/******************************************************************************/

/******************************************************************************/
#endif
/******************************************************************************/
#endif
/******************************************************************************/
/******************************** end of file *********************************/
