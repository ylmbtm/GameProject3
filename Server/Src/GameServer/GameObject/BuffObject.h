#ifndef __BUFF_OBJECT_H__
#define __BUFF_OBJECT_H__

#include "../ServerData/ServerDefine.h"

class CSceneObject;
struct StBuffInfo;

class CBuffObject
{
public:
	CBuffObject(CSceneObject* pObject, UINT32 dwBuffID);

	~CBuffObject();

public:
	BOOL OnAddBuff();
	BOOL OnRemoveBuff();
	BOOL OnEffect();
	BOOL OnUpdate(UINT32 dwData);

public:
	UINT32  m_dwBuffID;    //buffID
	UINT32  m_dwStartTime; //开始时间
	UINT32  m_dwLastTime;  //上次作用时间
	INT32   m_PtyChange[MAX_PROPERTY_NUM];
	CSceneObject* m_pSceneObject;
	StBuffInfo*   m_pBuffInfo;

};

#endif //__BUFF_OBJECT_H__
