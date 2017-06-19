#include "stdafx.h"
#include "GridManager.h"

CGridManager::CGridManager(void)
{

}

CGridManager::~CGridManager(void)
{
}

BOOL CGridManager::Init( INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom )
{
	clear();

	m_nMaxRows = (nBottom - nTop) / CELL_SIZE + 1;

	m_nMaxCols   = (nRight - nLeft) / CELL_SIZE + 1;

	assign(m_nMaxRows*m_nMaxCols,CGrid());

	m_nLeft		= nLeft;
	m_nRight	= nRight;
	m_nBottom	= nBottom;
	m_nTop		= nTop;

	return TRUE;
}

CGrid* CGridManager::GetGridByPos( FLOAT x, FLOAT z )
{
	return GetGridByIndex(GetIndexByPos(x, z));
}

INT32 CGridManager::GetIndexByPos( FLOAT x, FLOAT z )
{
	if((z < m_nTop)||(z > m_nBottom)||(x < m_nLeft)||(x > m_nRight))
	{
		return -1;
	}

	INT32 nRow = (z-m_nTop) / CELL_SIZE + 1;
	INT32 nCol = (x-m_nLeft) / CELL_SIZE + 1;

	if((nRow >= m_nMaxRows)||(nCol >= m_nMaxCols))
	{
		return -1;
	}

	return nRow*m_nMaxCols + nCol;
}

CGrid* CGridManager::GetGridByIndex( INT32 nIndex )
{
	if(nIndex < 0)
	{
		return NULL;
	}

	return &at(nIndex);
}

BOOL CGridManager::AddObjectToGrid( CWorldObject *pWorldObject, FLOAT x, FLOAT z )
{
	CGrid *pGrid = GetGridByPos(x, z);
	if(pGrid == NULL)
	{
		return FALSE;
	}

	return pGrid->AddObject(pWorldObject);
}

//将对象添加到网格中
BOOL CGridManager::AddObjectToGrid(CWorldObject *pWorldObject, INT32 nIndex)
{
	CGrid *pGrid = GetGridByIndex(nIndex);
	if(pGrid == NULL)
	{
		return FALSE;
	}

	return pGrid->AddObject(pWorldObject);
}

BOOL CGridManager::RemoveObjectFromGrid( CWorldObject *pWorldObject, FLOAT x, FLOAT z )
{
	CGrid *pGrid = GetGridByPos(x, z);
	if(pGrid == NULL)
	{
		return FALSE;
	}

	return pGrid->RemoveObject(pWorldObject);;
}

//将对象从网格中删除
BOOL CGridManager::RemoveObjectFromGrid(CWorldObject *pWorldObject, INT32 nIndex)
{
	CGrid *pGrid = GetGridByIndex(nIndex);
	if(pGrid == NULL)
	{
		return FALSE;
	}

	return pGrid->RemoveObject(pWorldObject);;
}

BOOL CGridManager::CalDiffGrids( INT32 nSrcIndex, INT32 nDestIndex, INT32 IncGrids[10], INT32 DecGrids[10], INT32 StayGrids[10] )
{
	INT32 SrcGrids[10], DestGrids[10];

	GetSurroundingGrids(nSrcIndex, SrcGrids);
	GetSurroundingGrids(nDestIndex, DestGrids);
	
	BOOL bFinished = FALSE;
	INT32 i = 0, j = 0;
	INT32 nInc = 0, nDec = 0, nStay = 0; 
	while(!bFinished)
	{
		if((SrcGrids[i] == -1)&&(DestGrids[j]==-1))
		{
			bFinished = TRUE;
			continue;
		}

		if(SrcGrids[i] == -1)
		{
			IncGrids[nInc] = DestGrids[j];
			nInc++;
			j++;
			continue;
		}

		if(DestGrids[j] == -1)
		{
			DecGrids[nDec] = SrcGrids[i];
			i++;
			nDec++;
			continue;
		}

		if(SrcGrids[i] == DestGrids[j])
		{
			StayGrids[nStay] = SrcGrids[i];
			i++;
			j++;
			nStay++;
		}
		else if(SrcGrids[i] < DestGrids[j])
		{
			DecGrids[nDec] = SrcGrids[i];
			i++;
			nDec++;
		}
		else
		{
			IncGrids[nInc] = DestGrids[j];
			j++;
			nInc++;
		}
	}

	while(TRUE)
	{
		if(nInc <= 9)
		{
			IncGrids[nInc] = -1;
			nInc++;
		}
		
		if(nDec <= 9)
		{
			DecGrids[nDec] = -1;
			nDec++;
		}

		if(nStay <= 9)
		{
			StayGrids[nStay]= -1;
			nStay++;
		}
		
		if((nInc == 10)&&(nStay == 10)&&(nDec == 10))
		{
			break;
		}
	}

	

	return TRUE;
}

BOOL CGridManager::GetSurroundingGrids( INT32 nIndex, INT32 sGridsp[10])
{
	if(nIndex%m_nMaxCols == 0)
	{
		sGridsp[0] = (nIndex/m_nMaxCols-1)*m_nMaxCols;
		sGridsp[1] = sGridsp[0] + 1;
		sGridsp[2] = nIndex;
		sGridsp[3] = sGridsp[2] + 1;
		sGridsp[4] = (nIndex/m_nMaxCols+1)*m_nMaxCols;
		sGridsp[5] = sGridsp[4] + 1;
		sGridsp[6] = -1;
		sGridsp[7] = -1;
		sGridsp[8] = -1;
	}
	else if(((nIndex+1)%m_nMaxCols) == 0)
	{
		sGridsp[0] = (nIndex/m_nMaxCols-1)*m_nMaxCols+ nIndex%m_nMaxCols-1;
		sGridsp[1] = sGridsp[0] + 1;
		sGridsp[2] = nIndex - 1;
		sGridsp[3] = sGridsp[2] + 1;
		sGridsp[4] = (nIndex/m_nMaxCols+1)*m_nMaxCols+ nIndex%m_nMaxCols-1;
		sGridsp[5] = sGridsp[4] + 1;
		sGridsp[6] = -1;
		sGridsp[7] = -1;
		sGridsp[8] = -1;
	}
	else
	{
		sGridsp[0] = (nIndex/m_nMaxCols-1)*m_nMaxCols+ nIndex%m_nMaxCols-1;
		sGridsp[1] = sGridsp[0] + 1;
		sGridsp[2] = sGridsp[1] + 1;
		sGridsp[3] = nIndex - 1;
		sGridsp[4] = sGridsp[3] + 1;
		sGridsp[5] = sGridsp[4] + 1;
		sGridsp[6] = (nIndex/m_nMaxCols+1)*m_nMaxCols+ nIndex%m_nMaxCols-1;
		sGridsp[7] = sGridsp[6] + 1;
		sGridsp[8] = sGridsp[7] + 1;
	}

	sGridsp[9] = -1;

	return TRUE;
}

BOOL CGridManager::IsObjectExist( CWorldObject *pWorldObject, INT32 dwIndex )
{
	CGrid *pGrid = GetGridByIndex(dwIndex);
	if(pGrid == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return pGrid->IsObjectExist(pWorldObject);
}
