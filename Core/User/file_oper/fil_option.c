/*******************************************************************************
Copyright	:	JACK SEWING MACHINE CO.,LTD
File name	:	fil_option.c
Description	:	文件操作c文件
History		:	Date		Version	Author	note
				2019/08/07	v1.0.0	xiongll	初始版本
*******************************************************************************/
#define __FIL_OPTION_C__
/******************************************************************************/
#include "fil_option.h"
/******************************************************************************/
FATFS *fs[FF_VOLUMES]; //逻辑磁盘工作区.
FIL *file;			   //文件1
FIL *ftemp;			   //文件2
UINT br, bw;		   //读写变量
FILINFO fileinfo;	   //文件信息
DIR dir;			   //目录

uint8_t *fatbuf; //SD卡数据缓存区
/******************************************************************************/

/**
 * @description: 初始化文件系统相关指针
 * @param {*}
 * @return {*}
 */
uint8_t fs_FilFuns_Init(void)
{
	uint8_t i;
	for (i = 0; i < FF_VOLUMES; i++)
	{
		fs[i] = (FATFS *)malloc(sizeof(FATFS)); //为磁盘i工作区申请内存
		if (!fs[i])
			break;
	}
	file = (FIL *)malloc(sizeof(FIL));	//为file申请内存
	ftemp = (FIL *)malloc(sizeof(FIL)); //为ftemp申请内存
	fatbuf = (uint8_t *)malloc(512);	//为fatbuf申请内存

	//申请有一个失败,即失败.
	if (i == FF_VOLUMES && file && ftemp && fatbuf)
		return 0;
	else
		return 1;
}

/**
 * @description: 文件复制
 * @param {uint8_t} *pSrcFile 源文件路径(含文件名)
 * @param {uint8_t} *pDstFile 目标文件路径(含文件名)
 * @param {uint8_t} fwmode 0:不覆盖,新建 1:覆盖
 * @param {uint8_t} cpmode 0:保留源文件 1:删除源文件
 * @return {*}
 */
uint8_t fs_FileCopy(uint8_t *pSrcFile, uint8_t *pDstFile, uint8_t fwmode, uint8_t cpmode)
{
	FRESULT fr;	 // FatFs函数公共返回代码
	UINT br, bw; // 文件读写计数
	FIL *fsrc = 0;
	FIL *fdst = 0;
	uint8_t *buffer = 0;
	fsrc = (FIL *)malloc(sizeof(FIL)); //文件对象申请内存
	fdst = (FIL *)malloc(sizeof(FIL));
	buffer = (uint8_t *)malloc(1024); // 拷贝文件缓冲区

	if (fwmode == 0)
		fwmode = FA_CREATE_NEW; //不覆盖
	else
		fwmode = FA_CREATE_ALWAYS; //覆盖存在的文件

	// 在驱动器上打开源文件
	fr = f_open(fsrc, (const TCHAR *)pSrcFile, FA_READ);
	if (fr)
		return (uint8_t)fr;
	// 在驱动器上创造目标文件
	fr = f_open(fdst, (const TCHAR *)pDstFile, FA_WRITE | fwmode);
	if (fr)
		return (uint8_t)fr;
	// 将源文件复制到目标文件上
	for (;;)
	{
		// 读源文件块
		fr = f_read(fsrc, buffer, sizeof buffer, &br);
		// 错误或者eof
		if (fr || br == 0)
			break;
		// 写入目标文件
		fr = f_write(fdst, buffer, br, &bw);
		// 错误或者磁盘已满
		if (fr || bw < br)
			break;
	}
	// 关闭文件
	f_close(fsrc);
	f_close(fdst);
	// 复制后是否保留源文件
	if (cpmode)
		f_unlink((const TCHAR *)pSrcFile);
	// 释放内存
	free(fsrc);
	free(fdst);
	free(buffer);
	return (uint8_t)fr;
}

/**
 * @description: 扫描指定路径(不含文件名)
 * @param {uint8_t} *path 路径
 * @return {*}
 */
uint8_t fs_ScanDir(uint8_t *path)
{
	FRESULT res;
	res = f_opendir(&dir, (const TCHAR *)path); //打开一个目录
	if (res == FR_OK)
	{
		printf("\r\n");
		while (1)
		{
			res = f_readdir(&dir, &fileinfo);			//读取目录下的一个文件
			if (res != FR_OK || fileinfo.fname[0] == 0) //错误了/到末尾了,退出
				break;
			//if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
			printf("%s/", path);			  //打印路径
			printf("%s\r\n", fileinfo.fname); //打印文件名
		}
	}
	return res;
}

/**
 * @description: 仅提取文件的上级路径名(不含文件名)
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
	while ((*path != 0x5c) && (*path != 0x2f)) //追述到倒数第一个"\"或者"/"处
	{
		path--;
	}
	return ++path;
}

/**
 * @description: 获取文件大小
 * @param {uint8_t} *path
 * @return {*}
 */
uint32_t fs_GetFileSize(uint8_t *path)
{
	FRESULT fr; // FatFs函数公共返回代码
	uint32_t file_size = 0;
	FIL *fsrc = 0;

	fsrc = (FIL *)malloc(sizeof(FIL)); //文件对象申请内存

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
 * @description: 获取目录内所有文件大小
 * @param {uint8_t} *path
 * @return {*} 100：内存申请失败 101：内存申请失败
 */
uint32_t fs_GetDirSize(uint8_t *path)
{
	uint16_t MAX_PATHNAME_DEPTH = 512 + 1; //最大目标文件路径+文件名深度
	uint8_t res = 0;
	DIR *fddir = 0;		   //目录
	FILINFO *finfo = 0;	   //文件信息
	uint8_t *pathname = 0; //目标文件夹路径+文件名
	uint16_t pathlen = 0;  //目标路径长度
	uint32_t fdsize = 0;

	fddir = (DIR *)malloc(sizeof(DIR)); //申请内存
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
			strcat((char *)pathname, (const char *)path); //复制路径
			res = f_opendir(fddir, (const TCHAR *)path);  //打开源目录
			if (res == 0)								  //打开目录成功
			{
				while (res == 0)
				{
					res = f_readdir(fddir, finfo); //读取目录下的一个文件
					if (res != FR_OK || finfo->fname[0] == 0)
						break; //错误了/到末尾了,退出
					if (finfo->fname[0] == '.')
						continue;			   //忽略上级目录
					if (finfo->fattrib & 0X10) //是子目录(文件属性,0X20,归档文件;0X10,子目录;)
					{
						pathlen = strlen((const char *)pathname);			  //得到当前路径的长度
						strcat((char *)pathname, (const char *)"/");		  //加斜杠
						strcat((char *)pathname, (const char *)finfo->fname); //源路径加上子目录名字
						fdsize += fs_GetDirSize(pathname);						  //得到子目录大小,递归调用
						pathname[pathlen] = 0;								  //加入结束符
					}
					else
						fdsize += finfo->fsize; //非目录,直接加上文件的大小
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
 * @description: 删除指定文件夹
 * @param {uint8_t} *path
 * @return {*}
 */
uint8_t fs_DelDir(uint8_t *path)
{
	uint16_t i, j;
	uint8_t res;
	DIR *dir = 0;
	FILINFO *finfo = 0; //文件信息

	dir = (DIR *)malloc(sizeof(DIR)); //申请内存
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
 * @description: 拷贝指定路径所有文件
 * @param {uint8_t} *pSrcFile 源文件路径(不含文件名)
 * @param {uint8_t} *pDstFile 目标文件路径(不含文件名)
 * @param {uint8_t} fwmode 0:不覆盖,新建 1:覆盖
 * @param {uint8_t} cpmode 0:保留源文件 1:删除源文件
 * @return {*}
 */
uint8_t fs_DirCopy(uint8_t *pSrcDir, uint8_t *pDstDir, uint8_t fwmode, uint8_t cpmode)
{
	uint16_t MAX_PATHNAME_DEPTH = 512 + 1; //最大目标文件路径+文件名深度
	uint8_t res = 0;
	DIR *srcdir = 0;	//源目录
	DIR *dstdir = 0;	//源目录
	FILINFO *finfo = 0; //文件信息
	uint8_t *fn = 0;	//长文件名

	uint8_t *dstpathname = 0; //目标文件夹路径+文件名
	uint8_t *srcpathname = 0; //源文件夹路径+文件名

	uint16_t dstpathlen = 0; //目标路径长度
	uint16_t srcpathlen = 0; //源路径长度

	srcdir = (DIR *)malloc(sizeof(DIR)); //申请内存
	dstdir = (DIR *)malloc(sizeof(DIR));
	finfo = (FILINFO *)malloc(sizeof(FILINFO));

	if (srcdir == NULL || dstdir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		// 申请最大文件深度空间
		dstpathname = malloc(MAX_PATHNAME_DEPTH);
		srcpathname = malloc(MAX_PATHNAME_DEPTH);
		fn = malloc(MAX_PATHNAME_DEPTH);
		if (dstpathname == NULL || srcpathname == NULL)
			res = 101;
		dstpathname[0] = 0;
		srcpathname[0] = 0;
		strcat((char *)srcpathname, (const char *)pSrcDir); //复制原始源文件路径
		strcat((char *)dstpathname, (const char *)pDstDir); //复制原始目标文件路径
		res = f_opendir(srcdir, (const TCHAR *)pSrcDir);	//打开源目录
		if (res == 0)										//打开目录成功
		{
			fn = fs_GetPathDir(pSrcDir);						// 判断是否是根目录
			strcat((char *)dstpathname, (const char *)"/"); //加入斜杠
			if (fn == 0)									//卷标拷贝
			{
				dstpathlen = strlen((const char *)dstpathname);
				dstpathname[dstpathlen] = pSrcDir[0]; //记录卷标
				dstpathname[dstpathlen + 1] = 0;	  //结束符
				printf("dir:%s\r\n", dstpathname);
			}
			else
			{
				strcat((char *)dstpathname, (const char *)fn); //加文件名
				printf("dir:%s\r\n", dstpathname);
			}
		}
		res = f_mkdir((const TCHAR *)dstpathname); //如果文件夹已经存在,就不创建.如果不存在就创建新的文件夹.
		if (res == FR_EXIST)
			res = 0;
		while (res == 0) //开始复制文件夹里面的内容
		{
			res = f_readdir(srcdir, finfo); //读取目录下的一个文件
			if (res != FR_OK || finfo->fname[0] == 0)
				break; //错误了/到末尾了,退出
			if (finfo->fname[0] == '.')
				continue; //忽略上级目录

			fn = (uint8_t *)finfo->fname;					//得到文件名
			dstpathlen = strlen((const char *)dstpathname); //得到当前目标路径的长度
			srcpathlen = strlen((const char *)srcpathname); //得到源路径长度

			strcat((char *)srcpathname, (const char *)"/"); //源路径加斜杠
			if (finfo->fattrib & 0X10)						//是子目录文件属性,0X20,归档文件;0X10,子目录;
			{
				strcat((char *)srcpathname, (const char *)fn);			 //源路径加上子目录名字
																		 //				printf("\r\ncopy folder %s to %s\r\n",srcpathname,dstpathname);//拷贝文件
				res = fs_DirCopy(srcpathname, dstpathname, fwmode, cpmode); //拷贝文件夹
			}
			else //非目录
			{
				strcat((char *)dstpathname, (const char *)"/");		//目标路径加斜杠
				strcat((char *)dstpathname, (const char *)fn);		//目标路径加文件名
				strcat((char *)srcpathname, (const char *)fn);		//源路径加文件名
																	//				printf("\r\ncopy file %s to %s\r\n",srcpathname,dstpathname);//拷贝文件
				fs_FileCopy(srcpathname, dstpathname, fwmode, cpmode); //复制文件
			}
			srcpathname[srcpathlen] = 0; //加入结束符
			dstpathname[dstpathlen] = 0; //加入结束符
		}
		free(dstpathname);
		free(srcpathname);
		free(fn);
	}
	free(srcdir);
	free(dstdir);
	free(finfo);
	// 是否保留源文件夹
	if (cpmode)
		fs_DelDir(pSrcDir);
	return res;
}


void read_write_flie_test(void)
{
	FIL fileTmp;

	UINT fnum;					 /* 文件成功读写数量 */
	BYTE ReadBuffer[1024] = {0}; /* 读缓冲区 */
	BYTE WriteBuffer[] =		 /* 写缓冲区*/
		"欢迎使用野火STM32 F429开发板 今天是个好日子，新建文件系统测试文件\r\n";
	FRESULT result; //部分在递归过程被修改的变量，不用全局变量
	/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");
	result = f_open(&fileTmp, "2:test.txt\0", FA_CREATE_ALWAYS | FA_WRITE);
	if (result == FR_OK)
	{
		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
		/* 将指定存储区内容写入到文件内 */
		result = f_write(&fileTmp, WriteBuffer, sizeof(WriteBuffer), &fnum);
		if (result == FR_OK)
		{
			printf("》文件写入成功，写入字节数据：%d\n", fnum);
			printf("》向文件写入的数据为：\r\n%s\r\n", WriteBuffer);
		}
		else
		{
			printf("！！文件写入失败：(%d)\n", result);
		}
		/* 不再读写，关闭文件 */
		f_close(&fileTmp);
	}
	else
	{
		printf("！！打开/创建文件失败。\r\n");
	}

	/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	result = f_open(&fileTmp, "2:test.txt\0", FA_OPEN_EXISTING | FA_READ);
	if (result == FR_OK)
	{
		printf("》打开文件成功。\r\n");
		result = f_read(&fileTmp, ReadBuffer, sizeof(ReadBuffer), &fnum);
		if (result == FR_OK)
		{
			printf("》文件读取成功,读到字节数据：%d\r\n", fnum);
			printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
		}
		else
		{
			printf("！！文件读取失败：(%d)\n", result);
		}
	}
	else
	{
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fileTmp);
}

void test_CopyVoiceData(void)
{
  static uint8_t time = 0;
  FIL fileTmp;
  UINT fnum;                      /* 文件成功读写数量 */
  BYTE ReadBuffer[4096] = {0};    /* 读缓冲区 */
  BYTE ReadBufferCmp[4096] = {0}; /* 读缓冲区 */
  FRESULT result;                 //部分在递归过程被修改的变量，不用全局变量
  uint32_t fileSize;
  uint16_t SectorSum = 0;
  uint32_t NumOfSingle = 0;

  if (time == 1)
    return;
  time = 1;
  fileSize = fs_GetFileSize("2:test.bin\0");
  printf("》文件大小 %d byte\r\n", fileSize);
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
    printf("》打开文件成功。\r\n");
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
          printf("！！测试失败! 次数%d\n\r", i);
          break;
        }
        if (i == SectorSum - 1)
        {
          printf("》》读写测试成功!\n\r");
        }
      }
      else
      {
        printf("！！文件读取失败：(%d)\n", result);
        break;
      }
    }
  }
  else
  {
    printf("！！打开文件失败。\r\n");
  }
  /* 不再读写，关闭文件 */
  f_close(&fileTmp);
}

/******************************** end of fileTmp *********************************/
