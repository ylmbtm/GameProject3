#ifndef _WATCH_MSG_HANDLER_H_
#define _WATCH_MSG_HANDLER_H_

#include "RapidXml.h"
#include "WebActionDef.h"
#include "HttpParameter.h"

enum EProcessStatus
{
    EPS_Stop,       //停止状态
    EPS_Start,      //启动状态
    EPS_Checking,   //检测状态
};

struct ProcessInfo
{
    INT64           ProcessID;
    std::string     ProcessName;
    EProcessStatus  ProscessStatus;
};

struct ServerInfo
{
    INT32           nAreaID = 0;
    BOOL            bWatch = FALSE;
    UINT64          uLastOpTime = 0; //上一次操作时间
    std::vector<ProcessInfo> ProcessList;
};

class CWatchMsgHandler
{
public:
    CWatchMsgHandler();

    ~CWatchMsgHandler();

    BOOL        Init(INT32 nReserved);

    BOOL        Uninit();

    BOOL        DispatchPacket( NetPacket* pNetPacket);

    BOOL        OnNewConnect(INT32 nConnID);

    BOOL        OnCloseConnect(INT32 nConnID);

    BOOL        OnSecondTimer();

    BOOL        ReportStatusTimer(INT32 key);

    BOOL        RegExitSignal();

    BOOL        SendWebResult(INT32 nConnID, EWebResult eResult);
public:
    //*********************消息处理定义开始******************************
    BOOL OnMsgWebCommandReq(NetPacket* pNetPacket);
    //*********************消息处理定义结束******************************

public:
    //*********************WebAction处理定义开始******************************
    void OnGmServerStart(HttpParameter& hParams, INT32 nConnID);
    void OnGmServerStop(HttpParameter& hParams, INT32 nConnID);
    void OnGmServerUpdate(HttpParameter& hParams, INT32 nConnID);
    void OnGmServerInfo(HttpParameter& hParams, INT32 nConnID);
    //*********************WebAction处理定义开始******************************

protected:

    BOOL InitServerList();

    BOOL LoadCloudParam();

    BOOL CheckServerStatus(BOOL bFrist = FALSE);

    ServerInfo* GetServerInfo(INT32 nAreaID);

    std::map<INT32, ServerInfo> m_mapServer;

    std::string m_strRootPath;
    std::string m_strOuterIp;
    std::string m_strInnerIp;
    std::string m_strGmSvrIp;
    INT32       m_nCloudID;

    std::string m_strSvrName;
};

#endif //_WATCH_MSG_HANDLER_H_
