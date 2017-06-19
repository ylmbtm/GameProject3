
// TestClientDlg.h : 头文件
//

#pragma once
#include "DlgLogin.h"
#include "SceneView.h"
#include "afxwin.h"

// CTestClientDlg 对话框
class CTestClientDlg : public CDialog
{
// 构造
public:
	CTestClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CDlgLogin  m_DlgLogin;

	CSceneView   m_SceneView;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLogin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDisconnect();
	afx_msg void OnConnect();
	afx_msg void OnLeaveGame();
	afx_msg void OnNewAccount();
};
