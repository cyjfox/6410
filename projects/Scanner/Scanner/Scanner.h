// Scanner.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CScannerApp:
// �йش����ʵ�֣������ Scanner.cpp
//

class CScannerApp : public CWinApp
{
private:
	HANDLE handle;
public:
	CScannerApp();
	static void MyGetLastError();
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CScannerApp theApp;
