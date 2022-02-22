#ifndef _MSG_HANDLER_MANAGER_H__
#define _MSG_HANDLER_MANAGER_H__

class CHandlerBase
{
public:
    virtual ~CHandlerBase() {}

    virtual BOOL operator()(void* pdata) = 0;

    virtual VOID* GetThisAddr() = 0;

    virtual VOID EmptyThisAddr() = 0;
};

template<typename TClass, typename TParam>
class CHandlerSlot : public CHandlerBase
{
    typedef BOOL (TClass::*FuncType)(TParam*);
public:
    CHandlerSlot(BOOL (TClass::*FuncType)(TParam*), TClass* pObj)
        : m_pFuncPtr(FuncType), m_pThis(pObj)
    {
    }
    virtual ~CHandlerSlot()
    {
    }

    virtual BOOL operator() (void* pData)
    {
        if (m_pThis != NULL && m_pFuncPtr != NULL)
        {
            (m_pThis->*m_pFuncPtr)(reinterpret_cast<TParam*>(pData));
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual VOID* GetThisAddr()
    {
        return reinterpret_cast<VOID*>(m_pThis);
    }

    virtual void EmptyThisAddr()
    {
        m_pThis = NULL;
    }


private:
    FuncType m_pFuncPtr;
    TClass*     m_pThis;
};

class CHandlerManager
{
public:
    CHandlerManager()
    {
    }

    ~CHandlerManager()
    {
        ClearAllHandler();
    }

    template<typename TClass, typename TParam>
    bool RegisterMessageHandle(int nMsgID, BOOL (TClass::*FuncPtr)(TParam*), TClass* pObj)
    {
        CHandlerBase* pSlot = new CHandlerSlot<TClass, TParam>(FuncPtr, pObj);
        if (pSlot == NULL)
        {
            return false;
        }

        std::vector<CHandlerBase*>* pMsgVector = NULL;
        std::map<int, std::vector<CHandlerBase* >* >::iterator it = m_mapHandler.find(nMsgID);
        if (it == m_mapHandler.end())
        {
            pMsgVector = new std::vector<CHandlerBase*>;
            pMsgVector->push_back(pSlot);
            m_mapHandler.insert(std::make_pair(nMsgID, pMsgVector));
        }
        else
        {
            pMsgVector = it->second;
            pMsgVector->push_back(pSlot);
        }

        return true;
    }

    template<typename T>
    bool UnregisterMessageHandle(int nMsgID, T* pObj)
    {
        std::map<int, std::vector<CHandlerBase* >* >::iterator itor = m_mapHandler.find(nMsgID);
        if (itor == m_mapHandler.end())
        {
            return true;
        }

        std::vector<CHandlerBase* >* vtHandler = itor->second;

        for (std::vector<CHandlerBase* >::iterator itor = vtHandler->begin(); itor != vtHandler->end(); ++itor)
        {
            if ((*itor)->GetThisAddr() != reinterpret_cast<void*>(pObj))
            {
                continue;
            }

            //反注册并不真实删除，只是将this指针置空
            (*itor)->EmptyThisAddr();
        }
        return true;
    }
    template<typename TParam>
    bool FireMessage(int nMsgID, TParam* pData)
    {
        std::map<int, std::vector<CHandlerBase* >* >::iterator itor = m_mapHandler.find(nMsgID);
        if (itor == m_mapHandler.end())
        {
            return false;
        }

        std::vector<CHandlerBase* >* vtHandler = itor->second;

        for (std::vector<CHandlerBase* >::iterator itorHandler = vtHandler->begin(); itorHandler != vtHandler->end(); itorHandler++)
        {
            if ((*itorHandler)->GetThisAddr() != 0)
            {
                (**itorHandler)((void*)pData);
            }
        }
        return true;
    }


    bool ClearAllHandler()
    {
        for (std::map<int, std::vector<CHandlerBase*>*>::iterator itor = m_mapHandler.begin(); itor != m_mapHandler.end(); itor++)
        {
            std::vector<CHandlerBase* >* pVtHandler = itor->second;
            for (std::vector<CHandlerBase* >::iterator itor2 = pVtHandler->begin(); itor2 != pVtHandler->end(); itor2++)
            {
                delete *itor2;
            }

            pVtHandler->clear();

            delete pVtHandler;
        }

        m_mapHandler.clear();

        return true;
    }

protected:
    std::map<int, std::vector<CHandlerBase* >* > m_mapHandler;
};

class CMsgHandlerManager : public CHandlerManager
{
private:
    CMsgHandlerManager(void) {};
    virtual ~CMsgHandlerManager(void) {};

public:
    static CMsgHandlerManager* GetInstancePtr()
    {
        static CMsgHandlerManager _Instance;

        return &_Instance;
    }
};

#endif // _MSG_HANDLER_MANAGER_H__
