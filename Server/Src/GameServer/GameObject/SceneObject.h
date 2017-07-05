#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
#include "WorldObject.h"

class CSceneObject : public CWorldObject
{
public:
	CSceneObject();

	~CSceneObject();
public:

	BOOL			SendProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata);
public:
	BOOL			OnUpdate(UINT32 dwTick);

public:
	BOOL            StartSkill();

	BOOL			SetConnectID(UINT32 dwProxyID, UINT32 dwClientID);
public:
	//对象成员
	UINT64 m_uID;			//ID
	UINT32 m_dwType;		//对象Type 1:玩家，2: 3:
	UINT32 m_dwObjType;		//对象实例type
	std::string m_strName;

	UINT32      m_dwProxyConnID;
	UINT32      m_dwClientConnID;

};


#endif //_SCENE_OBJECT_H_
