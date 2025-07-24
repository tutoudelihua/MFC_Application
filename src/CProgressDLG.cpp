// CProgressDLG.cpp: 实现文件
//

#include "pch.h"
#include "MFC_Application.h"
#include "afxdialogex.h"
#include "CProgressDLG.h"
#include "CConf.h"
#include "CMyMenu.h"
#include "CurveFitting.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <windows.h>


std::vector<Conf> dataList;
std::unordered_map<std::string, std::vector<std::pair<int, std::vector<double>>>> typeMap;
std::vector<std::pair<int, double>> tmpDataHC, tmpDataNO;
std::vector<double> coeffHC, coeffNO;

bool flag = true; // 是否加载原始数据图像

double Buffer[65536];

const void CProgressDLG::LoadtmpData() {
	for (Conf& conf : dataList) {
		typeMap[conf.getType()].push_back(std::make_pair(conf.getCodeNumber(), conf.getData()));
	}
	dataList.clear();
	double record = 0;
	int k = 0;
	for (auto& pair : typeMap["HC"]) {
		k = 0, record = 0, Buffer[0] = 0;
		for (int i = 0; i < pair.second.size(); ++i) {
			double tmp = typeMap["BJ"][0].second[i] / pair.second[i];
			tmp = log(tmp);
			Buffer[++k] = tmp;
		}
		for (int i = 140; i <= 180; ++i) {
			record += Buffer[i];
		}
		double tmp = 0;
		for (int i = 300; i <= 320; ++i) {
			tmp += Buffer[i];
		}
		tmp /= 20;
		record -= tmp;
		tmpDataHC.push_back(std::make_pair(pair.first, record));
	}

	for (auto& pair : typeMap["NO"]) {
		k = 0, record = 0, Buffer[0] = 0;
		for (int i = 0; i < pair.second.size(); ++i) {
			double tmp = typeMap["BJ"][0].second[i] / pair.second[i];
			tmp = log(tmp);
			Buffer[++k] = tmp;
		}
		for (int i = 140; i <= 180; ++i) {
			record += Buffer[i];
		}
		double tmp = 0;
		for (int i = 300; i <= 320; ++i) {
			tmp += Buffer[i];
		}
		tmp /= 20;
		record -= tmp;
		tmpDataNO.push_back(std::make_pair(pair.first, record));
	}
	return;
}

const void CProgressDLG::InitConf() {
	intptr_t hFile = 0; //文件句柄
	struct _finddata_t fileinfo; //文件信息
	const char* confFolder = "data\\HCNO";
	std::string p;

	
	if ((hFile = _findfirst(p.assign(confFolder).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if (!(fileinfo.attrib & _A_SUBDIR)) {
				std::string tmpPath = p.assign(confFolder).append("\\").append(fileinfo.name);
				Conf tmpConf;
				tmpConf.LoadConf(tmpPath.c_str());
				if (flag) tmpConf.PrintConf();
				dataList.push_back(tmpConf);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
	}

	LoadtmpData();

	std::sort(tmpDataHC.begin(), tmpDataHC.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
		return a.second < b.second; });
	std::sort(tmpDataNO.begin(), tmpDataNO.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
		return a.second < b.second; });

	CurveFitting::LeastSquaresFunction LSF;
	LSF.setDegree(2);
	LSF.CalculateData(tmpDataHC, coeffHC);
	LSF.CalculateData(tmpDataNO, coeffNO);
}

// CProgressDLG 对话框

IMPLEMENT_DYNAMIC(CProgressDLG, CDialogEx)

CProgressDLG::CProgressDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Progress, pParent)
{

}

CProgressDLG::~CProgressDLG()
{
}

BOOL CProgressDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	int msgboxID = MessageBox("是否加载原始数据图像？", "提示", MB_YESNO | MB_ICONQUESTION);
	if (msgboxID == IDNO) {
		flag = false;
	}

	InitConf();

	//SetWindowPos(&wndTop, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE | SWP_NOZORDER);

	MyProg.SetRange(0, 100);
	MyProg.SetPos(0);
	//MyProg.SetBkColor(RGB(255, 255, 255));
	//MyProg.SetBarColor(RGB(0, 128, 0));


	
	//利用CCoeff类将coeffHC 和 coeffNO 传给CMyMenu
	CMyMenu* pMyMenu = (CMyMenu*)AfxGetApp()->GetMainWnd();
	CCoeff CoeffHC, CoeffNO;
	CoeffHC.setCoeff(coeffHC), CoeffNO.setCoeff(coeffNO);
	pMyMenu->SetCoeff(CoeffHC, CoeffNO);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CProgressDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, MyProg);
}


BEGIN_MESSAGE_MAP(CProgressDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CProgressDLG::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDLG 消息处理程序

void CProgressDLG::OnCancel()
{
	exit(0);
	// TODO: 在此添加控件通知处理程序代码	
}
void CProgressDLG::OnBnClickedButton1()
{
	UINT_PTR timerID = SetTimer(2, 25, NULL);
	CString s = "正在拟合数据，请稍等...";
	SetDlgItemText(IDC_STATIC3, s);
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
	pBtn->EnableWindow(FALSE);
	// TODO: 在此添加控件通知处理程序代码
}

void CProgressDLG::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData();
	int iPos = 0;
	iPos = MyProg.GetPos();//获取当前进度条位置
	iPos += 1;
	MyProg.SetPos(iPos);
	CString s1, s2;
	s1.Format(_T("%d"), iPos);
	s2 = _T("%");
	SetDlgItemText(IDC_STATIC4, s1 + s2);
	if (iPos == 100)//当达到100的时候就关闭定时器,并且事按钮可用,位置设为0
	{
		KillTimer(1);
		MyProg.SetPos(0);
		CButton* pBtn = (CButton*)GetDlgItem(IDC_BUTTON1);
		pBtn->EnableWindow(TRUE);
		//返回主窗口
		CMyMenu* pMyMenu = (CMyMenu*)AfxGetApp()->GetMainWnd();
		pMyMenu->ShowWindow(SW_SHOW);
		pMyMenu->SetFocus();
		EndDialog(0);
	}
	CDialogEx::OnTimer(nIDEvent);
}

