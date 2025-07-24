#pragma once
#include "afxdialogex.h"


// CLoginDLG 对话框

class CLoginDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDLG)

public:
	CLoginDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLoginDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_user;
	CString m_pwd;
	CFont m_editFont;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	virtual void OnOK();
};
