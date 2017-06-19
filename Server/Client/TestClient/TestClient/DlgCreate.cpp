// DlgCreate.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "DlgCreate.h"
#include "CommandHandler.h"
#include "GameDefine.h"
#include "GameStruct.h"


// CDlgCreate 对话框

IMPLEMENT_DYNAMIC(CDlgCreate, CDialog)

CDlgCreate::CDlgCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreate::IDD, pParent)
	, m_strCharName(_T(""))
	, m_nSex(0)
{

}

CDlgCreate::~CDlgCreate()
{
}

void CDlgCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_NAME, m_strCharName);
}


BEGIN_MESSAGE_MAP(CDlgCreate, CDialog)
	ON_BN_CLICKED(IDC_BTN_ZHAN, &CDlgCreate::OnBnClickedBtnZhan)
	ON_BN_CLICKED(IDC_BTN_FA, &CDlgCreate::OnBnClickedBtnFa)
	ON_BN_CLICKED(IDC_BTN_DAO, &CDlgCreate::OnBnClickedBtnDao)
	ON_BN_CLICKED(IDOK, &CDlgCreate::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCreate::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgCreate 消息处理程序

void CDlgCreate::OnBnClickedBtnZhan()
{
	m_nCareer = 1;
}

void CDlgCreate::OnBnClickedBtnFa()
{
	m_nCareer = 2;
}

void CDlgCreate::OnBnClickedBtnDao()
{
	m_nCareer = 3;
}

void CDlgCreate::OnBnClickedOk()
{
	UpdateData(TRUE);

	St_CharFeature charFeature;
	charFeature.Career = m_nCareer;
	CButton *pCheck = (CButton *)GetDlgItem(IDC_RD_MALE);
	if(pCheck->GetCheck())
	{
		m_nSex = 1;
	}
	else
	{
		m_nSex = 0;
	}

	CClientCmdHandler::GetInstancePtr()->SendNewCharReq(CClientCmdHandler::GetInstancePtr()->m_dwAccountID, (LPCTSTR)m_strCharName , charFeature.dwValues);

	OnOK();
}

void CDlgCreate::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
