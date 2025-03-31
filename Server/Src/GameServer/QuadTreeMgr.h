#ifndef __QUAD_TREE_MANAGER_H__
#define __QUAD_TREE_MANAGER_H__

#include "CommonMath.h"
#include "GameObject/MapObject.h"

#define NODE_SPLIT_NUM 8   //格子对象数达到就开始分割
#define NODE_MERGE_NUM 4   //格子对象数小于就开始合并
#define NODE_MAX_CHILD 4   //最大分割子格子数
#define NODE_MAX_DEPTH 5   //最大分割层数

struct QuadTreeNode
{
    QuadTreeNode();

    QuadTreeNode(QuadTreeNode* pParent, FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom, INT32 nDepth);

    BOOL InsertObject(CMapObject* pObject);

    BOOL RemoveObject(CMapObject* pObject);

    BOOL SearchObject(Rect2D rcArea, std::list<CMapObject*>& objectList);

    BOOL TrySplit();

    BOOL TryMerge();

    INT32 GetObjectCount();

    BOOL Contains(CPoint2D pt);

    BOOL Intersects(Rect2D rcArea);

    QuadTreeNode* GetTreeNodeByPos(FLOAT x, FLOAT z);
public:
    INT32  m_nDepth = 0;
    BOOL   m_bLeaf = TRUE;
    FLOAT m_fLeft = 0;
    FLOAT m_fTop = 0;
    FLOAT m_fBottom = 0;
    FLOAT m_fRight = 0;
    QuadTreeNode* m_pParent = NULL;
    QuadTreeNode* m_pChildNodes[NODE_MAX_CHILD] = { NULL };
    std::list<CMapObject*> m_objectList;
};

class CQuadTreeManager
{
public:
    CQuadTreeManager(void);
    ~CQuadTreeManager(void);

    //初始化网格数据
    BOOL        Init(FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom);

    BOOL        AddMapObject(CMapObject* pMapObject);

    BOOL        RemoveMapObject(CMapObject* pMapObject);

    BOOL        SearchObject(Rect2D rcArea, std::list<CMapObject*>& objectList);

    QuadTreeNode* GetTreeNodeByPos(FLOAT x, FLOAT z);

    QuadTreeNode* m_pTreeRoot;
};

#endif /*__QUAD_TREE_MANAGER_H__*/