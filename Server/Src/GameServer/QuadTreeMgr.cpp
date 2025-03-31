/****************************************************
文件：QuadTreeManager.cpp
作者：ylmbtm@163.com
日期：2025/03/27 11:19:18
功能：四叉树地图管理
*****************************************************/


#include "stdafx.h"
#include "QuadTreeMgr.h"

CQuadTreeManager::CQuadTreeManager(void)
{

}

CQuadTreeManager::~CQuadTreeManager(void)
{
}

BOOL CQuadTreeManager::Init( FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom )
{
    m_pTreeRoot = new QuadTreeNode;
    m_pTreeRoot->m_fLeft = fLeft;
    m_pTreeRoot->m_fRight = fRight;
    m_pTreeRoot->m_fTop = fTop;
    m_pTreeRoot->m_fBottom = fBottom;
    m_pTreeRoot->m_bLeaf = TRUE;
    m_pTreeRoot->m_nDepth = 1;

    return TRUE;
}

BOOL CQuadTreeManager::AddMapObject(CMapObject* pMapObject)
{
    return m_pTreeRoot->InsertObject(pMapObject);
}

BOOL CQuadTreeManager::RemoveMapObject(CMapObject* pMapObject)
{
    return m_pTreeRoot->RemoveObject(pMapObject);
}

BOOL CQuadTreeManager::SearchObject(Rect2D rcArea, std::list<CMapObject*>& objectList)
{
    return m_pTreeRoot->SearchObject(rcArea, objectList);
}

QuadTreeNode* CQuadTreeManager::GetTreeNodeByPos(FLOAT x, FLOAT z)
{
    if (m_pTreeRoot->m_bLeaf)
    {
        return m_pTreeRoot;
    }

    return m_pTreeRoot->GetTreeNodeByPos(x, z);
}

QuadTreeNode::QuadTreeNode()
{

}

QuadTreeNode::QuadTreeNode(QuadTreeNode* pParent, FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom, INT32 nDepth)
{
    m_fLeft = fLeft;
    m_fRight = fRight;
    m_fTop = fTop;
    m_fBottom = fBottom;
    m_bLeaf = TRUE;
    m_nDepth = nDepth;
    m_pParent = pParent;
}

BOOL QuadTreeNode::TrySplit()
{
    if (m_objectList.size() <= NODE_SPLIT_NUM)
    {
        //格子里的对象数少于NODE_SPLIT_NUM， 所以不需要分割
        return TRUE;
    }

    if (m_nDepth >= NODE_MAX_DEPTH)
    {
        //己达最大深度不再向下分割
        return TRUE;
    }


    FLOAT nMidH = (m_fLeft + m_fRight) / 2;
    FLOAT nMidV = (m_fTop + m_fBottom) / 2;

    m_bLeaf = FALSE;  //分割之后，本节点就不是叶子节点了

    m_pChildNodes[0] = new QuadTreeNode(this, m_fLeft, m_fTop, nMidH, nMidV, m_nDepth + 1);
    m_pChildNodes[1] = new QuadTreeNode(this, nMidH, m_fTop, m_fRight, nMidV, m_nDepth + 1);
    m_pChildNodes[2] = new QuadTreeNode(this, m_fLeft, nMidV, nMidH, m_fBottom, m_nDepth + 1);
    m_pChildNodes[3] = new QuadTreeNode(this, nMidH, nMidV, m_fRight, m_fBottom, m_nDepth + 1);

    for (auto itor = m_objectList.begin(); itor != m_objectList.end();)
    {
        CMapObject* pTempObject = *itor;
        for (int i = 0; i < NODE_MAX_CHILD; i++)
        {
            if (m_pChildNodes[i]->Contains(pTempObject->m_aPos))
            {
                m_pChildNodes[i]->InsertObject(pTempObject);
                break;
            }
        }

        itor = m_objectList.erase(itor);
    }

    return TRUE;
}

BOOL QuadTreeNode::TryMerge()
{
    if (m_pParent == NULL)
    {
        return FALSE;
    }

    if (m_bLeaf)
    {
        return FALSE;
    }

    INT32 nCount = 0;
    for (INT32 i = 0; i < NODE_MAX_CHILD; i++)
    {
        nCount += m_pChildNodes[i]->GetObjectCount();
    }

    if(nCount > NODE_MERGE_NUM)
    {
        return FALSE;
    }

    for (INT32 i = 0; i < NODE_MAX_CHILD; i++)
    {
        m_objectList.merge(m_pChildNodes[i]->m_objectList);
        delete m_pChildNodes[i];
        m_pChildNodes[i] = NULL;
    }

    m_bLeaf = TRUE;

    return TRUE;
}

INT32 QuadTreeNode::GetObjectCount()
{
    return (INT32)m_objectList.size();
}

BOOL QuadTreeNode::Contains(CPoint2D pt)
{
    return  (pt.m_x >= m_fLeft && pt.m_y >= m_fTop && pt.m_x <= m_fRight && pt.m_y <= m_fBottom);
}

BOOL QuadTreeNode::Intersects(Rect2D rcArea)
{
    return !(m_fRight < rcArea.m_fLeft || rcArea.m_fRight < m_fLeft || m_fTop < rcArea.m_fBottom || rcArea.m_fTop < m_fBottom);
}

QuadTreeNode* QuadTreeNode::GetTreeNodeByPos(FLOAT x, FLOAT z)
{
    for (int i = 0; i < NODE_MAX_CHILD; i++)
    {
        if (m_pChildNodes[i]->Contains(CPoint2D(x, z)))
        {
            if (m_pChildNodes[i]->m_bLeaf)
            {
                return m_pChildNodes[i];
            }

            return m_pChildNodes[i]->GetTreeNodeByPos(x, z);
        }
    }

    return NULL;
}

BOOL QuadTreeNode::InsertObject(CMapObject* pObject)
{
    if (m_bLeaf)
    {
        m_objectList.push_back(pObject);

        TrySplit();

        return TRUE;
    }

    for (int i = 0; i < NODE_MAX_CHILD; i++)
    {
        if (m_pChildNodes[i]->Contains(pObject->m_aPos))
        {
            m_pChildNodes[i]->InsertObject(pObject);

            return TRUE;
        }
    }

    return FALSE;
}

BOOL QuadTreeNode::RemoveObject(CMapObject* pObject)
{
    if (m_bLeaf)
    {
        for (auto itor = m_objectList.begin(); itor != m_objectList.end(); ++itor)
        {
            if ((*itor) == pObject)
            {
                m_objectList.erase(itor);

                return TRUE;
            }
        }

        return FALSE;
    }

    BOOL bSuccess = FALSE;

    for (int i = 0; i < NODE_MAX_CHILD; i++)
    {
        if (m_pChildNodes[i]->Contains(pObject->m_aPos))
        {
            bSuccess = m_pChildNodes[i]->RemoveObject(pObject);
            break;
        }
    }

    if (bSuccess)
    {
        TryMerge();
    }

    return bSuccess;
}

BOOL QuadTreeNode::SearchObject(Rect2D rcArea, std::list<CMapObject*>& objectList)
{
    if (m_bLeaf)
    {
        for (auto itor = m_objectList.begin(); itor != m_objectList.end(); ++itor)
        {
            if (rcArea.Contains((*itor)->m_aPos))
            {
                objectList.push_back(*itor);
            }
        }
    }
    else
    {
        for (int i = 0; i < NODE_MAX_CHILD; i++)
        {
            if (m_pChildNodes[i]->Intersects(rcArea))
            {
                m_pChildNodes[i]->SearchObject(rcArea, objectList);
            }
        }
    }

    return TRUE;
}
