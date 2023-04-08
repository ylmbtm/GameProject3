#ifndef __WS_PLAYER_OBJECT_H__
#define __WS_PLAYER_OBJECT_H__
#include "AVLTree.h"
#include "Position.h"
#include "ModuleBase.h"
#include "../Message/Msg_Game.pb.h"
#include "../ServerData/ServerDefine.h"
#include "../ServerData/ServerStruct.h"
#include "MsgHandlerManager.h"

class CPlayerObject : public CHandlerManager
{
public:
    CPlayerObject();

    ~CPlayerObject();

    BOOL        Init(UINT64 uRoleID);

    BOOL        Uninit();

    BOOL        OnCreate(UINT64 uRoleID);

    BOOL        OnDestroy();

    BOOL        OnLogin();

    BOOL        OnLogout();

    BOOL        OnNewDay();

    BOOL        ReadFromDBLoginData(DBRoleLoginAck& Ack);

    BOOL        SendMsgProtoBuf(INT32 nMsgID, const google::protobuf::Message& pdata);

    BOOL        SendMsgRawData(INT32 nMsgID, const char* pdata, UINT32 dwLen);

    //向玩家所在的场景服发消息
    BOOL        SendMsgToScene(INT32 nMsgID, const google::protobuf::Message& pdata);

    BOOL        ToTransferData(TransferDataItem* pTransItem);

    BOOL        NotifyTaskEvent(UINT32 dwEventID, UINT32 dwParam1, UINT32 dwParm2);

    BOOL        IsOnline();

    BOOL        SetOnline(BOOL bOnline);

    BOOL        NotifyChange();

public:
    //全部是操作方法
    BOOL        SendIntoSceneNotify(UINT32 dwCopyGuid, UINT32 dwCopyID, UINT32 dwSvrID);
    BOOL        SendLeaveScene(UINT32 dwCopyGuid, UINT32 dwSvrID);
    BOOL        SendRoleLoginAck();
    BOOL        SendPlayerChange(EChangeType eChangeType, UINT64 uIntValue1, UINT64 uIntValue2, std::string strValue);

    BOOL        SetConnectID(UINT32 dwProxyID, UINT32 dwClientID);

    BOOL        ClearCopyStatus();
    BOOL        SetCopyStatus(UINT32 dwCopyGuid, UINT32 dwCopyID, UINT32 dwCopySvrID, BOOL bMainCity);

    //模块函数
    BOOL            CreateAllModule();
    BOOL            DestroyAllModule();
    CModuleBase*    GetModuleByType(UINT32 dwModuleType);

public:
    UINT32          CheckCopyConditoin(UINT32 dwCopyID);

public:
    UINT64          GetRoleID();
    UINT64          GetAccountID();
    UINT32          GetCityCopyID();
    UINT32          GetActorID();
    CHAR*           GetName();
    UINT32          GetCarrerID();
    INT64           GetProperty(ERoleProperty ePropertyID);
    //////////////////////////////////////////////////////////////////////////
    //当前的多人排队情况

    UINT64      m_uRoomID;      //房间ID

    UINT64      GetRoomID();

    VOID        SetRoomID(UINT64 uRoomID);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //计算角色，宠物， 伙伴的战斗属性
    BOOL        CalcFightDataInfo();

    INT32       m_Propertys[PROPERTY_NUM];

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //背包方法
    //////////////////////////////////////////////////////////////////////////
public:
    //网络消息泵

    UINT64          m_uRoleID;
    INT32           m_nProxyConnID;
    INT32           m_nClientConnID;
    BOOL            m_IsOnline;
    std::vector<CModuleBase*> m_MoudleList;

public:
    UINT32      m_dwCopyGuid;   //当前的副本实例ID
    UINT32      m_dwCopyID;     //当前的副本类型
    UINT32      m_dwCopySvrID;  //副本服务器的ID
    BOOL        m_bMainCity;    //是否在主城中




public:
    //*********************消息处理定义开始******************************
    //*********************消息处理定义结束******************************
};


#endif //__WS_PLAYER_OBJECT_H__
