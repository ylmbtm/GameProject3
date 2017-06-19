#pragma once


// CSceneView 对话框

class CSceneView : public CDialog
{
	DECLARE_DYNAMIC(CSceneView)

public:
	CSceneView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSceneView();

// 对话框数据
	enum { IDD = IDD_Dlg_Game };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	VOID  DrawPlayer(HDC hDC, int nX, int nY, int nDir, char *szName);

	VOID  DrawNpc(HDC hDC,int nX, int nY, char *szName);

	VOID  DrawItem(HDC hDC,int nX, int nY);

	VOID  Draw(HDC hDC,int nX, int nY, int nDir, COLORREF clr, int nRadius);


	int MapPointToViewPoint(FLOAT fMapPoint);

	
};
