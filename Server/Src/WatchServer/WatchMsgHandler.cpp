#include "stdafx.h"

#include "WatchMsgHandler.h"
#include "GameService.h"
#include "PacketHeader.h"
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "HttpParameter.h"
#include "WebActionDef.h"
#include "DBInterface/CppMysql.h"
#include "TimerManager.h"
#include "CommonSocket.h"

#ifdef WIN32
static BOOL        ExitRoutine(INT32 nSignal)
{
    switch (nSignal)
    {
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        {
            CGameService::GetInstancePtr()->m_bIsRun = FALSE;
        }
        break;
        default:
            break;
    }

    return TRUE;
}
#else

static VOID        ExitRoutine(INT32 nSignal)
{
    switch (nSignal)
    {
        case SIGINT:
        case SIGQUIT:
        case SIGKILL:
        case SIGTERM:
        {
            CGameService::GetInstancePtr()->m_bIsRun = FALSE;
        }
        break;
        default:
            break;
    }
}
#endif;

CWatchMsgHandler::CWatchMsgHandler()
{
}

CWatchMsgHandler::~CWatchMsgHandler()
{

}

BOOL CWatchMsgHandler::Init(INT32 nReserved)
{
    if (!RegExitSignal())
    {
        CLog::GetInstancePtr()->LogError("Cannot Register Exit Signal!");
        return FALSE;
    }

    if (!InitServerList())
    {
        return FALSE;
    }

    if (!LoadCloudParam())
    {
        return FALSE;
    }

    TimerManager::GetInstancePtr()->AddDiffTimer(60, 1, &CWatchMsgHandler::ReportStatusTimer, this);

    return TRUE;
}

BOOL CWatchMsgHandler::Uninit()
{
    return TRUE;
}

BOOL CWatchMsgHandler::DispatchPacket(NetPacket* pNetPacket)
{
    switch (pNetPacket->m_nMsgID)
    {
            PROCESS_MESSAGE_ITEM(MSG_PHP_GM_COMMAND_REQ,        OnMsgWebCommandReq)
    }

    return FALSE;
}

BOOL CWatchMsgHandler::OnNewConnect(INT32 nConnID)
{
    return TRUE;
}

BOOL CWatchMsgHandler::OnCloseConnect(INT32 nConnID)
{
    return TRUE;
}

BOOL CWatchMsgHandler::OnSecondTimer()
{
    return TRUE;
}

BOOL CWatchMsgHandler::ReportStatusTimer(INT32 key)
{
    //std::string strValue = CommonSocket::HttpGet("192.168.2.185", 80, "/gameconsole/main/api/testhttpget", "");

    return TRUE;
}

BOOL CWatchMsgHandler::RegExitSignal()
{
#ifdef _WIN32
    return SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitRoutine, TRUE);
#else
    signal(SIGINT, ExitRoutine);
    signal(SIGQUIT, ExitRoutine);
    signal(SIGKILL, ExitRoutine);
    signal(SIGTERM, ExitRoutine);
    return TRUE;
#endif
}

BOOL CWatchMsgHandler::OnMsgWebCommandReq(NetPacket* pNetPacket)
{
    ERROR_RETURN_TRUE(pNetPacket->m_pDataBuffer->GetBodyLenth() < 10000);

    CHAR szMsgBuf[10240] = { 0 };
    CommonConvert::StrCopy(szMsgBuf, pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth() + 1);

    HttpParameter Params;
    Params.ParseStringToMap(szMsgBuf);
    std::string strAction = Params.GetStrValue("Action");
    CLog::GetInstancePtr()->LogHiInfo("Web Action :%s", strAction.c_str());

    EWebAction eWebAction = (EWebAction)CommonConvert::StringToInt(strAction.c_str());
    switch (eWebAction)
    {
        case EWA_SERVER_START:
            OnGmServerStart(Params, pNetPacket->m_nConnID);
            break;
        case EWA_SERVER_STOP:
            OnGmServerStop(Params, pNetPacket->m_nConnID);
            break;
        case EWA_SERVER_UPDATE:
            OnGmServerUpdate(Params, pNetPacket->m_nConnID);
            break;
        case EWA_SERVER_INFO:
            OnGmServerInfo(Params, pNetPacket->m_nConnID);
            break;
        default:
            SendWebResult(pNetPacket->m_nConnID, EWR_INVALID_ACT);
            break;
    }

    return TRUE;
}

void CWatchMsgHandler::OnGmServerStart(HttpParameter& hParams, INT32 nConnID)
{
    ERROR_RETURN_NONE(nConnID != 0);

    INT32 nAreaID = hParams.GetIntValue("areaid");
    ERROR_RETURN_NONE(nAreaID != 0);

    CLog::GetInstancePtr()->LogHiInfo("OnGmServerStart Server:%d!", nAreaID);

    ServerInfo* pSvrInfo = GetServerInfo(nAreaID);
    if (pSvrInfo == NULL)
    {
        SendWebResult(nConnID, EWR_FAILURE);
        return;
    }

    if ((CommonFunc::GetCurrTime() - pSvrInfo->uLastOpTime) < 10)
    {
        SendWebResult(nConnID, EWR_FAILURE);
        return;
    }

    std::string strWorkPath = m_strRootPath + m_strSvrName + CommonConvert::IntToString(nAreaID) + '/';

    std::string strExe = strWorkPath + CConfigFile::GetInstancePtr()->GetStringValue("server_start");

    system(strExe.c_str());

    SendWebResult(nConnID, EWR_SUCCESSED);
}

void CWatchMsgHandler::OnGmServerStop(HttpParameter& hParams, INT32 nConnID)
{
    ERROR_RETURN_NONE(nConnID != 0);

    INT32 nAreaID = hParams.GetIntValue("areaid");
    ERROR_RETURN_NONE(nAreaID != 0);

    CLog::GetInstancePtr()->LogHiInfo("OnGmServerStop Server:%d!", nAreaID);

    ServerInfo* pSvrInfo = GetServerInfo(nAreaID);
    if (pSvrInfo == NULL)
    {
        SendWebResult(nConnID, EWR_FAILURE);
        return;
    }

    if ((CommonFunc::GetCurrTime() - pSvrInfo->uLastOpTime) < 10)
    {
        SendWebResult(nConnID, EWR_FAILURE);
        return;
    }

    std::string strWorkPath = m_strRootPath + m_strSvrName + CommonConvert::IntToString(nAreaID) + '/';

    std::string strExe = strWorkPath + CConfigFile::GetInstancePtr()->GetStringValue("server_stop");

    system(strExe.c_str());

    SendWebResult(nConnID, EWR_SUCCESSED);
}

void CWatchMsgHandler::OnGmServerUpdate(HttpParameter& hParams, INT32 nConnID)
{
    ERROR_RETURN_NONE(nConnID != 0);

    INT32 nAreaID = hParams.GetIntValue("areaid");
    ERROR_RETURN_NONE(nAreaID != 0);

    CLog::GetInstancePtr()->LogHiInfo("OnGmServerUpdate Server:%d!", nAreaID);

    ServerInfo* pSvrInfo = GetServerInfo(nAreaID);
    if (pSvrInfo == NULL)
    {
        SendWebResult(nConnID, EWR_FAILURE);
        return;
    }

    if ((CommonFunc::GetCurrTime() - pSvrInfo->uLastOpTime) < 10)
    {
        SendWebResult(nConnID, EWR_FAILURE);
        return;
    }

    SendWebResult(nConnID, EWR_SUCCESSED);

    std::string strWorkPath = m_strRootPath + m_strSvrName + CommonConvert::IntToString(nAreaID) + '/';

    std::string strExe = strWorkPath + CConfigFile::GetInstancePtr()->GetStringValue("server_update");

    system(strExe.c_str());
}

void CWatchMsgHandler::OnGmServerInfo(HttpParameter& hParams, INT32 nConnID)
{
    SendWebResult(nConnID, EWR_SUCCESSED);
}

BOOL CWatchMsgHandler::InitServerList()
{
    m_strRootPath = CConfigFile::GetInstancePtr()->GetStringValue("server_root_path");
    m_strSvrName = CConfigFile::GetInstancePtr()->GetStringValue("server_dir_name");
    m_strGmSvrIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");

    m_strInnerIp = CConfigFile::GetInstancePtr()->GetStringValue("watch_svr_ip");
    m_strOuterIp = CConfigFile::GetInstancePtr()->GetStringValue("login_svr_ip");

    std::vector<std::string> vtProcess;
    CommonConvert::SpliteString(CConfigFile::GetInstancePtr()->GetStringValue("process_list"), ' ', vtProcess);

    std::vector< std::string> vtDirNames;
    CommonFunc::GetSubDirNames(m_strRootPath.c_str(), m_strSvrName.c_str(), vtDirNames, FALSE);

    for (int i = 0; i < vtDirNames.size(); i++)
    {
        CLog::GetInstancePtr()->LogHiInfo("Server Directory[%d]：%s", i, vtDirNames.at(i).c_str());
    }

    for (INT32 i = 0; i < vtDirNames.size(); i++)
    {
        INT32 nAreaID = CommonConvert::StringToInt(vtDirNames.at(i).substr(m_strSvrName.size()).c_str());
        if (nAreaID <= 0)
        {
            continue;
        }

        ServerInfo svrInfo;
        svrInfo.bWatch = FALSE;
        svrInfo.nAreaID = nAreaID;
        svrInfo.uLastOpTime = 0;

        for (int j = 0; j < vtProcess.size(); j++)
        {
            ProcessInfo proInfo;
            proInfo.ProcessID = 0;
            proInfo.ProcessName = vtProcess.at(j);
            CommonConvert::ReplaceString(proInfo.ProcessName, "@", CommonConvert::IntToString(nAreaID));
            proInfo.ProscessStatus = EPS_Stop;
            svrInfo.ProcessList.push_back(proInfo);
        }

        m_mapServer.insert(std::make_pair(nAreaID, svrInfo));
    }

    return TRUE;
}

BOOL CWatchMsgHandler::LoadCloudParam()
{
    return TRUE;
}

BOOL CWatchMsgHandler::CheckServerStatus(BOOL bFrist)
{
    for (auto itor = m_mapServer.begin(); itor != m_mapServer.end(); ++itor)
    {
        ServerInfo& svrInfo = itor->second;

        if (svrInfo.bWatch == FALSE && !bFrist)
        {
            continue;
        }

        for (int i = 0; i < svrInfo.ProcessList.size(); i++)
        {
            ProcessInfo& proInfo = svrInfo.ProcessList.at(i);

            if (CommonFunc::IsProcessExist(proInfo.ProcessID))
            {
                continue;
            }

            proInfo.ProcessID = CommonFunc::GetProcessID(proInfo.ProcessName.c_str());
            if (proInfo.ProcessID == 0)
            {
                if (bFrist || svrInfo.bWatch == FALSE)
                {
                    continue;
                }

                //表示进程己经挂了

                //

            }
            else
            {
                svrInfo.bWatch = TRUE;
            }
        }
    }

    return TRUE;
}

ServerInfo* CWatchMsgHandler::GetServerInfo(INT32 nAreaID)
{
    auto itor = m_mapServer.find(nAreaID);
    if (itor == m_mapServer.end())
    {
        return NULL;
    }

    return &itor->second;
}

BOOL CWatchMsgHandler::SendWebResult(INT32 nConnID, EWebResult eResult)
{
    std::string strResult = CommonConvert::IntToString((INT64)eResult);

    ServiceBase::GetInstancePtr()->SendMsgRawData(nConnID, MSG_PHP_GM_COMMAND_ACK, 0, 0, strResult.c_str(), (INT32)strResult.size());

    return TRUE;
}
