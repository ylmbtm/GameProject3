#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_
#include "IBufferHandler.h"

template <int SIZE>
class  CBufferManager;

#define HEADER_LEN 28

template <int SIZE>
class CDataBuffer : public IDataBuffer
{
public:
    CDataBuffer(void)
    {
        m_nDataLen      = 0;
        m_nBufSize      = SIZE;
        m_nRefCount     = 0;
        m_pPrev         = NULL;
        m_pNext         = NULL;
        m_pManager      = NULL;
    }

    virtual ~CDataBuffer(void)
    {
        m_nDataLen = 0;
        m_nBufSize = SIZE;
        m_nRefCount = 0;
        m_pPrev = NULL;
        m_pNext = NULL;
        m_pManager = NULL;
    }

    BOOL AddRef()
    {
        m_pManager->m_BuffMutex.lock();
        m_nRefCount++;
        m_pManager->m_BuffMutex.unlock();
        return TRUE;
    }

    BOOL Release()
    {
        assert(m_pManager != NULL);

        m_pManager->ReleaseDataBuff(this);

        return TRUE;
    }

    CHAR* GetData()
    {
        return m_Buffer + HEADER_LEN;
    }

    INT32 GetTotalLenth()
    {
        return m_nDataLen;
    }

    INT32 GetBodyLenth()
    {
        return m_nDataLen - HEADER_LEN;
    }

    VOID SetTotalLenth(INT32 nPos)
    {
        m_nDataLen = nPos;
    }

    CHAR* GetBuffer()
    {
        return m_Buffer;
    }

    INT32 GetBufferSize()
    {
        return m_nBufSize;
    }

    INT32  CopyFrom(IDataBuffer* pSrcBuffer)
    {
        memcpy(m_Buffer, pSrcBuffer->GetBuffer(), pSrcBuffer->GetTotalLenth());

        m_nDataLen = pSrcBuffer->GetTotalLenth();

        return m_nDataLen;
    }

    INT32  CopyTo(CHAR* pDestBuf, INT32 nDestLen)
    {
        if(nDestLen < GetTotalLenth())
        {
            return 0;
        }

        memcpy(pDestBuf, GetBuffer(), GetTotalLenth());

        return nDestLen;
    }

    CDataBuffer<SIZE>* m_pPrev;

    CDataBuffer<SIZE>* m_pNext;

    CBufferManager<SIZE>* m_pManager;

public:
    INT32       m_nRefCount;

    INT32       m_nBufSize;

    CHAR        m_Buffer[SIZE];

    INT32       m_nDataLen;
};

template <int SIZE>
class  CBufferManager
{
public:
    CBufferManager()
    {
        m_pUsedList = NULL;
        m_pFreeList = NULL;
        m_nBufferCount = 0;
        m_EnablePool = TRUE;
    }

    ~CBufferManager()
    {
        ReleaseAll();
    }

    IDataBuffer* AllocDataBuff()
    {
        m_BuffMutex.lock();
        CDataBuffer<SIZE>* pDataBuffer = NULL;
        if(m_pFreeList == NULL)
        {
            pDataBuffer = new CDataBuffer<SIZE>();
            pDataBuffer->m_pManager = this;
        }
        else
        {
            pDataBuffer = m_pFreeList;

            m_pFreeList = m_pFreeList->m_pNext;

            if(m_pFreeList != NULL)
            {
                m_pFreeList->m_pPrev = NULL;
            }

            pDataBuffer->m_pNext = NULL;
            pDataBuffer->m_pPrev = NULL;
        }

        assert(pDataBuffer->m_nRefCount == 0);

        pDataBuffer->m_nRefCount = 1;

        if(m_pUsedList == NULL)
        {
            m_pUsedList = pDataBuffer;
        }
        else
        {
            pDataBuffer->m_pNext = m_pUsedList;
            m_pUsedList->m_pPrev = pDataBuffer;
            pDataBuffer->m_pPrev = NULL;
            m_pUsedList = pDataBuffer;
        }

        m_nBufferCount += 1;
        m_BuffMutex.unlock();
        return pDataBuffer;
    }

    BOOL ReleaseDataBuff(CDataBuffer<SIZE>* pBuff)
    {
        assert(pBuff != NULL);
        if (pBuff == NULL)
        {
            return FALSE;
        }

        assert(pBuff->m_nRefCount > 0);
        if (pBuff->m_nRefCount <= 0)
        {
            return FALSE;
        }
        std::lock_guard<std::mutex> lock(m_BuffMutex);
        pBuff->m_nRefCount--;

        if (pBuff->m_nRefCount <= 0)
        {
            pBuff->m_nDataLen = 0;
            //首先从己用中删除
            if (m_pUsedList == pBuff)
            {
                //自己是首结点
                m_pUsedList = pBuff->m_pNext;
                if (m_pUsedList != NULL)
                {
                    m_pUsedList->m_pPrev = NULL;
                }
            }
            else
            {
                assert(pBuff->m_pPrev != NULL);
                pBuff->m_pPrev->m_pNext = pBuff->m_pNext;
                if (pBuff->m_pNext != NULL)
                {
                    pBuff->m_pNext->m_pPrev = pBuff->m_pPrev;
                }
            }

            if (m_EnablePool)
            {
                //再把自己加到己用中
                pBuff->m_pNext = m_pFreeList;
                pBuff->m_pPrev = NULL;
                m_pFreeList = pBuff;

                if (pBuff->m_pNext != NULL)
                {
                    pBuff->m_pNext->m_pPrev = pBuff;
                }
            }
            else
            {
                delete pBuff;
            }
            m_nBufferCount--;
        }
        return TRUE;
    }

    void ReleaseAll()
    {
        CDataBuffer<SIZE>* pBufferNode = m_pFreeList;
        while (pBufferNode)
        {
            CDataBuffer<SIZE>* pTempNode = pBufferNode;
            pBufferNode = pTempNode->m_pNext;
            delete pTempNode;
        }

        pBufferNode = m_pUsedList;
        while (pBufferNode)
        {
            CDataBuffer<SIZE>* pTempNode = pBufferNode;
            pBufferNode = pTempNode->m_pNext;
            delete pTempNode;
        }

        return;
    }

    VOID SetEnablePool(BOOL bEnablePool)
    {
        m_EnablePool = bEnablePool;
    }

    void PrintOutList(CDataBuffer<SIZE>* pList)
    {
        INT32 nCount = 0;
        CDataBuffer<SIZE>* pBufferNode = pList;
        if(pBufferNode == NULL)
        {
            return ;
        }

        BOOL bNext = TRUE;
        while(pBufferNode)
        {
            if(bNext)
            {
                nCount++;
                if(pBufferNode->m_pNext != NULL)
                {
                    pBufferNode = pBufferNode->m_pNext;
                }
                else
                {
                    bNext = FALSE;
                    pBufferNode = pBufferNode->m_pPrev;
                }
            }
            else
            {
                nCount++;
                pBufferNode = pBufferNode->m_pPrev;
            }
        }

        return ;
    }

    CDataBuffer<SIZE>* m_pFreeList;

    CDataBuffer<SIZE>* m_pUsedList;

    std::mutex  m_BuffMutex;

    INT32       m_nBufferCount;

    BOOL        m_EnablePool;
private:
};

class CBufferAllocator
{
    CBufferAllocator();
    ~CBufferAllocator();
public:
    static CBufferAllocator* GetInstancePtr();

public:
    IDataBuffer* AllocDataBuff(int nSize);

    CBufferManager<64>     m_BufferManager64B;      //管理<=64B的内存池，
    CBufferManager<128>    m_BufferManager128B;     //管理<=128B的内存池，
    CBufferManager<256>    m_BufferManager256B;     //管理<=256B的内存池，
    CBufferManager<512>    m_BufferManager512B;     //管理<=512B的内存池，
    CBufferManager<1024>   m_BufferManager1K;       //管理<=1k的内存池，
    CBufferManager<2048>   m_BufferManager2K;       //管理<=2k的内存池，
    CBufferManager<4096>   m_BufferManager4K;       //管理<=4k的内存池，
    CBufferManager<8192>   m_BufferManager8K;       //管理<=8k的内存池，
    CBufferManager<16384>  m_BufferManager16K;      //管理<=16k的内存池，
    CBufferManager<32768>  m_BufferManager32K;      //管理<=32k的内存池，
    CBufferManager<65536>  m_BufferManager64K;      //管理<=64k的内存池，

    CBufferManager<10 * 1024 * 1014> m_BufferManagerAny;        //管理<=10M的内存, 并不用池管理, 直接申请, 直接释放.
};

#endif