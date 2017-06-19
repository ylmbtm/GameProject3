#pragma once


// CDlgRegister 对话框

class CDlgRegister : public CDialog
{
	DECLARE_DYNAMIC(CDlgRegister)

public:
	CDlgRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRegister();

// 对话框数据
	enum { IDD = IDD_DLG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_StrAccountName;
	CString m_strPassword;
	afx_msg void OnBnClickedOk();
};
