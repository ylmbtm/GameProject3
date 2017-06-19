#ifndef _CHAR_OBJECT_H_
#define _CHAR_OBJECT_H_
#include "WorldObject.h"
#include "GameDefine.h"
#include "GameStruct.h"

class CCharObject : public CWorldObject
{
public:
	CCharObject();

	~CCharObject();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual UINT32	WriteToBuffer(UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(UINT32 dwChangeFlag);

	char*			GetObjectName();

	void			SetObjectName(char *szName);

	char			m_szObjectName[MAX_NAME_LEN];

	St_ObjectStatus m_ObjectStatus;//角色状态
};

#endif //_CHAR_OBJECT_H_
