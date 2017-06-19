#ifndef __GRID_H__
#define __GRID_H__

#include "GameObject/WorldObject.h"

#define CELL_SIZE 50

class CGrid
{
public:
	CGrid(void);
	~CGrid(void);

	BOOL AddObject(CWorldObject *pObject);

	BOOL RemoveObject(CWorldObject *pObject);

	BOOL IsObjectExist(CWorldObject *pObject);

	//以下是新的版本
	CWorldObject *m_pHead;
};

#endif