#ifndef __BUFF_OBJECT_H__
#define __BUFF_OBJECT_H__

class CSceneObject;

class CBuffObject
{
public:
	CBuffObject(CSceneObject *pObject, UINT32 dwBuffID);

	~CBuffObject();

public:
	BOOL OnAddBuff();
	BOOL OnRemoveBuff();
	BOOL OnEffect();
    BOOL OnUpdate(UINT32 dwData);

public:
	UINT32  m_dwBuffID;    //buffID
	UINT32  m_dwBuffType;
	UINT32  m_dwEffTime;   //作用时间

	CSceneObject* m_pSceneObject;

};

#endif //__BUFF_OBJECT_H__
