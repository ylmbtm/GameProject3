#ifndef _OBJECT_H_
#define _OBJECT_H_
class CBufferHelper;

class CGObject
{
public:
	CGObject();

	~CGObject();

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);

	UINT8  GetObjectType();

	UINT64	GetObjectID();

	VOID	SetObjectID(UINT64 u64ObjectID);

protected:

	UINT64 m_u64ObjectID;	
};

#endif //_OBJECT_H_
