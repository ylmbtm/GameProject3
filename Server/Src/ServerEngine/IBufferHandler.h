#ifndef __IBUFFER_HANDLER_H__
#define __IBUFFER_HANDLER_H__
struct NetPacket;

class IDataBuffer
{
public:
    virtual BOOL    AddRef() = 0;

    virtual BOOL    Release() = 0;

    virtual CHAR*   GetData() = 0;

    virtual INT32   GetTotalLenth() = 0;

    virtual VOID    SetTotalLenth(INT32 nPos) = 0;

    virtual INT32   GetBodyLenth() = 0;

    virtual CHAR*   GetBuffer() = 0;

    virtual INT32   GetBufferSize() = 0;

    virtual INT32   CopyFrom(IDataBuffer* pSrcBuffer) = 0;

    virtual INT32   CopyTo(CHAR* pDestBuf, INT32 nDestLen) = 0;
};

struct IDataHandler
{
    virtual BOOL OnDataHandle( IDataBuffer* pDataBuffer, INT32 nConnID) = 0;
    virtual BOOL OnCloseConnect(INT32 nConnID) = 0;
    virtual BOOL OnNewConnect(INT32 nConnID) = 0;
};

struct IPacketDispatcher
{
    virtual BOOL DispatchPacket( NetPacket* pNetPacket) = 0;
    virtual BOOL OnCloseConnect(INT32 nConnID) = 0;
    virtual BOOL OnNewConnect(INT32 nConnID) = 0;
    virtual BOOL OnSecondTimer() = 0;
};

struct NetPacket
{
    NetPacket(INT32 nConnID = 0, IDataBuffer* pBuffer = NULL, INT32 nMsgID = 0 )
    {
        m_nConnID = nConnID;

        m_pDataBuffer = pBuffer;

        m_nMsgID = nMsgID;
    }

    INT32        m_nMsgID;
    INT32        m_nConnID;
    IDataBuffer* m_pDataBuffer;
};




#endif /* __IBUFFER_HANDLER_H__ */
