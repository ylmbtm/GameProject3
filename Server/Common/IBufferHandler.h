#ifndef __IBUFFER_HANDLER_H__
#define __IBUFFER_HANDLER_H__
class CConnection;
class CBufferHelper;
struct NetPacket;

class IDataBuffer
{
public:
	virtual INT32	GetRef() = 0;

	virtual bool	AddRef() = 0;

	virtual bool	Release() = 0;

	virtual CHAR*	GetData() = 0;

	virtual UINT32	GetTotalLenth() = 0;

	virtual VOID	SetTotalLenth(UINT32 nPos) = 0;

	virtual UINT32  GetBodyLenth() = 0;

	virtual CHAR*	GetBuffer() = 0;

	virtual UINT32	GetBufferSize() = 0;

	virtual UINT32  CopyFrom(IDataBuffer *pSrcBuffer) = 0;

	virtual UINT32  CopyTo(CHAR *pDestBuf, UINT32 dwDestLen) = 0;
};

struct IDataHandler
{
	virtual BOOL OnDataHandle( IDataBuffer *pDataBuffer, CConnection *pConnection) = 0;
	virtual BOOL OnCloseConnect(CConnection *pConnection) = 0;
	virtual BOOL OnNewConnect(CConnection *pConnection) = 0;
};

struct IPacketDispatcher
{
	virtual BOOL DispatchPacket( NetPacket *pNetPacket) = 0;
	virtual BOOL OnCloseConnect(CConnection *pConnection) = 0;
	virtual BOOL OnNewConnect(CConnection *pConnection) = 0;
};

struct  IThreadCommandHandler
{
	virtual BOOL OnCommandHandle(UINT32 dwMsgID, UINT64 u64ConnID, IDataBuffer *pDataBuffer) = 0;
	virtual BOOL OnUpdate(UINT32 dwTick) = 0;
	virtual BOOL OnThreadBegin() = 0;
	virtual BOOL OnThreadEnd() = 0;
};






#endif /* __IBUFFER_HANDLER_H__ */
