// ADC_AppDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ADC_App.h"
#include "ADC_AppDlg.h"
//#include <windows.h>
#include <ceddk.h>
#include <winsock2.h>

#pragma comment(lib, "Mmtimer.lib")
#pragma comment(lib, "ws2.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	MYDATA Datas[100000];
	DWORD dwMaxIndex;
	DWORD Index;
	DWORD dwDebug;
};


//全局变量
HANDLE hAdc;
HANDLE hThread;
HANDLE hThread1;
HANDLE hAdcEvent;
HANDLE hMapFile;
DWORD WINAPI AdcThread(PVOID);
DWORD WINAPI SendData(PVOID pParam);
BOOL WrittingToFile;
HANDLE hLogFile;
PVOID pData;
//DWORD index;
DWORD SendedIndex;
MYDATA *presult;
LARGE_INTEGER freq;

DWORD RuningState;

SOCKET s;
MYBUF1 *pMyBuf1;
//MYBUF1 MyBuf1;
//MYBUF MyBuf;
DWORD dwIndex;
DWORD Mark;
BOOL bInitilized;

DWORD Debug1;
DWORD Debug2;
DWORD Debug3;
DWORD Debug4;
LPVOID pMapMem;

// constants define
#define IOCTL_ADC_START \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 60, METHOD_BUFFERED, FILE_ANY_ACCESS) //60???
#define IOCTL_ADC_STOP \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)

// CADC_AppDlg 对话框

CADC_AppDlg::CADC_AppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADC_AppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADC_AppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CADC_AppDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CADC_AppDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CADC_AppDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CADC_AppDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CADC_AppDlg 消息处理程序

BOOL CADC_AppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	bInitilized = FALSE;
	hAdcEvent = NULL;

	WSADATA WSAData;
	
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	if (s == INVALID_SOCKET)
	{
		AfxMessageBox(L"Create Socket failed!");
		return FALSE;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(30000);
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.2");
	int r;
	r = bind(s, (sockaddr *)&addr, sizeof(sockaddr));
	if (r == SOCKET_ERROR)
	{
		AfxMessageBox(L"bind failed!");
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
		AfxMessageBox(buf);
		AfxMessageBox(L"connect failed!");
		return FALSE;
	}

	/*
	WCHAR *pBuf;
	pBuf = L"1111abcdcyjfox,hello,world";
	send(s, (char *)pBuf, wcslen(pBuf) * sizeof(WCHAR), 0);
	
	AfxMessageBox(L"send done!");
	*/

	// TODO: 在此添加额外的初始化代码
	hAdc = CreateFile(TEXT("MAD1:"),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if (hAdc == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL, L"Open adc failed!", L"Debug", MB_OK);
		printf ("can't open device\r\n");
		return FALSE;
	}
	else
	{
		::MessageBox(NULL, L"Open adc done!", L"Debug", MB_OK);
		printf ("Open ADC1\r\n");
	}	

	

	QueryPerformanceFrequency(&freq);

	SendedIndex = 0;

	RuningState = 1;

	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(MYBUF1), L"ADC_Data_Buffer");
	if (hMapFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"Create file mapping failed!");
		return TRUE;
	}
	
	pMapMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (pMapMem == NULL)
	{
		AfxMessageBox(L"MapViewOfFile failed!");
	}
	pMyBuf1 = (MYBUF1 *)pMapMem;
	pMyBuf1->dwMaxIndex = 99999;
	pMyBuf1->Index = 0;
	hThread = CreateThread (NULL, 0, AdcThread, NULL, 0, NULL);
	hThread1 = CreateThread(NULL, 0, SendData, NULL, 0, NULL);

	

	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CADC_AppDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_ADC_APP_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_ADC_APP_DIALOG));
	}
}
#endif

DWORD WINAPI SendData(PVOID pParam)
{
	MYDATA *pBuf;
	//pBuf = new MYDATA[100000];
	while (hAdcEvent == NULL);
	WaitForSingleObject(hAdcEvent, INFINITE);
	pBuf = (MYDATA *)pMyBuf1->Datas;
	if (pBuf == NULL)
	{
		AfxMessageBox(L"pBuf is an illegal pointer(pBuf in SendData)");
		return 0;
	}

	DWORD CurIndex;
	//Sleep(1000);
	CurIndex = pMyBuf1->Index;
	
	//CString str;

	
	//str.Format(L"In APP:(Note Here)pBuf is %08x,pointer to dwIndex is %08x,dwIndex is %d,value of mark is %08x", pBuf, &dwIndex, dwIndex, *(MyBuf.pMark));
	//AfxMessageBox(str);
	/*
	str.Format(L"CurIndex is %d,SendedIndex is %d,dwIndex is %d", CurIndex, SendedIndex, dwIndex);
	AfxMessageBox(str);
	*/
	//AfxMessageBox(L"About to send data");
	while (RuningState)
	{
	if (SendedIndex < CurIndex)//如果有数据要发送
	{
		//AfxMessageBox(L"Data ready to send");
		DWORD BytesToSend = sizeof(MYDATA) * (CurIndex - SendedIndex);
		//memcpy(pBuf, (void *)(presult + SendedIndex), BytesToSend);
		send(s, (char *)(pBuf + SendedIndex), BytesToSend, 0);
		SendedIndex = CurIndex;
		//AfxMessageBox(L"Data send");
	}

	DebugInfo Info;
	DWORD hh;
	DWORD nRead;
	//ReadFile(hAdc, (LPVOID)&Info, sizeof(DebugInfo), &nRead, NULL);
	//str.Format(L"ReadFile result g_convertervalue is %08x\r\npMyBuf is %08x\r\npDebug1 is %08x\r\n*pDebug2(Debug2) is %08x\r\npDatas is %08x", Info.dwInfo1, Info.dwInfo2, Info.dwInfo3, (*((DWORD *)Info.dwInfo3)), Info.dwInfo4);
	//AfxMessageBox(str);
	CurIndex = pMyBuf1->Index;
	//AfxMessageBox(L"Ready to test");
	/*
	if ((*(MyBuf.pMark)) != 0xffffffff)
	{
		str.Format(L"Mark(*pIndex) now is %08x, dwIndex is %d", *(MyBuf.pMark), dwIndex);
		AfxMessageBox(str);
		Mark = 0xffffffff;
	}

	if (Debug1 != 0xffffffff)
	{
		AfxMessageBox(L"Debug1 changed!");
	}
	if (Debug1 == 0xcccccccc)
	{
		AfxMessageBox(L"Waitting for event!");
		Debug1 = 0xffffffff;
	}
	if (Debug1 == 1)
	{
		AfxMessageBox(L"Event set!");
		Debug1 = 0xffffffff;
	}
	if (Debug2 == 1)
	{
		AfxMessageBox(L"Enable_start is low");
		Debug2 == 0xffffffff;
	}
	if (Debug3 == 1)
	{
		AfxMessageBox(L"Convertion done!");
		Debug3 = 0xffffffff;
	}
	if (Debug4 == 1)
	{
		AfxMessageBox(L"Result saved!");
		Debug4 = 0xffffffff;
	}
	*/
	//DWORD mark;
	//DWORD nRead;
	//ReadFile(hAdc, (LPVOID)&mark, sizeof(DWORD), &nRead, NULL); 
	//CString str;
	//str.Format(L"CurIndex is %d,SendedIndex is %d,pMyBuf1->Index is %d,dwDebug is %08x", CurIndex, SendedIndex, pMyBuf1->Index, pMyBuf1->dwDebug);
	//AfxMessageBox(str);
	Sleep(50);
	}
	return 0;
}

DWORD WINAPI WriteData(PVOID pParam)
{
	WrittingToFile = TRUE;

	::MessageBox(NULL, L"Begin to write data", L"Debug", MB_OK);
	if (hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD nWritten;
		WriteFile(hLogFile, pData, sizeof(MYDATA) * 100000, &nWritten, NULL);
	}

	WrittingToFile = FALSE;
	::MessageBox(NULL, L"Data written", L"Debug", MB_OK);
	return WrittingToFile;
}

//----------------------------------------------------------------------------//
//	AD Converter Thread																																	//
//----------------------------------------------------------------------------//
DWORD WINAPI AdcThread (PVOID hdcMain)
{	
	DWORD dwBytesRead;
	//DWORD convalue;
	DWORD convalue;
	//max conversion rate is 1MSPS,so the maximum size needed for a second data is 2MB,double it,we choose 4mB
	//MYDATA *result1;
	//result1 = new MYDATA[100000];//4MB//10万个数据
	//if (result1 == NULL)
	//{
	//	::MessageBox(NULL, L"memory allocation failed!(result1)", L"Debug", MB_OK);
	//	return 0;
	//}
	//MYDATA *result2;//4MB
	//result2 = new MYDATA[100000];//4MB
	//if (result2 == NULL)
	//{
	//	::MessageBox(NULL, L"memory allocation failed!(result2)", L"Debug", MB_OK);
	//	return 0;
	//}

	//memset(result1, 0x33, sizeof(MYDATA) * 100000);
	//memset(result2, 0x33, sizeof(MYDATA) * 100000);

	/*
	//MyBuf.dwMaxIndex = 99999;
	//MyBuf.pBuf = (PVOID)result1;
	dwIndex = 0;
	MyBuf.pIndex = &dwIndex;
	Mark = 0xffffffff;
	Debug1 = 0xffffffff;
	Debug2 = 0xcccccccc;
	Debug3 = 0xffffffff;
	Debug4 = 0xffffffff;
	MyBuf.pDebug1 = &Debug1;
	MyBuf.pDebug2 = &Debug2;
	MyBuf.pDebug3 = &Debug3;
	MyBuf.pDebug4 = &Debug4;
	MyBuf.pMark = &Mark;
	
	MyBuf1.dwMaxIndex = 99999;
	MyBuf1.Index = 0;
	*/
	char tempvalue[3];
	CString str;
	CADC_AppDlg *pdlg =(CADC_AppDlg*)(AfxGetApp()->m_pMainWnd);



	hAdcEvent = CreateEvent(NULL, TRUE, FALSE, _T("TE6410_ADC_read"));
	
	//index = 0;
	
	RuningState = 1;
	
	//presult = result1;
	//::MessageBox(NULL, L"Ready to convert", L"Debug", MB_OK);
	DWORD lastt;
	lastt = 0;
	DWORD NumOfSame;
	DWORD SumOfSame;
	NumOfSame = 0;
	SumOfSame = 0;
	//AfxMessageBox(L"Initilized!");
	//while(1)
	{
		//Sleep(99999);
	/*	WaitForSingleObject(hAdcEvent, INFINITE);
		ReadFile(hAdc, &convalue, sizeof(DWORD), &dwBytesRead, NULL);
		//presult[index].data = convalue;
		DWORD tt;
		tt = timeGetTime();
		if (lastt == tt)//如果仍是只是相同的时间戳，则对所有相同时间戳的数据求平均数，以后可改用更高分辨率的时间戳来提高精度
		{
			NumOfSame++;
			SumOfSame += convalue;
			presult[index].data = (double)SumOfSame / (double)NumOfSame;
			presult[index].time = tt;
		}
		else
		{
			presult[index].data = convalue;
			presult[index].time = tt;
			index++;
			lastt = tt;
			NumOfSame = 0;
			SumOfSame = 0;
		}

		if (index > 100000)//if full
		{
			if (WrittingToFile == TRUE) MessageBox(NULL, L"Data lost!", L"Error", MB_OK);
			//switch buffer
			if (presult == result1)
			{
				presult = result2;
				pData = result1;
			}
			if (presult == result2)
			{
				presult = result1;
				pData = result2;
			}

			index = 0;
			CreateThread(NULL, 0, WriteData, NULL, 0, NULL);
		}
		
		//str.Format(_T("%d"),convalue);

		//pdlg->SetDlgItemTextW(IDC_EDIT1,str);

		//printf("convalue = %d !!!!\n", convalue);
		//Sleep(2);
		*/

	}
	return 0;
}

void CADC_AppDlg::OnBnClickedButton2()
{
	CloseHandle(hAdcEvent);
	CloseHandle(hThread);
	CloseHandle(hAdc);

	
	//CString str;
	//str.Format(L"mark now is %08x,dwIndex is %d", *(MyBuf.pMark), dwIndex);
	//AfxMessageBox(str);

	//保存未保存的数据
	if ((pMyBuf1->Index > 0) && (pMyBuf1->Index <= 100000))
	{
		DWORD nWritten;
		WriteFile(hLogFile, pMyBuf1->Datas, sizeof(MYDATA) * pMyBuf1->Index, &nWritten, NULL);
	}
	CloseHandle(hLogFile);
	EndDialog(IDOK);   
}

void CADC_AppDlg::OnBnClickedButton4()
{

	pData = NULL;
	hLogFile = INVALID_HANDLE_VALUE;

	SYSTEMTIME Time;
	GetLocalTime(&Time);


	WCHAR FileName[1024];
	wsprintf(FileName, L"\\Storage Card\\Data_%d_%d_%d_%d_%d_%d.log", Time.wYear, Time.wMonth, Time.wDay, Time.wHour, Time.wMinute, Time.wSecond);
	hLogFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hLogFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"Create log file failed!", L"Error", MB_OK);
		return;
	}

	WrittingToFile = FALSE;

	//TCHAR Buf[256];
	//wsprintf(Buf, L"Note:pBuf is %08x,*pIndex is %d", MyBuf.pBuf, *(MyBuf.pIndex));
	//::MessageBox(NULL, Buf, L"App_Debug", MB_OK);
	DeviceIoControl (hAdc, IOCTL_ADC_START, 
							 NULL, 0, 
							 NULL, 0, NULL, NULL);


	SetEvent(hAdcEvent);


}

void CADC_AppDlg::OnBnClickedButton5()
{
	ResetEvent(hAdcEvent);
	DeviceIoControl (hAdc, IOCTL_ADC_STOP, 
							 NULL, 0, 
							 NULL, 0, NULL, NULL);
	//保存未保存的数据
	if ((pMyBuf1->Index > 0) && (pMyBuf1->Index <= 100000))
	{
		DWORD nWritten;
		WriteFile(hLogFile, pMyBuf1->Datas, sizeof(MYDATA) * pMyBuf1->Index, &nWritten, NULL);
	}

	CloseHandle(hLogFile);

	RuningState = 0;
}
