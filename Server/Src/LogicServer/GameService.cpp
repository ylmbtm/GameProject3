#include "stdafx.h"
#include "GameService.h"

#include "DataPool.h"
#include "StaticData.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "../Message/Msg_RetCode.pb.h"
#include "../Message/Msg_ID.pb.h"

#include "TimerManager.h"
#include "SimpleManager.h"
#include "GlobalDataMgr.h"
#include "MailManager.h"
#include "PayManager.h"
#include "GuildManager.h"
#include "ActivityManager.h"
#include "RankMananger.h"
#include "MsgHandlerManager.h"
#include "PacketHeader.h"
#include "TeamCopyMgr.h"
#include "WebCommandMgr.h"
#include "WatcherClient.h"
#include "SealManager.h"

//#include "LuaManager.h"
//#include "Lua_Script.h"

CGameService::CGameService(void)
{
    m_dwLogConnID    = 0;
    m_dwLoginConnID  = 0;
    m_dwDBConnID     = 0;
    m_dwCenterConnID = 0;   //中心服的连接ID
    m_uSvrOpenTime   = 0;
    m_dwDbErrorCount = 0;
    m_bRegSuccessed  = FALSE;
}

CGameService::~CGameService(void)
{
    m_dwLogConnID   = 0;
    m_dwLoginConnID = 0;
    m_dwDBConnID    = 0;
    m_dwCenterConnID  = 0;   //中心服的连接ID
    m_uSvrOpenTime  = 0;
    m_dwDbErrorCount = 0;
    m_bRegSuccessed = FALSE;
}

CGameService* CGameService::GetInstancePtr()
{
    static CGameService _GameService;

    return &_GameService;
}

UINT32 CGameService::GetLogSvrConnID()
{
    return m_dwLogConnID;
}

VOID CGameService::RegisterMessageHanler()
{
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_LOGIC_REGTO_LOGIN_ACK, &CGameService::OnMsgRegToLoginAck, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_LOGIC_UPDATE_ACK,      &CGameService::OnMsgUpdateInfoAck, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_DB_WRITE_ERROR_NTY,    &CGameService::OnMsgDBWriteErrorNty, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_LOGIC_REGTO_CENTER_ACK, &CGameService::OnMsgRegToCenterAck, this);
    CMsgHandlerManager::GetInstancePtr()->RegisterMessageHandle(MSG_LOGIC_REGTO_DBSVR_ACK, &CGameService::OnMsgRegToDBSvrAck, this);
}

UINT64 CGameService::GetSvrOpenTime()
{
    return m_uSvrOpenTime;
}

BOOL CGameService::Init()
{
    CommonFunc::SetCurrentWorkDir("");

    if(!CLog::GetInstancePtr()->Start("LogicServer", "log"))
    {
        return FALSE;
    }

    CLog::GetInstancePtr()->LogInfo("---------服务器开始启动--------");

    if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
    {
        CLog::GetInstancePtr()->LogError("加载 servercfg.ini文件失败!");
        return FALSE;
    }

    if (CommonFunc::IsAlreadyRun("LogicServer" + CConfigFile::GetInstancePtr()->GetStringValue("areaid")))
    {
        CLog::GetInstancePtr()->LogError("LogicServer己经在运行!");
        return FALSE;
    }

    CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("logic_log_level"));

    UINT16 nPort = CConfigFile::GetInstancePtr()->GetRealNetPort("logic_svr_port");
    if (nPort <= 0)
    {
        CLog::GetInstancePtr()->LogError("配制文件logic_svr_port配制错误!");
        return FALSE;
    }

    std::string strListenIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");

    INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_max_con");
    if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this, strListenIp))
    {
        CLog::GetInstancePtr()->LogError("启动服务失败!");
        return FALSE;
    }

    if (!CDataPool::GetInstancePtr()->InitDataPool())
    {
        CLog::GetInstancePtr()->LogError("初始化共享内存池失败!");
        return FALSE;
    }

    ///////////////////////////////////
    //服务器启动之前需要加载的数据
    if (!CStaticData::GetInstancePtr()->LoadConfigData("Config.db"))
    {
        CLog::GetInstancePtr()->LogError("加载静态配制数据失败!");
        return FALSE;
    }

    //if (!CLuaManager::GetInstancePtr()->Init())
    //{
    //  CLog::GetInstancePtr()->LogError("初始化Lua环境失败!");
    //  return FALSE;
    //}

    //if (!luaopen_LuaScript(CLuaManager::GetInstancePtr()->GetLuaState()))
    //{
    //  CLog::GetInstancePtr()->LogError("导出Lua接口失败!");
    //  return FALSE;
    //}

    //if (!CLuaManager::GetInstancePtr()->LoadAllLua(".\\Lua"))
    //{
    //  CLog::GetInstancePtr()->LogError("加载lua代码失败!");
    //  return FALSE;
    //}

    ///////////////////////////////////
    //服务器启动之前需要加载的数据
    std::string strHost = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_ip");
    nPort = CConfigFile::GetInstancePtr()->GetIntValue("mysql_game_svr_port");
    std::string strUser = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_user");
    std::string strPwd = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_pwd");
    std::string strDb = CConfigFile::GetInstancePtr()->GetStringValue("mysql_game_svr_db_name");

    CppMySQL3DB tDBConnection;
    if(!tDBConnection.open(strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDb.c_str(), nPort))
    {
        CLog::GetInstancePtr()->LogError("Error: Can not open mysql database! Reason:%s", tDBConnection.GetErrorMsg());
        return FALSE;
    }

    BOOL bRet = FALSE;

    bRet = CGlobalDataManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet);

    bRet = CSimpleManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet)

    bRet = CMailManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet)

    bRet = CGuildManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet)

    bRet = CActivityManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet)

    bRet = CRankManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet)

    bRet = CGameSvrMgr::GetInstancePtr()->Init();
    ERROR_RETURN_FALSE(bRet);

    bRet = CTeamCopyMgr::GetInstancePtr()->Init();
    ERROR_RETURN_FALSE(bRet);

    bRet = CPayManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet);

    bRet = CSealManager::GetInstancePtr()->LoadData(tDBConnection);
    ERROR_RETURN_FALSE(bRet);

    bRet = CWebCommandMgr::GetInstancePtr()->Init();
    ERROR_RETURN_FALSE(bRet);

    bRet = m_LogicMsgHandler.Init(0);
    ERROR_RETURN_FALSE(bRet);

    RegisterMessageHanler();

    CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");

    return TRUE;
}

BOOL CGameService::Uninit()
{
    CLog::GetInstancePtr()->LogError("==========服务器开始关闭=======================");

    ServiceBase::GetInstancePtr()->StopNetwork();

    m_LogicMsgHandler.Uninit();

    CDataPool::GetInstancePtr()->ReleaseDataPool();

    google::protobuf::ShutdownProtobufLibrary();

    CLog::GetInstancePtr()->LogError("==========服务器关闭完成=======================");

    return TRUE;
}

BOOL CGameService::Run()
{
    while (CWatcherClient::GetInstancePtr()->IsRun())
    {
        ServiceBase::GetInstancePtr()->Update();

        m_LogicMsgHandler.OnUpdate(CommonFunc::GetTickCount());

        ServiceBase::GetInstancePtr()->FixFrameNum(200);
    }

    return TRUE;
}

BOOL CGameService::SendCmdToDBConnection(INT32 nMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
{
    ERROR_RETURN_FALSE(m_dwDBConnID != 0);
    ERROR_RETURN_FALSE(ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwDBConnID, nMsgID, u64TargetID, dwUserData, pdata));
    return TRUE;
}


BOOL CGameService::ConnectToLogServer()
{
    if (m_dwLogConnID != 0)
    {
        return TRUE;
    }
    INT32 nLogPort = CConfigFile::GetInstancePtr()->GetRealNetPort("log_svr_port");
    ERROR_RETURN_FALSE(nLogPort > 0);
    std::string strLogIp = CConfigFile::GetInstancePtr()->GetStringValue("log_svr_ip");
    CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strLogIp, nLogPort);
    ERROR_RETURN_FALSE(pConnection != NULL);

    m_dwLogConnID = pConnection->GetConnectionID();
    return TRUE;
}

BOOL CGameService::ConnectToLoginSvr()
{
    if (m_dwLoginConnID != 0)
    {
        return TRUE;
    }
    INT32 nLoginPort = CConfigFile::GetInstancePtr()->GetIntValue("login_svr_port");
    ERROR_RETURN_FALSE(nLoginPort > 0);
    std::string strLoginIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");
    CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strLoginIp, nLoginPort);
    ERROR_RETURN_FALSE(pConnection != NULL);
    m_dwLoginConnID = pConnection->GetConnectionID();
    return TRUE;
}

BOOL CGameService::ConnectToDBSvr()
{
    if (m_dwDBConnID != 0)
    {
        return TRUE;
    }
    INT32 nDBPort = CConfigFile::GetInstancePtr()->GetRealNetPort("db_svr_port");
    ERROR_RETURN_FALSE(nDBPort > 0);
    std::string strDBIp = CConfigFile::GetInstancePtr()->GetStringValue("db_svr_ip");
    CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strDBIp, nDBPort);
    ERROR_RETURN_FALSE(pConnection != NULL);
    m_dwDBConnID = pConnection->GetConnectionID();
    return TRUE;
}


BOOL CGameService::ConnectToCenterSvr()
{
    if (m_dwCenterConnID != 0)
    {
        return TRUE;
    }
    INT32 nCenterPort = CConfigFile::GetInstancePtr()->GetIntValue("center_svr_port");
    ERROR_RETURN_FALSE(nCenterPort > 0);
    std::string strCenterIp = CConfigFile::GetInstancePtr()->GetStringValue("center_svr_ip");
    CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strCenterIp, nCenterPort);
    ERROR_RETURN_FALSE(pConnection != NULL);
    m_dwCenterConnID = pConnection->GetConnectionID();
    return TRUE;
}

BOOL CGameService::RegisterToLoginSvr()
{
    LogicRegToLoginReq Req;
    UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
    std::string strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("areaname");
    UINT32 dwPort  = CConfigFile::GetInstancePtr()->GetRealNetPort("proxy_svr_port");
    UINT32 dwHttpPort = CConfigFile::GetInstancePtr()->GetRealNetPort("logic_svr_port");
    std::string strIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
    Req.set_serverid(dwServerID);
    Req.set_serverport(dwPort);
    Req.set_svrinnerip(strIp);
    Req.set_servername(strSvrName);
    Req.set_httpport(dwHttpPort);
    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLoginConnID, MSG_LOGIC_REGTO_LOGIN_REQ, 0, 0, Req);
}

BOOL CGameService::RegisterToDBSvr()
{
    LogicRegToDbSvrReq Req;
    UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
    std::string strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("areaname");
    Req.set_serverid(dwServerID);
    Req.set_servername(strSvrName);
    Req.set_processid(CommonFunc::GetCurProcessID());
    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwDBConnID, MSG_LOGIC_REGTO_DBSVR_REQ, 0, 0, Req);
}

BOOL CGameService::RegisterToCenterSvr()
{
    SvrRegToSvrReq Req;
    UINT32 dwServerID = CConfigFile::GetInstancePtr()->GetIntValue("areaid");
    std::string strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("areaname");
    UINT32 dwPort  = CConfigFile::GetInstancePtr()->GetRealNetPort("logic_svr_port");
    std::string strIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
    Req.set_serverid(dwServerID);
    Req.set_serverport(dwPort);
    Req.set_serverip(strIp);
    Req.set_servername(strSvrName);
    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwCenterConnID, MSG_LOGIC_REGTO_CENTER_REQ, 0, 0, Req);
}

BOOL CGameService::OnNewConnect(INT32 nConnID)
{
    if(nConnID == m_dwLoginConnID)
    {
        RegisterToLoginSvr();

        return TRUE;
    }

    if(nConnID == m_dwCenterConnID)
    {
        RegisterToCenterSvr();

        return TRUE;
    }

    if (nConnID == m_dwDBConnID)
    {
        RegisterToDBSvr();

        return TRUE;
    }

    CWatcherClient::GetInstancePtr()->OnNewConnect(nConnID);

    return TRUE;
}

BOOL CGameService::OnCloseConnect(INT32 nConnID)
{
    if(m_dwLoginConnID == nConnID)
    {
        m_dwLoginConnID = 0;
        return TRUE;
    }

    if(m_dwLogConnID == nConnID)
    {
        m_dwLogConnID = 0;
        return TRUE;
    }

    if(m_dwDBConnID == nConnID)
    {
        m_dwDBConnID = 0;
        return TRUE;
    }

    if(m_dwCenterConnID == nConnID)
    {
        m_dwCenterConnID = 0;
        return TRUE;
    }

    CWatcherClient::GetInstancePtr()->OnCloseConnect(nConnID);

    CGameSvrMgr::GetInstancePtr()->OnCloseConnect(nConnID);

    return TRUE;
}

BOOL CGameService::OnSecondTimer()
{
    ConnectToLogServer();

    ConnectToLoginSvr();

    ConnectToDBSvr();

    ConnectToCenterSvr();

    ReportServerStatus();

    CWatcherClient::GetInstancePtr()->OnSecondTimer();

    return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket* pNetPacket)
{
    if (pNetPacket->m_nMsgID != MSG_LOGIC_UPDATE_ACK)
    {
        CLog::GetInstancePtr()->LogInfo("Receive Message:[%s]", MessageID_Name((MessageID)pNetPacket->m_nMsgID).c_str());
    }

    if (CWatcherClient::GetInstancePtr()->DispatchPacket(pNetPacket))
    {
        return TRUE;
    }

    if (CWebCommandMgr::GetInstancePtr()->DispatchPacket(pNetPacket))
    {
        return TRUE;
    }

    if (CMsgHandlerManager::GetInstancePtr()->FireMessage(pNetPacket->m_nMsgID, pNetPacket))
    {
        return TRUE;
    }

    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    CPlayerObject* pPlayer = CPlayerManager::GetInstancePtr()->GetPlayer(pHeader->u64TargetID);
    if (pPlayer == NULL)
    {
        CLog::GetInstancePtr()->LogError("CGameService::DispatchPacket Error Invalid u64TargetID:%lld, MessageID:%d", pHeader->u64TargetID, pNetPacket->m_nMsgID);
        return TRUE;
    }

    if (pPlayer->FireMessage(pNetPacket->m_nMsgID, pNetPacket))
    {
        return TRUE;
    }

    return TRUE;
}


UINT32 CGameService::GetDBConnID()
{
    return m_dwDBConnID;
}

UINT32 CGameService::GetLoginConnID()
{
    return m_dwLoginConnID;
}

UINT32 CGameService::GetServerID()
{
    return CConfigFile::GetInstancePtr()->GetIntValue("areaid");
}

UINT32 CGameService::GetCenterConnID()
{
    return m_dwCenterConnID;
}

BOOL CGameService::ReportServerStatus()
{
    if (m_dwLoginConnID <= 0)
    {
        return TRUE;
    }

    if (!m_bRegSuccessed)
    {
        return TRUE;
    }

    static INT32 nCount = 30;
    nCount++;
    if (nCount < 59)
    {
        return TRUE;
    }

    nCount = 0;

    if (CGlobalDataManager::GetInstancePtr()->GetMaxOnline() < CPlayerManager::GetInstancePtr()->GetOnlineCount())
    {
        CGlobalDataManager::GetInstancePtr()->SetMaxOnline(CPlayerManager::GetInstancePtr()->GetOnlineCount());
    }

    LogicUpdateInfoReq Req;

    Req.set_maxonline(CGlobalDataManager::GetInstancePtr()->GetMaxOnline()); //最大在线人数
    Req.set_curonline(CPlayerManager::GetInstancePtr()->GetOnlineCount());   //当前在线人数
    Req.set_totalnum(CSimpleManager::GetInstancePtr()->GetTotalCount());     //总注册人数
    Req.set_cachenum(CPlayerManager::GetInstancePtr()->GetCount());          //当前缓存人数
    Req.set_serverid(CConfigFile::GetInstancePtr()->GetIntValue("areaid"));  //区服ID
    Req.set_servername(CConfigFile::GetInstancePtr()->GetStringValue("areaname")); //区服名字
    Req.set_dberrcnt(m_dwDbErrorCount);                                      //db写错误数

    return ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwLoginConnID, MSG_LOGIC_UPDATE_REQ, 0, 0, Req);
}

BOOL CGameService::OnMsgRegToLoginAck(NetPacket* pNetPacket)
{
    LogicRegToLoginAck Ack;
    Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    ERROR_RETURN_TRUE(Ack.retcode() == MRC_SUCCESSED);
    //表示注册成功
    m_bRegSuccessed = TRUE;

    m_uSvrOpenTime = Ack.svropentime();

    CLog::GetInstancePtr()->LogError("---------开服时间:%s--------", CommonFunc::TimeToString(m_uSvrOpenTime).c_str());

    return TRUE;
}

BOOL CGameService::OnMsgRegToCenterAck(NetPacket* pNetPacket)
{
    SvrRegToSvrAck Ack;
    Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(Ack.retcode() == MRC_SUCCESSED);
    return TRUE;
}

BOOL CGameService::OnMsgUpdateInfoAck(NetPacket* pNetPacket)
{
    LogicUpdateInfoAck Ack;
    Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
    ERROR_RETURN_TRUE(Ack.retcode() == 0);

    if (m_uSvrOpenTime == Ack.svropentime())
    {
        return TRUE;
    }

    m_uSvrOpenTime = Ack.svropentime();

    //这里处理开服时间发生改变有事件
    CLog::GetInstancePtr()->LogError("---------开服时间:%s--------", CommonFunc::TimeToString(m_uSvrOpenTime).c_str());

    return TRUE;
}

BOOL CGameService::OnMsgDBWriteErrorNty(NetPacket* pNetPacket)
{
    Msg_DbErrorCountNty Nty;
    Nty.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
    PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

    m_dwDbErrorCount = Nty.errorcount();

    return TRUE;
}

BOOL CGameService::OnMsgRegToDBSvrAck(NetPacket* pNetPacket)
{
    return TRUE;
}
