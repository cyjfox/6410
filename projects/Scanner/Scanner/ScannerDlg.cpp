// ScannerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Scanner.h"
#include "ScannerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE handle = NULL;
// CScannerDlg 对话框

CScannerDlg::CScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScannerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScannerDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CScannerDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_COPYDLL, &CScannerDlg::OnBnClickedButtonCopydll)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_REG, &CScannerDlg::OnBnClickedButtonWriteReg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DRIVER, &CScannerDlg::OnBnClickedButtonLoadDriver)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD_DRIVER, &CScannerDlg::OnBnClickedButtonUnloadDriver)
END_MESSAGE_MAP()


// CScannerDlg 消息处理程序

BOOL CScannerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CScannerDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_SCANNER_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_SCANNER_DIALOG));
	}
}
#endif


void CScannerDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(L"Hello,Scanner!");
	HANDLE handle1 = ActivateDeviceEx(L"Drivers\\BuiltIn\\DAC", NULL, 0, NULL);
	if (handle1 == NULL) {
		AfxMessageBox(L"ActivateDeviceEx returned NULL!");
	} else {
		WCHAR buf[128];
		wsprintf(buf, L"handle is 0x%08X!", handle1);
		AfxMessageBox(buf);
		DeactivateDevice(handle1);
	}
}

void CScannerDlg::OnBnClickedButtonCopydll()
{
	// TODO: 在此添加控件通知处理程序代码
	CString dllName = L"DAC_Driver.dll";
	CString dllNewName = L"DAC_Driver_Copy.dll";
	WCHAR buf[512];
	GetModuleFileName(NULL, buf, 512);
	/*
	WCHAR buf1[1024];
	wsprintf(buf1, L"current directory is : %s", buf),
	AfxMessageBox(buf1);
	*/
	CString fullPath = buf;
	CString directoryPath;
	int lastAntiSlash = fullPath.ReverseFind(L'\\');
	if (lastAntiSlash >= 0) {
		directoryPath = fullPath.Left(lastAntiSlash);
	}
	//AfxMessageBox(directoryPath);
	CString dllPath = directoryPath + L"\\" + dllName;
	CString dllNewPath = L"\\Windows\\" + dllName;
	//AfxMessageBox(dllPath);
	//AfxMessageBox(dllNewPath);
	if (!CopyFile(dllPath, dllNewPath, FALSE)) {
		CScannerApp::MyGetLastError();
		AfxMessageBox(L"copy DAC_Driver.dll to windows folder failed!!!");
	} else {
		this->SetDlgItemTextW(IDC_STATIC_INFO, L"copy DAC_Driver.dll to windows folder done!");
	}
}

void CScannerDlg::OnBnClickedButtonWriteReg()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD GdwDrvOrder = 0;
	DWORD GdwDrvIndex = 1;
	HKEY hDrvkey = NULL;
	DWORD dwTmp = 0;
    LONG nErr = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Drivers\\BuiltIn\\DAC", 0, L"", 0, 0, NULL, &hDrvkey, &dwTmp);
    if (nErr != ERROR_SUCCESS) {
        //AfxMessageBox(L"创建驱动注册表子键失败\r\n");
		CScannerApp::MyGetLastError();
		AfxMessageBox(L"create reg key failed!!!");
    }
    else {
        RegSetValueEx(hDrvkey,L"Prefix",0,REG_SZ,(LPBYTE)L"DAC",(wcslen(L"DAC")+1)*sizeof(WCHAR));
        RegSetValueEx(hDrvkey,L"Dll",0,REG_SZ,(LPBYTE)L"DAC_Driver.dll",(wcslen(L"DAC_Driver.dll")+1)*sizeof(WCHAR));
        RegSetValueEx(hDrvkey,L"FriendlyName",0,REG_SZ,(LPBYTE)L"DAC Driver",(wcslen(L"DAC Driver")+1)*sizeof(WCHAR));
        RegSetValueEx(hDrvkey,L"Order",0,REG_DWORD,(LPBYTE)&GdwDrvOrder, sizeof(DWORD));
        RegSetValueEx(hDrvkey,L"Index",0,REG_DWORD,(LPBYTE)&GdwDrvIndex, sizeof(DWORD));
		this->SetDlgItemTextW(IDC_STATIC_INFO, L"write to reg done!");
    }
	/*
    m_hDriver = ActivateDeviceEx(L"Drivers\\BuiltIn\\MFL",NULL,0,NULL); 
	// 加载驱动
    if (m_hDriver == INVALID_HANDLE_VALUE) {
        UpdataMsgShow(_T("加载驱动失败\r\n"));
        return false;
    }
    m_hFile = CreateFile(TEXT("MFL1:"),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
    if (m_hFile == INVALID_HANDLE_VALUE) {
        UpdataMsgShow(_T("打开驱动失败\r\n"));
        nErr = GetLastError();
        return false;
    }
	*/


}

void LoadDriver() {
	handle = ActivateDeviceEx(L"Drivers\\BuiltIn\\DAC", NULL, 0, NULL);
	if (handle == NULL) {
		AfxMessageBox(L"ActivateDeviceEx returned NULL!");
	} else {
		WCHAR buf[128];
		wsprintf(buf, L"handle is 0x%08X!", handle);
		AfxMessageBox(buf);
	}
}

void CScannerDlg::OnBnClickedButtonLoadDriver()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadDriver();
	this->SetDlgItemTextW(IDC_STATIC_INFO, L"load driver done!");

}


void unloadDriver() {
	if (handle != NULL) {
		DeactivateDevice(handle);
		handle = NULL;
	}
}
void CScannerDlg::OnBnClickedButtonUnloadDriver()
{
	// TODO: 在此添加控件通知处理程序代码
	unloadDriver();
	this->SetDlgItemTextW(IDC_STATIC_INFO, L"unload driver done!");
}
