#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_
#include "Utility/Position.h"
#include "../Message/Msg_Move.pb.h"

enum  OBJECT_STATE
{
	BS_Static = 1,  ///停
	BS_Walk = 2,    ///移动
	BS_DIE = 3,   ///死亡
};

class CScene;
class CSceneObject
{
public:
	CSceneObject();

	~CSceneObject();
public:

	BOOL			SendMsgProtoBuf(UINT32 dwMsgID, const google::protobuf::Message& pdata);
	BOOL			SendMsgRawData(UINT32 dwMsgID, const char * pdata,UINT32 dwLen);
    BOOL			SetConnectID(UINT32 dwProxyID, UINT32 dwClientID);
	BOOL			OnUpdate(UINT32 dwTick);

public:
    UINT32          GetHp();
    VOID            AddHp(UINT32 dwValue);
    VOID            SubHp(UINT32 dwValue);

    UINT64          GetObjectID();
	UINT32			GetType();

	BOOL			IsConnected();

	BOOL            IsEnterCopy();
	VOID			SetEnterCopy();
    BOOL            IsDie();

    BOOL            SaveNewObject(ObjectNewNty &Nty);
    BOOL            SaveUpdateObject(ObjectUpdateNty &Nty);

    VOID            SetChanged();

public:
	//对象成员
    //////////////////////////////////////////////////////////
	UINT64          m_uID;			    //ID
	UINT32          m_dwType;		    //对象Type 1:玩家，2: 3:
	UINT32          m_dwObjType;		//对象实例type,玩家就是职业，怪就是怪的静态ID
	std::string     m_strName;          //对象的名字
    
    //对象的连接情况，仅玩家有效
    ////////////////////////////////////////////////////////////////
	UINT32          m_dwProxyConnID;
	UINT32          m_dwClientConnID;


    //对象的基本信息
    ////////////////////////////////////////////////////////////////
	FLOAT			x, z;			//对象坐标
	FLOAT           vx,vz;			//对象方向
    UINT32          m_dwObjState;	//对象当前的状态
    UINT32          m_dwHp;			//对象的当前血量
    UINT32          m_dwProperty[11]; //11个属性的数值



    //对象的一些标记
    CScene			*m_pScene;  //所在场景
    BOOL            m_bEnter;   //玩家是否己经进入副本
    BOOL            m_bChanged; //玩家状态是否发生改变
    
	//对象的副要结果
	////////////////////////////////////////////////////////////////
	UINT32			m_dwResult;   //0:未结算，1:胜利，2: 失败 3: 平局
};


#endif //_SCENE_OBJECT_H_
