// DAC_Driver.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "DAC_Driver.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, L"DLL_PROCESS_ATTACH", L"dll loading...", MB_OK);
		break;
	case DLL_THREAD_ATTACH:
		MessageBox(NULL, L"DLL_THREAD_ATTACH", L"dll loading...", MB_OK);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL, L"DLL_THREAD_DETACH", L"dll loading...", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL, L"DLL_PROCESS_DETACH", L"dll loading...", MB_OK);
		break;
	}
    return TRUE;
}

// 这是导出变量的一个示例
DAC_DRIVER_API int nDAC_Driver=0;

// 这是导出函数的一个示例。
DAC_DRIVER_API int fnDAC_Driver(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 DAC_Driver.h
CDAC_Driver::CDAC_Driver()
{ 
	return; 
}


extern "C" __declspec(dllexport) DWORD DAC_Init(LPCTSTR pContext, LPCVOID lpvBusContext) { 
	MessageBox(NULL, L"DAC_Init!!!", L"DAC_Init...", MB_OK);
	//Virtual address mapping

	
	//PHYSICAL_ADDRESS Addr;
	//Addr.QuadPart = S3C6410_BASE_REG_PA_ADC;
	//v_pADCPregs = (volatile S3C6410_ADC_REG*)/*DrvLib_*/MmMapIoSpace(Addr,
					                                          //sizeof(S3C6410_ADC_REG),
															  //FALSE);

	//v_pADCPregs->ADCDLY = 0x0001;
	//v_pADCPregs->ADCCLRINT = 0x1;
	//hAdcEvent = (HANDLE)0x55555555;

	//hThread = CreateThread (NULL, 0, AdcThread, NULL, 0, NULL);


	//MessageBox(NULL, L"Init finish!", L"Debug", MB_OK);
	return TRUE;
} 

//----------------------------------------------------------------------------//
//	ADC deinit																																//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) BOOL DAC_Deinit(DWORD hDeviceContext) {
	/*
	VirtualFree((PVOID)v_pADCPregs, 0, MEM_RELEASE);
	v_pADCPregs = NULL;

	CloseHandle(hAdcEvent);
	CloseHandle(hThread);
	*/
	return TRUE; 
} 

//----------------------------------------------------------------------------//
//	ADC open																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD DAC_Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode) { 
	MessageBox(NULL, L"DAC_Open!!!", L"DAC_Open...", MB_OK);
	//OutputDebugString(L"ADC - ADC Open\n");
	return TRUE; 
} 

//----------------------------------------------------------------------------//
//	ADC close																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) BOOL DAC_Close(DWORD hOpenContext) {
	//OutputDebugString(L"ADC - ADC Close\n");
	//Converter_Stop();
	return TRUE; 
} 

//----------------------------------------------------------------------------//
//	ADC read																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD DAC_Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count) { 

	/*
	DebugInfo *p = (DebugInfo *)pBuffer;
	p->dwInfo1 = (DWORD)g_convertervalue;
	p->dwInfo2 = (DWORD)pMyBuf;
	p->dwInfo3 = (DWORD)pMyBuf->pDebug2;
	p->dwInfo4 = (DWORD)pDatas;
	*/
	//*(DWORD *)pBuffer = g_convertervalue;
	//*(DWORD *)pBuffer = Mark;
	//*(DWORD *)pBuffer = (DWORD)hAdcEvent;
	return TRUE;
} 

//----------------------------------------------------------------------------//
//	ADC write																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD DAC_Write(DWORD hOpenContext, LPCVOID pBuffer, DWORD Count) { 
	return TRUE; 
} 

//----------------------------------------------------------------------------//
//	ADC power up																															//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) VOID DAC_PowerUp(DWORD hDeviceContext) {   
} 

//----------------------------------------------------------------------------//
//	ADC down																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) VOID DAC_PowerDown(DWORD hDeviceContext) {   
}  

//----------------------------------------------------------------------------//
//	ADC seek																																	//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) DWORD DAC_Seek(DWORD hOpenContext, long Amount, DWORD Type) {   
  return FALSE;   
} 



//----------------------------------------------------------------------------//
//	ADC IO control																														//
//----------------------------------------------------------------------------//
extern "C" __declspec(dllexport) BOOL DAC_IOControl(DWORD Handle,
									 DWORD dwIoControlCode,
									 PDWORD pInBuf,
									 DWORD nInBufSize,
									 PDWORD pOutBuf,
									 DWORD nOutBufSize,
									 PDWORD pBytesReturned) {  
										 WCHAR buf[128];
 switch (dwIoControlCode) {
	  
    case IOCTL_DAC_START:
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
		//Converter_Start();
		//Sleep(3000);

		wsprintf(buf, L"%08X", Handle);
		MessageBox(NULL, buf, L"Starting...", MB_OK);
		break;
    case IOCTL_DAC_STOP:
		wsprintf(buf, L"%08X", Handle);
		MessageBox(NULL, buf, L"Stoping...", MB_OK);
		//Converter_Stop();
		//*(pMyBuf->pMark) = 0x87654321;

		break;
	default:
		break;
	}

  return TRUE;   
}  