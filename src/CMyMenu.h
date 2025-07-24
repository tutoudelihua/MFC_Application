#pragma once
#include "afxdialogex.h"
#include "CConf.h"
#include "CCoeff.h"
#include <vector>

#ifndef _BJ_PATH
#define _BJ_PATH "data\\HCNO\\BJ.txt"
#endif

// CMyMenu 对话框

class CMyMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CMyMenu)

public:
	CMyMenu(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyMenu();
	void SetCoeff(CCoeff Coeffhc, CCoeff Coeffno);
	CMenu m_Menu;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Menu };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeMfceditbrowse1();
	void CMyMenu::OnCancel();
	CComboBox Combox;
	afx_msg void On32773();
	afx_msg void On32783();
	afx_msg void OnHc32784();
	afx_msg void OnHc32785();
	afx_msg void OnHc32786();
	afx_msg void OnHc32787();
	afx_msg void OnHc32788();
	afx_msg void OnNo32789();
	afx_msg void OnNo32790();
	afx_msg void OnNo32791();
	afx_msg void OnNo32792();
	afx_msg void OnNo32793();
	afx_msg void OnNo32794();
	afx_msg void OnNo32795();
	afx_msg void OnNo32796();
	afx_msg void OnNo32797();
};
