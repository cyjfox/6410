// Scanner.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Scanner.h"
#include "ScannerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScannerApp

BEGIN_MESSAGE_MAP(CScannerApp, CWinApp)
END_MESSAGE_MAP()

void CScannerApp::MyGetLastError()
{
	WCHAR buf[128];
	int err = GetLastError();
	wsprintf(buf, L"last error code is %08X !!!", err);
	AfxMessageBox(buf);

}
// CScannerApp ����
CScannerApp::CScannerApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CScannerApp ����
CScannerApp theApp;

// CScannerApp ��ʼ��

BOOL CScannerApp::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	handle = NULL;

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CScannerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
