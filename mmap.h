#ifndef __MMAP_H__
#define __MMAP_H_
#include <stdio.h>  
#include <string.h>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define READ 1
#define WRITE 2
#define WRITE_SUCCESS 0
#define WRITE_ERROR -1
#define WRITE_FULL 1

using namespace std;

class CMMAP
{

public:
	CMMAP(const char* pFile,int type,int size);
	virtual ~CMMAP();
public:
	bool InitMmapWrite();
	bool InitMmapRead();
	int writeData(const char* pData,int nLen);
	const char* readData();
private:
	int m_nMmapSize;
	int m_nWriteLen;
	const char* m_pFile;
	int m_nFd;
	char* m_pAddr;
	bool m_bWrite;
};

const char* CMMAP::readData()
{
	return m_pAddr;
}

CMMAP::CMMAP(const char* pFile,int type,int size=0)
{
	m_pFile = pFile;
	if(type == READ)
	{
		m_bWrite = false;
		InitMmapRead();
	}
	else if(type == WRITE)
	{
		m_bWrite = true;
		m_nWriteLen = 0;
		m_nMmapSize = (size == 0) ? 10*1024*1024 : size;
		if(!InitMmapWrite())
		{
			printf("\033[31m mmap file fail \033[0m\n");
		}
	}
}

int CMMAP::writeData(const char* pData,int nLen)
{
	if(pData == NULL || m_pAddr == NULL)
	{
		fprintf(stderr,"write err\n");
		return WRITE_ERROR;
	}

	if(m_nWriteLen + nLen >= m_nMmapSize)
	{
		munmap(m_pAddr, m_nMmapSize);
		m_pAddr = NULL;
		return WRITE_FULL;
	}
		
	memcpy(m_pAddr+m_nWriteLen,pData,nLen);
	m_nWriteLen += nLen;
	return WRITE_SUCCESS;
}

CMMAP::~CMMAP()
{
	(m_pAddr != NULL) ? munmap(m_pAddr, m_nMmapSize) : 0;
	
	if(m_bWrite && m_nFd > 0)
	{
		ftruncate(m_nFd,m_nWriteLen);
		close(m_nFd);
	}
}

bool CMMAP::InitMmapWrite()
{
	if(m_pFile == NULL || m_nMmapSize <= 0)
		return false;
	
	if((m_nFd=open(m_pFile, O_RDWR | O_CREAT, 0644)) < 0)  
		return false; 	

	if(0 != ftruncate(m_nFd,m_nMmapSize))
		return false;

	if((m_pAddr=(char*)mmap(NULL,m_nMmapSize,PROT_WRITE|PROT_READ,MAP_SHARED,m_nFd,0)) == NULL)
		return false;
		
	return true;
}

bool CMMAP::InitMmapRead()
{
	if(m_pFile == NULL)
		return false;

	struct stat statbuf;

	if(stat(m_pFile, &statbuf) == -1)
	      return false;

	m_nMmapSize = statbuf.st_size;

	if((m_nFd=open(m_pFile, O_RDONLY)) < 0)  
	    return false;  

	if((m_pAddr=(char *)mmap(0, m_nMmapSize, PROT_READ, MAP_SHARED,m_nFd, 0)) == NULL)
		return false;
	
	close(m_nFd);
	return true;
}
#endif
