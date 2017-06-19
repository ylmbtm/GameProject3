#include "stdafx.h"
#include "Grid.h"

CGrid::CGrid(void)
{
	m_pHead = NULL;
}

CGrid::~CGrid(void)
{
}

BOOL CGrid::AddObject( CWorldObject *pObject )
{
	ASSERT(pObject->m_pGridNext == NULL);
	ASSERT(pObject->m_pGridPrev == NULL);

	if(m_pHead == NULL)
	{
		m_pHead = pObject;

		m_pHead->m_pGridNext = NULL;

		m_pHead->m_pGridPrev = NULL;

		return TRUE;
	}

	pObject->m_pGridNext = m_pHead;

	pObject->m_pGridPrev = NULL;
	
	m_pHead->m_pGridPrev = pObject;

	m_pHead = pObject;

	return TRUE;
}

BOOL CGrid::RemoveObject(CWorldObject *pObject)
{
	if(m_pHead == pObject)
	{
		if(pObject->m_pGridNext != NULL)
		{
			ASSERT(pObject->m_pGridNext->m_pGridPrev == pObject);
		}

		m_pHead = pObject->m_pGridNext;

		if(m_pHead != NULL)
		{
			m_pHead->m_pGridPrev = NULL;
		}
	}
	else
	{
		CWorldObject *pTest = pObject;
		while(pTest->m_pGridPrev != NULL)
		{
			pTest = pTest->m_pGridPrev;
		}

		ASSERT(m_pHead == pTest);

		ASSERT(pObject->m_pGridPrev->m_pGridNext == pObject);

		pObject->m_pGridPrev->m_pGridNext = pObject->m_pGridNext;

		if(pObject->m_pGridNext != NULL)
		{
			pObject->m_pGridNext->m_pGridPrev = pObject->m_pGridPrev;
		}
	}

	pObject->m_pGridNext = NULL;

	pObject->m_pGridPrev = NULL;

	return TRUE;
}

BOOL CGrid::IsObjectExist( CWorldObject *pObject )
{
	CWorldObject *pTemp = m_pHead;
	if(pTemp == NULL)
	{
		return FALSE;
	}

	while (pTemp != NULL)
	{
		if(pTemp == pObject)
		{
			return TRUE;
		}

		pTemp = pTemp->m_pGridNext;
	}

	return FALSE;
}

