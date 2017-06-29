#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
#include "WorldObject.h"

class CSceneObject : public CWorldObject
{
public:
	CSceneObject();

	~CSceneObject();
public:
	virtual UINT32	WriteToBuffer( UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(UINT32 dwChangeFlag);

	BOOL  SendProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata);
public:
	BOOL			OnUpdate(UINT32 dwTick);

public:
	BOOL            StartSkill();

};


#endif //_SCENE_OBJECT_H_
