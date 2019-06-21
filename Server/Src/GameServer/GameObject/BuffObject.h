#ifndef __BUFF_OBJECT_H__
#define __BUFF_OBJECT_H__

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
	BOOL OnEffect(BOOL bFirst = FALSE);
	BOOL OnUpdate(UINT64 uTick);
	BOOL IsOver();
	VOID SetOver();

public:
	UINT32  m_dwBuffID;    //buffID
	UINT32  m_dwStartTime; //开始时间
	UINT32  m_dwLastTime;  //上次作用时间
	BOOL	m_bOver;
	INT32   m_PtyChange[PROPERTY_NUM];
	CSceneObject* m_pSceneObject;
	StBuffInfo*   m_pBuffInfo;

};

#endif //__BUFF_OBJECT_H__
