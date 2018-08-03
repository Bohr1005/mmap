#include <stdio.h>
#include "mmap.h"
#define FILENAME(x) "file_"##x

int main(int argc, char** argv)
{

	int nFile = 0;
	int nRet = 0;
	char cTmp[15] = "";
	char cFileName[20] = "";
	sprintf(cFileName,"test%d.txt",++nFile);

	//mmap write file
	CMMAP* pWrite = new CMMAP(cFileName,WRITE,2*1024*1024);
	for(int i = 0; i < 1000000; i++)
	{
		sprintf(cTmp,"%09d\n",i);
		nRet = pWrite->writeData(cTmp,10);
		if(nRet == WRITE_FULL)
		{
			delete pWrite;
			sprintf(cFileName,"test%d.txt",++nFile);
			printf("create new file %s\n",cFileName);
			pWrite = new CMMAP(cFileName,WRITE,2*1024*1024);
		}
		else if(nRet != WRITE_SUCCESS)
		{
			printf("write error\n");
		}
	}

	//mmap read file
	CMMAP* pRead = new CMMAP("test.txt",READ);
	printf("%.1024s\n",pRead->readData());

	delete pRead;
	delete pWrite;
	return 0;
}
