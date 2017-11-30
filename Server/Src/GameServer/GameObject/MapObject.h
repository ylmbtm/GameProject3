#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__

class CGrid;
class CScene;

/*
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

	TElem*		m_pNext;
	TElem*		m_pPrev;
	TContainer*	m_pContainer;
};

CNodeLink<CGrid, CMapObject>	m_GridLink;
*/

class CMapObject
{
public:
	CMapObject();

	~CMapObject();

public:
	BOOL		SetUpdate();

	CScene*     GetScene();

	VOID		SetScene(CScene* pScene);

	CMapObject* NextObject();

	CMapObject* PrevObject();

	//for (CSceneObject* pSceneObject = grid.GetHeadObject(); pSceneObject != NULL; pSceneObject = (CSceneObject*)pSceneObject->NextObject())
	//{
	//
	//}

public:
	CMapObject*		m_pObjectNext;
	CMapObject*		m_pObjectPrev;

public:
	CScene*		m_pScene;

public:

};

#endif //__MAP_OBJECT_H__
