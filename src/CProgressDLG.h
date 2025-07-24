#pragma once
#include "afxdialogex.h"


// CProgressDLG 对话框

class CProgressDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDLG)

public:
	CProgressDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProgressDLG();

	const void LoadtmpData();

	const void InitConf();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Progress };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl MyProg;
	void OnTimer(UINT_PTR nIDEvent);
	void OnCancel();
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnPaint();
//	afx_msg void OnPaint();
};
