// ScannerDlg.h : 头文件
//

#pragma once

// CScannerDlg 对话框
class CScannerDlg : public CDialog
{
// 构造
public:
	CScannerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCANNER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonCopydll();
	afx_msg void OnBnClickedButtonWriteReg();
	afx_msg void OnBnClickedButtonLoadDriver();
	afx_msg void OnBnClickedButtonUnloadDriver();
};
