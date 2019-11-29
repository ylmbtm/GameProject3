#include "stdafx.h"
#include "Grid.h"

CGrid::CGrid(void)
{
	m_pHead = NULL;
}

CGrid::~CGrid(void)
{
}

BOOL CGrid::AddObject(CMapObject* pObject )
{
	ERROR_RETURN_FALSE(pObject->m_pObjectNext == NULL);
	ERROR_RETURN_FALSE(pObject->m_pObjectPrev == NULL);

	if(m_pHead == NULL)
	{
		m_pHead = pObject;

		m_pHead->m_pObjectNext = NULL;

		m_pHead->m_pObjectPrev = NULL;

		return TRUE;
	}

	pObject->m_pObjectNext = m_pHead;

	pObject->m_pObjectPrev = NULL;

	m_pHead->m_pObjectPrev = pObject;

	m_pHead = pObject;

	return TRUE;
}

BOOL CGrid::RemoveObject(CMapObject* pObject)
{
	if(m_pHead == pObject)
	{
		if(pObject->m_pObjectNext != NULL)
		{
			ERROR_RETURN_FALSE(pObject->m_pObjectNext->m_pObjectPrev == pObject);
		}

		m_pHead = pObject->m_pObjectNext;

		if(m_pHead != NULL)
		{
			m_pHead->m_pObjectPrev = NULL;
		}
	}
	else
	{
		CMapObject* pTest = pObject;
		while(pTest->m_pObjectPrev != NULL)
		{
			pTest = pTest->m_pObjectPrev;
		}

		ERROR_RETURN_FALSE(m_pHead == pTest);

		ERROR_RETURN_FALSE(pObject->m_pObjectPrev->m_pObjectNext == pObject);

		pObject->m_pObjectPrev->m_pObjectNext = pObject->m_pObjectNext;

		if(pObject->m_pObjectNext != NULL)
		{
			pObject->m_pObjectNext->m_pObjectPrev = pObject->m_pObjectPrev;
		}
	}

	pObject->m_pObjectNext = NULL;

	pObject->m_pObjectPrev = NULL;

	return TRUE;
}

BOOL CGrid::IsObjectExist(CMapObject* pObject)
{
	CMapObject* pTemp = m_pHead;
	ERROR_RETURN_FALSE(pTemp != NULL);

	while (pTemp != NULL)
	{
		if(pTemp == pObject)
		{
			return TRUE;
		}

		pTemp = pTemp->m_pObjectNext;
	}

	return FALSE;
}

CMapObject* CGrid::GetHeadObject()
{
	return m_pHead;
}

