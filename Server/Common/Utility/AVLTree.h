#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__
#include <vector>

template<typename TKey, typename TValue>
class TreeNode
{
private:
    TreeNode():m_pLeft(NULL),m_pRight(NULL),m_nHeight(0)
	{
	}

public:
	TKey* GetKey()
	{
		return &m_Key;
	}

	TValue* GetValue()
	{
		return &m_Data;
	}

public:
    TValue m_Data;
	TKey   m_Key;
    int	   m_nHeight;
	TreeNode<TKey, TValue> *m_pLeft;
    TreeNode<TKey, TValue> *m_pRight;
	TreeNode<TKey, TValue> *m_pParent;
};

//AVL树类的属性和方法声明
template<typename TKey, typename TValue>
class AVLTree
{
public:
	typedef TreeNode<TKey, TValue>  TNodeType;
	typedef TreeNode<TKey, TValue>* TNodeTypePtr;

public:
	AVLTree():m_pRoot(NULL)
	{
		m_pFreeHead = NULL;

		AllocBufferNode();

		m_nCount = 0;
	}

	~AVLTree()
	{
		FreeBufferNode();
		m_pFreeHead = NULL;
		m_pRoot = NULL;
	}

	//对外的接口
public:
	TValue*			InsertAlloc(TKey Key);
	bool			Insert(TKey Key, TValue Value);
	bool			Delete(TKey Key);
	TNodeTypePtr	Find(TKey Key);
	void			Traversal();
	TValue*			GetByKey(TKey Key);
	bool			Insert(TNodeTypePtr pNode);
	TNodeTypePtr    AllocNode();
	void			FreeNode(TNodeTypePtr pNode);
	void			DoEnumNode(TNodeTypePtr pNode);
	int				GetCount();
	bool			Delete(TNodeTypePtr pNode);
	TNodeTypePtr    MoveFirst();
	TNodeTypePtr    MoveNext(TNodeTypePtr pNode);


private:
	TNodeTypePtr	FindInner(TNodeTypePtr pRootNode, TKey Key);	
    bool			InsertInner(TNodeTypePtr &pParentNode, TNodeTypePtr pInsertNode);	//插入
    void			Insubtree(TNodeTypePtr pRootNode);									//中序遍历
    bool			DeleteInner(TNodeTypePtr &pNode,  TKey Key);		//删除
    void			SingRotateLeft(TNodeTypePtr &pNode);//左左情况下的旋转
    void			SingRotateRight(TNodeTypePtr &pNode);//右右情况下的旋转
    void			DoubleRotateLR(TNodeTypePtr &pNode);//左右情况下的旋转
    void			DoubleRotateRL(TNodeTypePtr &pNode);//右左情况下的旋转
    int				Max(int Value1,int Value2);//求最大值
	int				GetHeight(TNodeTypePtr pNode);

	
	bool			AllocBufferNode(int nSize = 1024);
	bool			FreeBufferNode();
private:
	TNodeType *m_pRoot;       //根节点

	TNodeType *m_pFreeHead;

	int		   m_nCount;

	std::vector<TNodeType*> m_NodeBuff;
};

template<typename TKey, typename TValue>
int AVLTree<TKey, TValue>::GetCount()
{
	return m_nCount;
}

template<typename TKey, typename TValue>
TValue* AVLTree<TKey, TValue>::InsertAlloc( TKey Key )
{
	TNodeTypePtr pNode = AllocNode();
	if(pNode == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	pNode->m_Key  = Key;

	if(m_pRoot == NULL)
	{
		m_pRoot			= pNode;
		m_nCount		+= 1;
		pNode->m_pParent= NULL;
	}
	else
	{
		if(!InsertInner(m_pRoot, pNode))
		{
			FreeNode(pNode);
			ASSERT(FALSE);
			return NULL;
		}
	}

	return &pNode->m_Data;
}

template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::Insert( TNodeTypePtr pNode )
{
	if(m_pRoot == NULL)
	{
		m_pRoot = pNode;

		m_nCount += 1;

		return true;
	}

	return InsertInner(m_pRoot, pNode);
}

template<typename TKey, typename TValue>
TValue* AVLTree<TKey, TValue>::GetByKey( TKey Key )
{
	TNodeTypePtr pNode = Find(Key);
	if(pNode == NULL)
	{
		return NULL;
	}

	return &pNode->m_Data;
}

template<typename TKey, typename TValue>
TreeNode<TKey, TValue>* AVLTree<TKey, TValue>::AllocNode()
{
	if(m_pFreeHead == NULL)
	{
		AllocBufferNode();
	}

	if(m_pFreeHead == NULL)
	{
		return NULL;
	}

	TNodeTypePtr pValidNode = m_pFreeHead;

	m_pFreeHead = m_pFreeHead->m_pRight;

	m_pFreeHead->m_pLeft = NULL;

	pValidNode->m_nHeight = 0;
	pValidNode->m_pLeft   = NULL;
	pValidNode->m_pRight  = NULL;

	return pValidNode;
}

template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::FreeNode(TNodeTypePtr pNode)
{
	pNode->m_pRight = m_pFreeHead;
	m_pFreeHead->m_pLeft = pNode;
	m_pFreeHead = pNode;
	m_pFreeHead->m_pLeft = NULL;

	return ;
}

template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::Delete(TNodeTypePtr pNode)
{
	if((m_pRoot == NULL)||(pNode == NULL))
	{
		return false;
	}

	ASSERT(Find(*pNode->GetKey()) != NULL);

	return Delete(*pNode->GetKey());
}

template<typename TKey, typename TValue>
TreeNode<TKey, TValue>* AVLTree<TKey, TValue>::MoveFirst()
{
	if(m_pRoot == NULL)
	{
		return NULL;
	}

	TreeNode<TKey, TValue>*pTempNode = m_pRoot;
	while(pTempNode->m_pLeft != NULL)
	{
		pTempNode = pTempNode->m_pLeft;
	}

	return pTempNode;
}


template<typename TKey, typename TValue>
TreeNode<TKey, TValue>* AVLTree<TKey, TValue>::MoveNext(TNodeTypePtr pNode)
{
	if(pNode == NULL)
	{
		return NULL;
	}

	TreeNode<TKey, TValue> *pTempNode = NULL;

	if (pNode->m_pRight != NULL)
	{
		pTempNode =(TreeNode<TKey, TValue>*) pNode->m_pRight;
		while (pTempNode->m_pLeft != NULL)
		{
			pTempNode = (TreeNode<TKey, TValue>*)pTempNode->m_pLeft;
		}

		return pTempNode;
	}

	pTempNode = (TreeNode<TKey, TValue>*)pNode->m_pParent;
	while (pTempNode != NULL && pNode == (TreeNode<TKey, TValue>*)pTempNode->m_pRight)
	{
		pNode = pTempNode;

		pTempNode = (TreeNode<TKey, TValue>*)pTempNode->m_pParent;
	}

	return pTempNode;
}


template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::AllocBufferNode(int nSize)
{
	TNodeType *pNode = (TNodeType *)malloc(sizeof(TNodeType) * nSize);
	if(pNode == NULL)
	{
		ASSERT_FAIELD;
		return false;
	}

	memset(pNode, 0, sizeof(TNodeType) * nSize);

	m_NodeBuff.push_back(pNode);

	for(int i = 0; i < nSize; i++)
	{
		pNode->m_Key = i;

		if(m_pFreeHead == NULL)
		{
			m_pFreeHead = pNode;
		}
		else
		{
			pNode->m_pRight = m_pFreeHead;
			m_pFreeHead->m_pLeft = pNode;
			m_pFreeHead = pNode;
			m_pFreeHead->m_pLeft = NULL;
		}

		pNode ++;
	}

	return true;
}

template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::FreeBufferNode()
{
	for(size_t i = 0; i < m_NodeBuff.size(); ++i)
	{
		TNodeType *pNode = m_NodeBuff.at(i);
		if(pNode != NULL)
		{
			free(pNode);
		}
	}

	return true;
}



template<typename TKey, typename TValue>
int AVLTree<TKey, TValue>::Max( int Value1,int Value2 )
{
	 return Value1>Value2?Value1:Value2;
}

//求最大值
template<typename TKey, typename TValue>
int AVLTree<TKey, TValue>::GetHeight(TNodeTypePtr pNode)
{
	if(pNode == NULL)
	{
		return -1;
	}

	return pNode->m_nHeight;

}

//左左情况下的旋转
template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::SingRotateLeft(TNodeTypePtr &pNode)
{
	TNodeTypePtr  pOrgParent = pNode->m_pParent;
    TNodeTypePtr pTempNode;
    pTempNode = pNode->m_pLeft;
    pNode->m_pLeft  = pTempNode->m_pRight;
    pTempNode->m_pRight = pNode;
	pNode->m_pParent = pTempNode;

    pNode->m_nHeight = Max(GetHeight(pNode->m_pLeft), GetHeight(pNode->m_pRight)) + 1;
    pTempNode->m_nHeight = Max(GetHeight(pTempNode->m_pLeft), GetHeight(pNode)) + 1;
	pNode = pTempNode;
	pNode->m_pParent = pOrgParent;
}
//右右情况下的旋转
template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::SingRotateRight(TNodeTypePtr &pNode)
{
	TNodeTypePtr  pOrgParent = pNode->m_pParent;
    TNodeTypePtr pTempNode;
    pTempNode = pNode->m_pRight;
    pNode->m_pRight = pTempNode->m_pLeft;
    pTempNode->m_pLeft = pNode;
	pNode->m_pParent = pTempNode;

    pNode->m_nHeight=Max(GetHeight(pNode->m_pLeft), GetHeight(pNode->m_pRight))+1;
    pTempNode->m_nHeight=Max(GetHeight(pTempNode->m_pRight), GetHeight(pNode))+1;

	pNode = pTempNode;
	pNode->m_pParent = pOrgParent;
}
//左右情况的旋转
template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::DoubleRotateLR(TNodeTypePtr &pNode)
{
    SingRotateRight(pNode->m_pLeft);

    SingRotateLeft(pNode);
}
//右左情况的旋转
template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::DoubleRotateRL(TNodeTypePtr &pNode)
{
    SingRotateLeft(pNode->m_pRight);

    SingRotateRight(pNode);
}

//插入接口
template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::Insert(TKey Key, TValue Value)
{
	TNodeTypePtr pNode = AllocNode();
	if(pNode == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	pNode->m_Key  = Key;
	pNode->m_Data = Value;

	if(m_pRoot == NULL)
	{
		m_pRoot = pNode;
		m_nCount += 1;
		pNode->m_pParent = NULL;

		return true;
	}

	return InsertInner(m_pRoot, pNode);
}

//插入
template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::InsertInner(TNodeTypePtr &pParentNode, TNodeTypePtr pInsertNode)
{
    if(pParentNode==NULL)
    {
		ASSERT(FALSE);
        return false;
    }

    if(pParentNode->m_Key > pInsertNode->m_Key)
    {
		if(pParentNode->m_pLeft != NULL)
		{
			if(!InsertInner(pParentNode->m_pLeft, pInsertNode))
			{
				ASSERT(FALSE);
				return false;
			}
		}
		else
		{
			pParentNode->m_pLeft = pInsertNode;
			pInsertNode->m_pParent = pParentNode;
			m_nCount += 1;
		}

        if((GetHeight(pParentNode->m_pLeft) - GetHeight(pParentNode->m_pRight)) >= 2)
		{
            if(pInsertNode->m_Key < pParentNode->m_pLeft->m_Key)
			{
				SingRotateLeft(pParentNode);
			}
            else
			{
				DoubleRotateLR(pParentNode);
			}
		}
    }
    else if(pParentNode->m_Key < pInsertNode->m_Key)
    {
		if(pParentNode->m_pRight != NULL)
		{
			if(!InsertInner(pParentNode->m_pRight, pInsertNode))
			{
				ASSERT(FALSE);
				return false;
			}
		}
		else
		{
			pParentNode->m_pRight = pInsertNode;
			pInsertNode->m_pParent = pParentNode;
			m_nCount += 1;
		}

        if((GetHeight(pParentNode->m_pRight) - GetHeight(pParentNode->m_pLeft)) >= 2)
		{
			if(pInsertNode->m_Key > pParentNode->m_pRight->m_Key)
			{
				SingRotateRight(pParentNode);
			}
            else
			{
				DoubleRotateRL(pParentNode);
			}
		}
    }
	else
	{
		//己经存在
		return false;
	}

    pParentNode->m_nHeight = Max(GetHeight(pParentNode->m_pLeft), GetHeight(pParentNode->m_pRight)) + 1;

	return true;
}


template<typename TKey, typename TValue>
 TreeNode<TKey, TValue>* AVLTree<TKey, TValue>::FindInner(TNodeTypePtr pNode, TKey Key)
{
    if(pNode==NULL)//如果节点为空说明没找到,返回NULL
    {
        return NULL;
    }
    if(pNode->m_Key > Key)//如果x小于节点的值,就继续在节点的左子树中查找x
    {
        return FindInner(pNode->m_pLeft, Key);
    }
    else if(pNode->m_Key< Key)//如果x大于节点的值,就继续在节点的左子树中查找x
    {
        return FindInner(pNode->m_pRight, Key);
    }
	else
	{
		return pNode;
	}
}
//查找接口
template<typename TKey,typename TValue>
 TreeNode<TKey, TValue>* AVLTree<TKey,TValue>::Find(TKey Key)
{
    return FindInner(m_pRoot, Key);
}
//删除
template<typename TKey, typename TValue>
bool AVLTree<TKey,TValue>::DeleteInner(TNodeTypePtr &pNode, TKey Key)
{
    if(pNode==NULL)
	{
		return false;
	}

    if(Key < pNode->m_Key)
    {
         if(!DeleteInner(pNode->m_pLeft, Key))
		 {
			 return false;
		 }

         if((GetHeight(pNode->m_pRight) - GetHeight(pNode->m_pLeft)) >= 2)
		 {
            if((pNode->m_pRight->m_pLeft != NULL)&&(GetHeight(pNode->m_pRight->m_pLeft) > GetHeight(pNode->m_pRight->m_pRight)) )
			{
				DoubleRotateRL(pNode);
			}
            else
			{
				SingRotateRight(pNode);
			}
		}
    }
    else if(Key > pNode->m_Key)
    {
         if(!DeleteInner(pNode->m_pRight,Key))
		 {
			 return false;
		 }

         if((GetHeight(pNode->m_pLeft)-GetHeight(pNode->m_pRight)) >= 2)
		 {
            if(pNode->m_pLeft->m_pRight!=NULL&& (GetHeight(pNode->m_pLeft->m_pRight)>GetHeight(pNode->m_pLeft->m_pLeft) ))
			{
				DoubleRotateLR(pNode);
			}
            else
			{
				SingRotateLeft(pNode);
			}
		}
    }
    else//如果相等,此节点就是要删除的节点
    {
        if(pNode->m_pLeft && pNode->m_pRight)//此节点有两个儿子
        {
           TNodeTypePtr pTempNode = pNode->m_pRight;//temp指向节点的右儿子
            while(pTempNode->m_pLeft != NULL)
			{
				pTempNode = pTempNode->m_pLeft;//找到右子树中值最小的节点
			}

            //把右子树中最小节点的值赋值给本节点
            pNode->m_Key  = pTempNode->m_Key;
			pNode->m_Data = pTempNode->m_Data;
            
			if(!DeleteInner(pNode->m_pRight, pTempNode->m_Key))//删除右子树中最小值的节点
			{
				return false;
			}

            if((GetHeight(pNode->m_pLeft)-GetHeight(pNode->m_pRight)) >= 2)
            {
                if(pNode->m_pLeft->m_pRight!=NULL&& (GetHeight(pNode->m_pLeft->m_pRight) > GetHeight(pNode->m_pLeft->m_pLeft) ))
				{
					DoubleRotateLR(pNode);
				}
                else
				{
					SingRotateLeft(pNode);
				}
            }
        }
        else//此节点有1个或0个儿子
        {
			TNodeTypePtr pOrgParentNode = pNode->m_pParent;
            TNodeTypePtr pTempNode = pNode;

            if(pNode->m_pLeft == NULL)//有右儿子或者没有儿子
			{
				pNode = pNode->m_pRight;
			}
            else if(pNode->m_pRight == NULL)//有左儿子
			{
				pNode = pNode->m_pLeft;
			}

			if(pNode != NULL)
			{
				pNode->m_pParent = pOrgParentNode;
			}

            FreeNode(pTempNode);
			m_nCount--;
        }
    }

	if(pNode==NULL) 
	{
		return false;
	}

    pNode->m_nHeight = Max(GetHeight(pNode->m_pLeft),GetHeight(pNode->m_pRight))+1;

    return true;
}


//删除接口
template<typename TKey, typename TValue>
bool AVLTree<TKey, TValue>::Delete(TKey Key)
{
    return DeleteInner(m_pRoot, Key);
}
//中序遍历函数
template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::Insubtree(TNodeTypePtr pNode)
{
    if(pNode == NULL)
	{
		return;
	}

    Insubtree(pNode->m_pLeft);//先遍历左子树
	DoEnumNode(pNode);
    Insubtree(pNode->m_pRight);//再遍历右子树
}
//中序遍历接口
template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::Traversal()
{
    Insubtree(m_pRoot);
}

template<typename TKey, typename TValue>
void AVLTree<TKey, TValue>::DoEnumNode( TNodeTypePtr pNode )
{
	
}

#endif
