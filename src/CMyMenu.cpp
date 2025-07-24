// CMyMenu.cpp: 实现文件
//

#include "pch.h"
#include "MFC_Application.h"
#include "afxdialogex.h"
#include "CMyMenu.h"

CString strCBText;
CString mFilePath;
Conf mConf;
Conf BJConf;

// CMyMenu 对话框

IMPLEMENT_DYNAMIC(CMyMenu, CDialogEx)

CMyMenu::CMyMenu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Menu, pParent)
{

}

CMyMenu::~CMyMenu()
{
}

CCoeff CoeffHC, CoeffNO;
void CMyMenu::SetCoeff(CCoeff Coeffhc, CCoeff Coeffno) {
	CoeffHC = Coeffhc;
	CoeffNO = Coeffno;
}

BOOL CMyMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Menu.LoadMenu(IDR_MENU);
	SetMenu(&m_Menu);

	CRect r;
	Combox.GetDroppedControlRect(&r);
	Combox.GetParent()->ScreenToClient(&r);
	r.bottom = r.top + 150;
	Combox.MoveWindow(&r);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CMyMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, Combox);
}


BEGIN_MESSAGE_MAP(CMyMenu, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMyMenu::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CMyMenu::OnEnChangeMfceditbrowse1)
	ON_WM_SIZING()
	ON_COMMAND(ID_32773, &CMyMenu::On32773)
	ON_COMMAND(ID_32783, &CMyMenu::On32783)
	ON_COMMAND(ID_HC32784, &CMyMenu::OnHc32784)
	ON_COMMAND(ID_HC32785, &CMyMenu::OnHc32785)
	ON_COMMAND(ID_HC32786, &CMyMenu::OnHc32786)
	ON_COMMAND(ID_HC32787, &CMyMenu::OnHc32787)
	ON_COMMAND(ID_HC32788, &CMyMenu::OnHc32788)
	ON_COMMAND(ID_NO32789, &CMyMenu::OnNo32789)
	ON_COMMAND(ID_NO32790, &CMyMenu::OnNo32790)
	ON_COMMAND(ID_NO32791, &CMyMenu::OnNo32791)
	ON_COMMAND(ID_NO32792, &CMyMenu::OnNo32792)
	ON_COMMAND(ID_NO32793, &CMyMenu::OnNo32793)
	ON_COMMAND(ID_NO32794, &CMyMenu::OnNo32794)
	ON_COMMAND(ID_NO32795, &CMyMenu::OnNo32795)
	ON_COMMAND(ID_NO32796, &CMyMenu::OnNo32796)
	ON_COMMAND(ID_NO32797, &CMyMenu::OnNo32797)
END_MESSAGE_MAP()


// CMyMenu 消息处理程序

void CMyMenu::OnCbnSelchangeCombo1() 
{
	GetDlgItemText(IDC_COMBO1, strCBText);
	mConf.setType(std::string(strCBText.GetBuffer()));
	//MessageBox(strCBText);
}

double record = 0, tmp = 0, result = 0;
int k = 0;

std::vector<double> coeff;
std::vector<double> MyData, BjData;

extern double Buffer[65536];

void CMyMenu::OnEnChangeMfceditbrowse1() 
{
	GetDlgItemText(IDC_MFCEDITBROWSE1, mFilePath);

	if (mConf.getType() == "HC") coeff = CoeffHC.getCoeff();
	else if (mConf.getType() == "NO") coeff = CoeffNO.getCoeff();

	MyData = mConf.LoadConf(mFilePath);
	BjData = BJConf.LoadConf(_BJ_PATH);
	
	Buffer[0] = 0;
	
	for (int i = 0; i < MyData.size(); ++i) {//计算读入气体数据吸光度
		tmp = BjData[i] / MyData[i];
		tmp = log(tmp);
		Buffer[++k] = tmp;
	}
	for (int i = 140; i <= 180; ++i) {
		record += Buffer[i];
	}
	tmp = 0;
	for (int i = 300; i <= 320; ++i) {
		tmp += Buffer[i];
	}
	tmp /= 20;
	record -= tmp;

	for (int op = 0; op < 2; ++op) {
		result += coeff[op] * pow(record, op);
	}
	//输出结果
	CStringA strAnsi("气体浓度为：");
	strAnsi += CStringA(std::to_string(result).c_str());
	MessageBox(CString(strAnsi));
}

void CMyMenu::OnCancel()
{
	AfxGetApp()->ExitInstance();
	CDialogEx::OnCancel();
}
void CMyMenu::On32773()
{

	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::On32783()
{
	//打开指定路径照片
	const char* path = "data\\BJ_0.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	
	
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnHc32784()
{
	//打开指定路径照片
	const char* path = "data\\HC_10.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
}

void CMyMenu::OnHc32785()
{
	//打开指定路径照片
	const char* path = "data\\HC_41.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnHc32786()
{
	//打开指定路径照片
	const char* path = "data\\HC_102.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnHc32787()
{
	//打开指定路径照片
	const char* path = "data\\HC_160.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnHc32788()
{
	//打开指定路径照片
	const char* path = "data\\HC_196.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32789()
{
	//打开指定路径照片
	const char* path = "data\\NO_10.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32790()
{
	//打开指定路径照片
	const char* path = "data\\NO_50.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32791()
{
	//打开指定路径照片
	const char* path = "data\\NO_200.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32792()
{
	//打开指定路径照片
	const char* path = "data\\NO_500.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32793()
{
	//打开指定路径照片
	const char* path = "data\\NO_1006.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32794()
{
	//打开指定路径照片
	const char* path = "data\\NO_1511.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32795()
{
	//打开指定路径照片
	const char* path = "data\\NO_2020.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32796()
{
	//打开指定路径照片
	const char* path = "data\\NO_2497.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

void CMyMenu::OnNo32797()
{
	//打开指定路径照片
	const char* path = "data\\NO_2954.jpg";
	ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}
