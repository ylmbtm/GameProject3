#ifndef __BAG_MODULE_H__
#define __BAG_MODULE_H__

class CBagModule : public CModuleBase
{
public:
	CBagModule();

	~CBagModule();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual UINT32	WriteToBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag);

public:


};

#endif //__BAG_MODULE_H__
