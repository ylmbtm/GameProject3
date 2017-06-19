// DlgRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "DlgRegister.h"
#include "CommandHandler.h"


// CDlgRegister 对话框

IMPLEMENT_DYNAMIC(CDlgRegister, CDialog)

CDlgRegister::CDlgRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRegister::IDD, pParent)
	, m_StrAccountName(_T(""))
	, m_strPassword(_T(""))
{

}

CDlgRegister::~CDlgRegister()
{
}

void CDlgRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ACCOUNTNAME, m_StrAccountName);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgRegister::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRegister 消息处理程序

void CDlgRegister::OnBnClickedOk()
{
	UpdateData(TRUE);

	CClientCmdHandler::GetInstancePtr()->SendNewAccountReq((LPCTSTR)m_StrAccountName, (LPCTSTR)m_strPassword);

	OnOK();
}
