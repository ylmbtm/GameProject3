#ifndef __GRID_MANAGER_H__
#define __GRID_MANAGER_H__
#include "Grid.h"

class CGridManager : public std::vector<CGrid>
{
public:
	CGridManager(void);
	~CGridManager(void);

	//初始化网格数据
	BOOL		Init(INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom);

	//根据位置坐标得到网格
	CGrid*		GetGridByPos(FLOAT x, FLOAT z);

	//根据位置坐标到得网格索引
	INT32		GetIndexByPos(FLOAT x, FLOAT z);

	//根据索引得到网格
	CGrid*		GetGridByIndex(INT32 nIndex);

	//将对象添加到网格中
	BOOL		AddObjectToGrid(CWorldObject *pWorldObject, FLOAT x, FLOAT z);

	//将对象添加到网格中
	BOOL		AddObjectToGrid(CWorldObject *pWorldObject, INT32 nIndex);

	//将对象从网格中删除
	BOOL		RemoveObjectFromGrid(CWorldObject *pWorldObject, FLOAT x, FLOAT z);

	//将对象从网格中删除
	BOOL		RemoveObjectFromGrid(CWorldObject *pWorldObject, INT32 nIndex);

	//计算差异格
	BOOL		CalDiffGrids( INT32 nSrcIndex, INT32 nDestIndex, INT32 IncGrids[10], INT32 DecGrids[10], INT32 StayGrids[10]);

	//取周围格子
	BOOL		GetSurroundingGrids(INT32 nIndex, INT32 sGridsp[10]);

	//以下是用来测试的方法
	BOOL		IsObjectExist(CWorldObject *pWorldObject, INT32 dwIndex);

public:

	INT32	m_nMaxRows;
	INT32	m_nMaxCols;

	INT32	m_nLeft, m_nRight, m_nTop, m_nBottom;
};

#endif /*__GRID_MANAGER_H__*/