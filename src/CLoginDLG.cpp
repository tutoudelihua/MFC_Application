// CLoginDLG.cpp: 实现文件
//

#include "pch.h"
#include "MFC_Application.h"
#include "afxdialogex.h"
#include "CLoginDLG.h"
#include "CInfoFile.h"
#include <dwmapi.h>

CInfoFile cif;
CBrush m_brush;

// CLoginDLG 对话框

IMPLEMENT_DYNAMIC(CLoginDLG, CDialogEx)

BOOL CLoginDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	// 绘制透明背景
	memDC.FillSolidRect(rect, RGB(0, 0, 0));  // 设置透明颜色
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	return TRUE;
}

HBRUSH CLoginDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetTextColor(RGB(255, 255, 255));//设置字体颜色 
	}
	if (nCtlColor == CTLCOLOR_EDIT) {
		pDC->SetTextColor(RGB(0, 0, 0)); // 字体颜色
		return m_brush; // 返回自定义画刷
	}
	
	// 处理子控件透明背景
	pDC->SetBkMode(TRANSPARENT);
	return HBRUSH(GetStockObject(NULL_BRUSH));
}


BOOL CLoginDLG::OnInitDialog()
{

	cif.InitLoad();

	CDialogEx::OnInitDialog();
	
	// 设置分层窗口属性（整体透明度）
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 185, LWA_ALPHA);  // 150为透明度（0-255）
	UpdateData(FALSE); //内容更新到对应的控件

	return TRUE;
}

CLoginDLG::CLoginDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_user(_T(""))
	, m_pwd(_T(""))
{

}

CLoginDLG::~CLoginDLG()
{
}

void CLoginDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_pwd);
}


BEGIN_MESSAGE_MAP(CLoginDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDLG::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLoginDLG 消息处理程序

void CLoginDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //更新控件的数据到对应的变量

	if (!cif.CheckLoad(m_user, m_pwd)) {
		MessageBox(_T("用户名或密码错误！"));
		m_user.Empty(); //清空
		m_pwd.Empty();
		return;
	}

	else CDialogEx::OnOK();

	this->SendMessage(WM_DESTROY);
	this->SendMessage(WM_CLOSE);
	return;
}

void CLoginDLG::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	exit(0);
	return;
}

void CLoginDLG::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); //更新控件的数据到对应的变量

	cif.CreateLoad(m_user, m_pwd);
	MessageBox(_T("注册成功！"));
	m_user.Empty(); //清空
	m_pwd.Empty();

	//CDialogEx::OnOK();
}

void CLoginDLG::OnOK()
{
	CDialogEx::OnOK();
}
