#ifndef _WORLD_OBJECT_H_
#define _WORLD_OBJECT_H_
#include "Utility/Position.h"
class CGrid;
class CScene;

template <typename TContainer, typename TElem>
class CNodeLink
{
public:
    CNodeLink()
    {
        m_pNext = NULL;
        m_pPrev = NULL;
        m_pContainer = NULL;
    }

	TElem		*m_pNext;
	TElem		*m_pPrev;
	TContainer	*m_pContainer;
};

class CWorldObject
{
public:
	CWorldObject();

	~CWorldObject();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual BOOL    SetUpdate(UpdateStatusEnum UpdateStatus);

	virtual UINT32	WriteToBuffer(UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(UINT32 dwChangeFlag);

public:
	CPosition&	GetPosition();

	VOID		SetPosition(FLOAT x, FLOAT y, FLOAT z);

	CScene*     GetOwnerScene();

	VOID		SetOwnerScene(CScene *pScene);

public:
	//CNodeLink<CGrid, CWorldObject>	m_GridLink;

	CWorldObject		*m_pGridNext;
	CWorldObject		*m_pGridPrev;

public:
	CPosition	m_ObjectPos;

	CScene		*m_pOwnerScene;		

public:
	CPosition			m_UpdateObjPos;//己经同步出去的地址
	UpdateStatusEnum	m_UpdateStatus;
	UpdateStatusEnum	GetUpdateStatus();

};

#endif //_WORLD_OBJECT_H_
