// DlgGame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "SceneView.h"
#include "CommandHandler.h"


// CSceneView 对话框

IMPLEMENT_DYNAMIC(CSceneView, CDialog)

CSceneView::CSceneView(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneView::IDD, pParent)
{

}

CSceneView::~CSceneView()
{
}

void CSceneView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSceneView, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSceneView 消息处理程序

void CSceneView::OnPaint()
{
	CPaintDC dc(this);

	CClientCmdHandler *pClientCmdHandler = CClientCmdHandler::GetInstancePtr();
	if(pClientCmdHandler == NULL)
	{
		return ;
	}

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(&rc, RGB(255,255,255));
	
	DrawPlayer(dc.GetSafeHdc(), /*MapPointToViewPoint(*/pClientCmdHandler->m_HostPlayer.m_ObjectPos.x/*)*/, /*MapPointToViewPoint(*/pClientCmdHandler->m_HostPlayer.m_ObjectPos.z/*)*/, pClientCmdHandler->m_HostPlayer.m_ObjectStatus.nDir, pClientCmdHandler->m_HostPlayer.m_szObjectName);

	for(CPlayerObjectMgr::iterator itor = pClientCmdHandler->m_PlayerObjMgr.begin(); itor != pClientCmdHandler->m_PlayerObjMgr.end(); ++itor)
	{
		CPlayerObject *pObject = itor->second;

		DrawPlayer(dc.GetSafeHdc(),  /*MapPointToViewPoint(*/pObject->m_ObjectPos.x/*)*/, /*MapPointToViewPoint(*/pObject->m_ObjectPos.z/*)*/,pObject->m_ObjectStatus.nDir, pObject->m_szObjectName);

		//DrawItem(dc.GetSafeHdc(), rand()%300,rand()%300);

		//DrawNpc(dc.GetSafeHdc(), rand()%300,rand()%300);
	}

	return ;
}

void CSceneView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

VOID CSceneView::DrawPlayer( HDC hDC, int nX, int nY , int nDir, char *szName)
{
	int nTempX = nX;
	int nTempY = nY;

	nX = MapPointToViewPoint(nX);
	nY = MapPointToViewPoint(nY);

	RECT Rc;
	Rc.left		= nX - 40;
	Rc.right	= nX + 40;
	Rc.top		= nY - 20;
	Rc.bottom	= nY+10;
	DrawText(hDC, szName, strlen(szName), &Rc, DT_CENTER);

	Rc.top		-= 15;
	Rc.bottom	-= 15;
	char szValue[128];
	sprintf(szValue, "(%d, %d)", nTempX, nTempY);
	DrawText(hDC, szValue, strlen(szValue), &Rc, DT_CENTER);
	Draw(hDC, nX, nY, nDir, RGB(255, 0 , 0), 5);

	return ;
}

VOID CSceneView::DrawNpc( HDC hDC,int nX, int nY , char *szName)
{
	RECT Rc;
	Rc.left = nX - 40;
	Rc.right = nX + 40;
	Rc.top   = nY - 20;
	Rc.bottom = nY+10;
	DrawText(hDC, szName, strlen(szName), &Rc, DT_CENTER);
	Draw(hDC, nX, nY, 0, RGB(0 ,255,  0), 5);
	return ;
}

VOID CSceneView::DrawItem( HDC hDC,int nX, int nY )
{
	Draw(hDC, nX, nY, 0, RGB(0 ,  0, 255), 7);
	return ;
}

VOID CSceneView::Draw( HDC hDC,int nX, int nY, int nDir, COLORREF clr, int nRadius )
{
	CPoint point;

	MoveToEx(hDC, nX-(nX%50)-50, nY-(nY%50)-50, &point);
	LineTo(hDC, nX-(nX%50)+100, nY-(nY%50)-50);
	LineTo(hDC, nX-(nX%50)+100, nY-(nY%50)+100);
	LineTo(hDC, nX-(nX%50)-50, nY-(nY%50)+100);
	LineTo(hDC, nX-(nX%50)-50, nY-(nY%50)-50);

	MoveToEx(hDC, nX-75, nY-75, &point);
	LineTo(hDC, nX+75, nY-75);
	LineTo(hDC, nX+75, nY+75);
	LineTo(hDC, nX-75, nY+75);
	LineTo(hDC, nX-75, nY-75);

	MoveToEx(hDC, nX, nY, &point);
	switch(nDir)
	{
	case 1:
		{
			LineTo(hDC, nX+10, nY);
		}
		break;
	case 2:
		{
			LineTo(hDC, nX, nY+10);
		}
		break;
	case 3:
		{
			LineTo(hDC, nX-10, nY);
		}
		break;
	case 4:
		{
			LineTo(hDC, nX, nY-10);
		}
		break;
	}

	HBRUSH hNewBrush = CreateSolidBrush(clr);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);
	Ellipse(hDC, nX - nRadius, nY - nRadius, nX + nRadius, nY + nRadius);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hNewBrush);
}

int CSceneView::MapPointToViewPoint( FLOAT fMapPoint )
{
	return (fMapPoint+1000)/2;
}
