// ScannerDlg.h : ͷ�ļ�
//

#pragma once

// CScannerDlg �Ի���
class CScannerDlg : public CDialog
{
// ����
public:
	CScannerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCANNER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
