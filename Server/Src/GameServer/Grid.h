#ifndef __GRID_H__
#define __GRID_H__

#include "GameObject/MapObject.h"

#define CELL_SIZE 50

class CGrid
{
public:
	CGrid(void);
	~CGrid(void);

	BOOL AddObject(CMapObject* pObject);

	BOOL RemoveObject(CMapObject* pObject);

	BOOL IsObjectExist(CMapObject* pObject);

	CMapObject* GetHeadObject();

	//以下是新的版本
	CMapObject* m_pHead;
};

#endif