#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
  
#include   <windows.h>   
#include   <winioctl.h>  
#include "adt860api.h" 

#pragma	comment(lib,"adt860dll.lib")

HANDLE hEvent;
HANDLE hWdm1;
HANDLE  hFile;
unsigned short buffer[4096] = {'\0'};
int eventflag = 0;
int exitflag = 0;

//辅助线程部分：
DWORD WINAPI ThreadProc(LPVOID junkola)
{   // ThreadProc
   DWORD junk,i;
   static DWORD jun=0;
   DWORD ju=0;

   while (TRUE)
   {
		Sleep(10);
	    //DWORD s = WaitForSingleObject(hEvent, INFINITE); 
		//DWORD s = WaitForSingleObject(hEvent, 1000);       //等待事件变成有信号状态。
		ZeroMemory(buffer, 8192);
		if (ReadFile(hWdm1, buffer, 8192, &junk, NULL) != 0)
		{ 

			if (junk)
			{
				jun+=junk;
				printf("%lx %2x	::",jun,junk);
				//printf("Rec %ld:%8lx,", junk,jun);
				//for ( i=0; (DWORD)i < junk; i+=8)
				for ( i=0; (DWORD)i < 8; i++)
				{
					printf(" %4x",buffer[i]);
				}
				printf("\n");
			}
			else printf("noRec");

		} 
		else
			printf("Error %d in call to ReadFile\n", GetLastError());
		if (ju++ >= 0x1000)
			exitflag = 1;

   }
       return 0;

}    

int main(int   argc,char   *argv[])   
{  
	DWORD junk;
//	UCHAR	value;
	UCHAR	data[12];
	//int i;
	io8255 		adt860iod;//io8255是一个结构体
	io8255 *	adt860io =&adt860iod;
   static DWORD j = 0;	
    DWORD	uexitcode;

	hWdm1 = ADT860_DevLoad(0);
	if( hWdm1==INVALID_HANDLE_VALUE)
	{
                printf("无法加载驱动程序！");
		        exit(1);
	}

	printf("     Opened OK\n");
	ADT860_RstADT(	hWdm1);

//DA
	data[0] = 3; //DA modeval	DA range mode value:0(-5--5),1(-10--10),2(0--5),3(0--10)
	ADT860_SetDABootMode(hWdm1, data[0]);
	Sleep(20);
	ADT860_DAConvert(hWdm1, 0, 0xfff);
	ADT860_DAConvert(hWdm1, 1, 0x800);
	ADT860_DAConvert(hWdm1, 2, 0x400);
	ADT860_DAConvert(hWdm1, 3, 0x00);

//IO  struct  io8255
	adt860io->modesel = 1;
	adt860io->modegrp0 = 0;
	adt860io->modegrp1 = 0;
	adt860io->dira = adt860io->dirb = adt860io->dircl = adt860io->dirch = 0;
	adt860io->paval = 0xaa;
	adt860io->pbval = 0x55;
	adt860io->pclval = 0x0f;
	adt860io->pchval = 0x0f;
	ADT860_Iomd0Oper(hWdm1, adt860io);
	////--------也可以用以下的方式进行IO测试----//
#if 1
//	ADT860_ReadIOPort(hWdm1, uchar portoff, uchar *portval);
	ADT860_ConfigIOPorts(hWdm1,0,0,0,0);//全部为输出
	ADT860_WriteDigitIO(hWdm1, 0, 0xff);
	ADT860_WriteDigitIO(hWdm1, 1, 0xaa);
	ADT860_WriteDigitIO(hWdm1, 2, 0x55);
#else
	ADT860_ConfigIOPorts(hWdm1,1,1,1,1);//全部为输入
	int pValue=ADT860_ReadDigitIO(hWdm1,0);
	printf("PA:%d",pValue);
	pValue=ADT860_ReadDigitIO(hWdm1,1);
	printf("PB:%d",pValue);
	pValue=ADT860_ReadDigitIO(hWdm1,2);
	printf("PC:%d",pValue);

#endif
//AD
	ADT860_SetADBootMode(hWdm1, 12);	//modeval  is 12 0--10v
	Sleep(20);

	ADT860_ADConfig(hWdm1, 1, 1, 0, 0);//range, polarity, gain
	ADT860_SetChannel(hWdm1, 7, 0);
	ADT860_ADFIFOReset(hWdm1);
	ADT860_FIFOEN(hWdm1, 1);
	ADT860_ADTrigSet(hWdm1, 3);//1--soft,2--Ext,3--hard  Timer
	ADT860_SelIRQSource(hWdm1, (ADTIMER)1);//1--AD interrupt as interrupt source,
	ADT860_Timer1FRQ(hWdm1, (FRQCLK)0);//0--10M,1--1M  as timer1 clock frequency,
	ADT860_ClearIRQ(hWdm1);
	
	ADT860_ClockMode(hWdm1, 1, 3);//timer 1, mode 3
	//10M--timer1,  10--  timer 1 out 1000 000
	ADT860_ClockDivisor(hWdm1, 1, 10);
	//timer 1 out as timer2 clock source
	ADT860_ClockMode(hWdm1, 2, 3);
	ADT860_ClockDivisor(hWdm1, 2, 6);//   x--timer 2 out ---trig AD


	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);         //创建一个事件
ADT860_SetEvent(hWdm1,hEvent);
	HANDLE hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &junk);
/*	LPSECURITY_ATTRIBUTESlpThreadAttributes,*///线程安全属性
//DWORDdwStackSize,//堆栈大小
//LPTHREAD_START_ROUTINElpStartAddress,//线程函数
//LPVOIDlpParameter,//线程参数
//DWORDdwCreationFlags,//线程创建属性
//LPDWORDlpThreadId//线程ID
	//testgit
	while (1){
		
		Sleep(2);
		if (exitflag)
			break;
	}

	ADT860_ADTrigSet(hWdm1, 1);
	CloseHandle(hEvent);
	GetExitCodeThread(hThread, &uexitcode);

	ADT860_DevClose(hWdm1);

	return 0;
}
                           