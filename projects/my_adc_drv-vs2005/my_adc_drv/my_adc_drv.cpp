// my_adc_drv.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "bsp.h"
#include <winsock2.h>
#include <s3c6410.h>
#include <DrvLib.h>
#include <winuser.h>
#include <ceddk.h>
#include <shlwapi.h>

#include "ADC.h"

#pragma comment(lib, "ceddk.lib")
#pragma comment(lib, "Mmtimer.lib")
#pragma comment(lib, "ws2.lib")
struct DebugInfo
{
	DWORD dwInfo1;
	DWORD dwInfo2;
	DWORD dwInfo3;
	DWORD dwInfo4;
};

struct MYDATA
{
	DWORD time;
	DWORD data;
};


struct MYDATA1
{
	DWORD HighTime;
	DWORD LowTime;
	DWORD Data;
};

struct MYBUF
{
	PVOID pBuf;
	DWORD dwMaxIndex;
	DWORD *pIndex;
	DWORD *pMark;
	DWORD *pDebug1;
	DWORD *pDebug2;
	DWORD *pDebug3;
	DWORD *pDebug4;
};

struct MYBUF1
{
	MYDATA Datas[5000];
	
	DWORD dwMaxIndex;
	DWORD Index;
	DWORD dwDebug;
};

//#define ADC_FREQ 2500000
#define ADC_FREQ 5000000
// adc virtual address
static volatile S3C6410_ADC_REG  *v_pADCPregs  ;

// global variable
HANDLE hThread;
HANDLE hAdcEvent;
unsigned int g_convertervalue;

//MYBUF *pMyBuf;
MYBUF1 *pMyBuf1;
MYDATA *pDatas;
DWORD dwMaxIndex;
DWORD *pIndex;

DWORD Mark;

MYDATA1 MyData;

SOCKET s;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: 
      //OutputDebugString(L"\nADC - DLL_PROCESS_ATTACH\n"); 
	  //RETAILMSG(1,(TEXT("ADC - DLL_PROCESS_ATTACH\n")));
   		break; 
    case DLL_PROCESS_DETACH: 
			break; 
    case DLL_THREAD_ATTACH: 
			break; 
    case DLL_THREAD_DETACH: 
			break; 
    default: 
			break; 
  }
	//pMyBuf = NULL;
	//g_convertervalue = 0;
	//MessageBox(NULL, L"DLLMain finish!", L"Debug", MB_OK);               
    return TRUE;
}


//----------------------------------------------------------------------------//
//	Delay																																	//
//----------------------------------------------------------------------------//
static void Delay(UINT32 count)
{
    volatile int i, j = 0;
    volatile static int loop = S3C6410_ACLK/100000;
    
    for(;count > 0;count--)
        for(i=0;i < loop; i++) { j++; }
}

//----------------------------------------------------------------------------//
//	AD Converter Thread																																	//
//----------------------------------------------------------------------------//
DWORD WINAPI AdcThread (PVOID hdcMain)
{	

	::CeSetThreadPriority(::GetCurrentThread(), 10);
	int	preScaler = S3C6410_PCLK/ADC_FREQ -1;
	//WCHAR buf[1024];
	//wsprintf(buf, L"preScaler is %d", preScaler);
	//MessageBox(NULL, buf, L"Debug", MB_OK);

	//pMyBuf = NULL;
	g_convertervalue = 0;
	hAdcEvent = NULL;
	v_pADCPregs->ADCCON = (1<<14)|(preScaler<<6)|(1<<3)|(0<<2);   //channel 0

	DWORD lastt;
	lastt = 0;
	DWORD NumOfSame;
	DWORD SumOfSame;
	NumOfSame = 0;
	SumOfSame = 0;


	
	DWORD Index;
	Index = 0;
	//Delay(10);
	DWORD LastIndex;
	//
	hAdcEvent = CreateEvent(NULL, TRUE, FALSE, _T("TE6410_ADC"));
	//while (pMyBuf == NULL)
	//{
	//	g_convertervalue = 0xaaaaaaaa;
	//	Sleep(50);
	//}

	//g_convertervalue = 0x77777777;
	//g_convertervalue = 0x76767676;
	//
	//g_convertervalue = 0x68686868;
	//BOOL bShown;
	//bShown = FALSE;
	HANDLE hMapObj;
	hMapObj = NULL;
	
	LPVOID pMem;
	
	MYBUF1 *pBuf1;
	pBuf1 = NULL;
	pBuf1 = new MYBUF1[1];
	Mark = 0;
	DWORD Count;
	Count = 0;

	WaitForSingleObject(hAdcEvent, INFINITE);
int r;
	WSADATA WSAData;
	
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	BOOL opt;
	opt = TRUE;
	r = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(BOOL));
	if (r == -1)
	{
		Sleep(50000);
	}

	if (s == INVALID_SOCKET)
	{
		//AfxMessageBox(L"Create Socket failed!");
		return FALSE;
	}

	int nZero = 0;
setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(30000);
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.2");
	
	r = bind(s, (sockaddr *)&addr, sizeof(sockaddr));
	if (r == SOCKET_ERROR)
	{
		//AfxMessageBox(L"bind failed!");
		return FALSE;
	}

	sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(30000);
	addr1.sin_addr.S_un.S_addr = inet_addr("192.168.1.1");
	r = connect(s, (sockaddr *)&addr1, sizeof(sockaddr));
	if (r == SOCKET_ERROR)
	{
		int errcode;
		errcode = WSAGetLastError();
		WCHAR buf[16];
		wsprintf(buf, L"errcode is %d", errcode);
		//AfxMessageBox(buf);
		//AfxMessageBox(L"connect failed!");
		return FALSE;
	}
	/*
	MyData.Data = 0x11111111;
	QueryPerformanceFrequency((LARGE_INTEGER *)&MyData);

	DWORD ccc;
	ccc = 4;
	send(s, (char *)&MyData, sizeof(MYDATA1), 0);
	*/

	//Sleep(3000);

	
	BOOL CanStart;
	CanStart = FALSE;
	while (!CanStart)
	{
		recv(s, (char *)&MyData, sizeof(MYDATA1), 0);
		if (MyData.Data == 0x55555555)
		{
			MyData.Data = 0x66666666;
			MyData.LowTime = timeGetTime();
			send(s, (char *)&MyData, sizeof(MYDATA1), 0);
			CanStart = TRUE;
		}
	}

	//Sleep(3000);
	while(1)
	{
		char str[16];
		//sprintf(str, "%08x", ccc++);

		//send(s, str, 8, 0);
		//if (bShown == FALSE)
		//{
			//MessageBox(NULL, L"be ready to wait for event!", L"Debug", MB_OK);
		//}

		//wsprintf(buf, L"hEvent is %08x", hAdcEvent);
		//MessageBox(NULL, buf, L"Debug", MB_OK);
		
		//g_convertervalue = 0x66666666;
		//while (pMyBuf == NULL)
		//{
			//g_convertervalue = 0x33333333;
			//Sleep(50);
		//}
		//(*(pMyBuf->pDebug2)) = 1;
		//DWORD *pp;
		//pp = pMyBuf->pDebug2;
		//*pp = 1;
		//g_convertervalue = 0x55555555;
		//if (pMyBuf != NULL)
		//{
			//(*(pMyBuf->pDebug1)) = 0xcccccccc;
		//}
		
		/*
		if (hMapObj == NULL)
		{
			hMapObj = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(MYBUF1), L"ADC_Data_Buffer");
			DWORD code;
			code = GetLastError();
			if (code == ERROR_ALREADY_EXISTS)
			{
				if ((hMapObj != NULL) && (hMapObj != INVALID_HANDLE_VALUE))
				{
					//MessageBox(NULL, L"Open file mapping ok!", L"Debug", MB_OK);
				}
				else
				{
					MessageBox(NULL, L"Open failed!", L"Debug", MB_OK);
				}
			}
			if (hMapObj == INVALID_HANDLE_VALUE)
			{
				MessageBox(NULL, L"Open File Mapping failed!", L"Debug", MB_OK);
			}
			else
			{

				pMem = MapViewOfFile(hMapObj, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (pMem == NULL)
				{
					MessageBox(NULL, L"Map view of file failed!", L"Debug", MB_OK);
				}
				pBuf1 = (MYBUF1 *)pMem;
			}
		}

		if (pBuf1 == NULL)
		{
			continue;
		}
		*/
		//g_convertervalue = 0x44444444;
		//(*(pMyBuf->pDebug1)) = 1;
		//pp = pMyBuf->pDebug1;
		//*pp = 1;

		//if (bShown == FALSE)
		//{
		//	bShown = TRUE;
			//MessageBox(NULL, L"Event set!", L"Debug", MB_OK);
		//}
		//pBuf1->dwDebug = Mark;
		//LARGE_INTEGER ttt;
		//QueryPerformanceFrequency(&ttt);

		//BOOL Done = FALSE;
		//while (!Done)
		//{
		v_pADCPregs->ADCCON = (1<<14)|(preScaler<<6)|(1<<3)|(0<<2);   //channel 0

		v_pADCPregs->ADCCON|=0x1;   //start ADC
		//MessageBox(NULL, L"Set ADC started", L"Debug_Driver", MB_OK);
		while(v_pADCPregs->ADCCON & 0x1);	//check if Enable_start is low
		//MessageBox(NULL, L"Enable_start is low", L"Debug_Driver", MB_OK);
		//LastIndex = *pIndex;
		//(*(pMyBuf->pDebug2)) = 1;
		while(!(v_pADCPregs->ADCCON & 0x8000));	//check if EC(End of Conversion) flag is high
		//MessageBox(NULL, L"End of Conversion", L"Debug_Driver", MB_OK);
		//wsprintf(buf, L"Info:pIndex%08x,*pIndex%08x,dwMaxIndex%d, Index%08x, pDatas%08x,pDatas[Index].data%08x", pIndex, *pIndex, dwMaxIndex, Index, pDatas, pDatas[Index].data);
		//MessageBox(NULL, buf, L"Debug_key", MB_OK);
		//(*(pMyBuf->pDebug3)) = 1;
		/*
		//在等待转换结束的时间前准备接受缓冲区
		if ((pBuf1->Index) > (pBuf1->dwMaxIndex))
		{
			//MessageBox(NULL, L"pIndex > dwMaxIndex", L"Debug", MB_OK);
			//(*(pMyBuf->pMark)) = (*pIndex);
			//(*pIndex) = 0;
			pBuf1->Index = 0;
		}

		
		
		Index = pBuf1->Index;
		*/
		//TCHAR buf[256];
		//wsprintf(buf, L"Index is %d,*pIndex is %08x", Index, *pIndex);
		//MessageBox(NULL, buf, L"Debug_DRIVER", MB_OK);
		//wsprintf(buf, L"Index is %d,*pIndex is %08x", Index, *pIndex);
		//MessageBox(NULL, buf, L"Debug_DRIVER_key", MB_OK);
		//???获取时间的时机定在哪里好？
		g_convertervalue = (int)v_pADCPregs->ADCDAT0 & 0x3ff;
		//MyData.Data = (DWORD)v_pADCPregs->ADCDAT0 & 0x3ff;
		//LARGE_INTEGER ttt1;
		//QueryPerformanceCounter(
		//QueryPerformanceCounter((LARGE_INTEGER *)&MyData);
		//send(s, (char *)&MyData, sizeof(MYDATA1), 0);
		//(*(pMyBuf->pDebug4)) = 1;
		//wsprintf(buf, L"Index is %d,pIndex is %08x", Index, pIndex);
		//MessageBox(NULL, buf, L"Debug_DRIVER1", MB_OK);
		//wsprintf(buf, L"convertion result is %d", g_convertervalue);
		//MessageBox(NULL, buf, L"Debug_DRIVER", MB_OK);
		
		DWORD tt;
		tt = timeGetTime();
		if (lastt == 0)
		{
			lastt = tt;
		}
		//wsprintf(buf, L"Index is %d,pIndex is %08x", Index, pIndex);
		//MessageBox(NULL, buf, L"Debug_DRIVER2", MB_OK);
		//MessageBox(NULL, L"timeGetTime done!", L"Debug", MB_OK);

		//send(s, (char *)&lastt, 4, 0);
		//send(s, (char *)&tt, 4, 0);
		//send(s, (char *)&Index, 4, 0);
		//send(s, (char *)&NumOfSame, 4, 0);
		if (lastt == tt)//如果仍是只是相同的时间戳，则对所有相同时间戳的数据求平均数，以后可改用更高分辨率的时间戳来提高精度
		{
			//MessageBox(NULL, L"1111lastt == t", L"Debug1", MB_OK);
			//send(s, "11111111", 8, 0);
			NumOfSame++;
			SumOfSame += g_convertervalue;
			
			//pBuf1->Datas[LastIndex].data = (double)SumOfSame / (double)NumOfSame;
			//pDatas[LastIndex].data = (double)SumOfSame / (double)NumOfSame;
			//MessageBox(NULL, L"lastt == t", L"Debug1", MB_OK);
			//presult[index].time = tt;
		}
		else
		{
			//if (pDatas[Index].data != 0x33333333)
			{
				//MessageBox(NULL, L"Error in pDatas", L"Error", MB_OK);

				//wsprintf(buf, L"Info:%08x,%08x,%d, %08x, %08x,%08x", pIndex, *pIndex, dwMaxIndex, Index, pDatas, pDatas[Index].data);
				//MessageBox(NULL, buf, L"Debug_key", MB_OK);
			}
			//wsprintf(buf, L"1234FIRST %08x,%08x,%d, %08x, %08x", pIndex, *pIndex, dwMaxIndex, Index, pDatas);
			//MessageBox(NULL, buf, L"Debug_key", MB_OK);
			//Mark = 0x12344321;
			//Mark++;
			//SumOfSame = 0;
			//NumOfSame = 0;
			pBuf1->Datas[Index].data = SumOfSame / NumOfSame;
			pBuf1->Datas[Index].time = lastt;

			NumOfSame = 0;
			SumOfSame = 0;

			SumOfSame += g_convertervalue;
			NumOfSame++;

			


			Index++;
			//send(s, "22222222", 8, 0);
			//LastIndex = 0;
			
			//pBuf1->Index = Index + 1;
			//MessageBox(NULL, L"testtest2222astt != t", L"Debug1", MB_OK);
			lastt = tt;
			//LastIndex = *pIndex;
			

			//Count++;

			//Sleep(1);
			//Mark = 0x99999999;
			//Mark++;
			//MessageBox(NULL, L"lastt != t", L"Debug1", MB_OK);
			//wsprintf(buf, L"3456FIRST %08x,%08x,%d", pIndex, *pIndex, dwMaxIndex);
			//MessageBox(NULL, buf, L"Debug_key", MB_OK);
		}

		
		
		//TCHAR buf[32];
		//wsprintf(buf, L"AFTER %08x,%08x,%d", pIndex, *pIndex, dwMaxIndex);
		//MessageBox(NULL, buf, L"Debug_key", MB_OK);
		
		

		//
		if (Index >= 50)
		{
			//
			send(s, (char *)pBuf1, Index * sizeof(MYDATA), 0);
			//send(s, "12345678", 8, 0);
			//Count = 0;
			Index = 0;
			Sleep(1);
			
			//Sleep(25);
		}

		
		//send(s, "77777777", 8, 0);
		//pDatas[Index].data = g_convertervalue;
		//pDatas[Index].time = timeGetTime();
 
		//RETAILMSG(1,(TEXT("convertervalue = %d\n"), g_convertervalue));
		//Delay(50);

	}
	return 0;
}

//----------------------------------------------------------------------------//
//	AD CONVERTER START																																	//
//----------------------------------------------------------------------------//
void Converter_Start( void )
{
	//TCHAR Buf[64];
	//wsprintf(Buf, L"hEvent is %08x", hAdcEvent);
	//MessageBox(NULL, Buf, L"Debug", MB_OK);
	SetEvent(hAdcEvent);
	//MessageBox(NULL, L"Set Event done", L"Debug", MB_OK);
}

//----------------------------------------------------------------------------//
//	AD CONVERTER STOP																																	//
//----------------------------------------------------------------------------//
void Converter_Stop( void )
{
	ResetEvent(hAdcEvent);
}


//----------------------------------------------------------------------------//
//	ADC initial																																//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD MAD_Init(LPCTSTR pContext, LPCVOID lpvBusContext) { 
//Virtual address mapping

	PHYSICAL_ADDRESS Addr;
	Addr.QuadPart = S3C6410_BASE_REG_PA_ADC;
	v_pADCPregs = (volatile S3C6410_ADC_REG*)/*DrvLib_*/MmMapIoSpace(Addr,
					                                          sizeof(S3C6410_ADC_REG),
															  FALSE);

	v_pADCPregs->ADCDLY = 0x0001;
	v_pADCPregs->ADCCLRINT = 0x1;
	//hAdcEvent = (HANDLE)0x55555555;

	hThread = CreateThread (NULL, 0, AdcThread, NULL, 0, NULL);


	//MessageBox(NULL, L"Init finish!", L"Debug", MB_OK);
	return 1;
} 

//----------------------------------------------------------------------------//
//	ADC deinit																																//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) BOOL MAD_Deinit(DWORD hDeviceContext) { 
	VirtualFree((PVOID)v_pADCPregs, 0, MEM_RELEASE);
	v_pADCPregs = NULL;

	CloseHandle(hAdcEvent);
	CloseHandle(hThread);

	return TRUE; 
} 

//----------------------------------------------------------------------------//
//	ADC open																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD MAD_Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode) { 
	//OutputDebugString(L"ADC - ADC Open\n");
	return 1; 
} 

//----------------------------------------------------------------------------//
//	ADC close																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) BOOL MAD_Close(DWORD hOpenContext) {
	//OutputDebugString(L"ADC - ADC Close\n");
	Converter_Stop();
	return TRUE; 
} 

//----------------------------------------------------------------------------//
//	ADC read																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD MAD_Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count) { 

	/*
	DebugInfo *p = (DebugInfo *)pBuffer;
	p->dwInfo1 = (DWORD)g_convertervalue;
	p->dwInfo2 = (DWORD)pMyBuf;
	p->dwInfo3 = (DWORD)pMyBuf->pDebug2;
	p->dwInfo4 = (DWORD)pDatas;
	*/
	//*(DWORD *)pBuffer = g_convertervalue;
	*(DWORD *)pBuffer = Mark;
	//*(DWORD *)pBuffer = (DWORD)hAdcEvent;
	return 1;
} 

//----------------------------------------------------------------------------//
//	ADC write																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD MAD_Write(DWORD hOpenContext, LPCVOID pBuffer, DWORD Count) { 
	return 1; 
} 

//----------------------------------------------------------------------------//
//	ADC power up																															//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) VOID MAD_PowerUp(DWORD hDeviceContext) {   
} 

//----------------------------------------------------------------------------//
//	ADC down																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) VOID MAD_PowerDown(DWORD hDeviceContext) {   
}  

//----------------------------------------------------------------------------//
//	ADC seek																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD MAD_Seek(DWORD hOpenContext, long Amount, DWORD Type) {   
  return 0;   
} 



//----------------------------------------------------------------------------//
//	ADC IO control																														//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) BOOL MAD_IOControl(DWORD Handle,
									 DWORD dwIoControlCode,
									 PDWORD pInBuf,
									 DWORD nInBufSize,
									 PDWORD pOutBuf,
									 DWORD nOutBufSize,
									 PDWORD pBytesReturned) {  
 switch (dwIoControlCode) {
	  
    case IOCTL_ADC_START:
		/*
		pMyBuf = (MYBUF *)pInBuf;
		pDatas = (MYDATA *)pMyBuf->pBuf;
		pIndex = pMyBuf->pIndex;
		dwMaxIndex = pMyBuf->dwMaxIndex;
		*(pMyBuf->pMark) = 0x22222222;
		*/
		/*
		TCHAR buf[32];
		wsprintf(buf, L"%08x", pIndex);
		*(pMyBuf->pMark) = 0x12345678;
		MessageBox(NULL, buf, L"pIndex is", MB_OK);
		wsprintf(buf, L"%d", *pIndex);
		MessageBox(NULL, buf, L"*pIndex is", MB_OK);
		wsprintf(buf, L"%d", dwMaxIndex);
		MessageBox(NULL, buf, L"dwMaxIndex is", MB_OK);
		*/
		//pMyBuf1 = (MYBUF1 *)pInBuf;
		Converter_Start();
		//Sleep(3000);

		//wsprintf(buf, L"%d", *pIndex);
		//MessageBox(NULL, buf, L"*pIndex now is", MB_OK);
		break;
    case IOCTL_ADC_STOP:
		Converter_Stop();
		//*(pMyBuf->pMark) = 0x87654321;

		break;
	default:
		break;
	}

  return TRUE;   
}  

