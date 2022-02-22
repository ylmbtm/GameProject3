#ifndef _LOGIC_SEVER_MANAGERH_
#define _LOGIC_SEVER_MANAGERH_
#include "DBInterface/CppMysql.h"
#include "LockFreeQueue.h"

enum EStatusFlag
{
    ESF_NONE        = 0,//未知
    ESF_GOOD        = 1,//流畅
    ESF_BUSY        = 2,//拥挤
    ESF_FULL        = 3,//爆满
    ESF_MAINTAIN    = 4,//维护
    ESF_SHUTDOWN    = 5,//关闭
    ESF_REVIEW      = 6,//评审
};

enum ECornerMark
{
    ECM_NONE = 0,//无
    ECM_NEW = 1, //新服
    ECM_SURGEST = 2,//推荐
};

enum EUpdateStatus
{
    EUS_NONE = 0,   //无变化
    EUS_UPDATE = 1, //数据变化
    EUS_RE_REG = 2, //重启注册
    EUS_NEW_REG = 3 //新服注册
};

enum EServerStatus
{
    ESS_SVR_OFFLINE     = 0, //掉线
    ESS_SVR_ONLINE      = 1, //在线
    ESS_SVR_STARTING    = 2, //启动中
    ESS_SVR_UPDATING    = 3, //更新中
};

struct LogicServerNode
{
    LogicServerNode()
    {
        m_nConnID      = 0;
        m_nServerID    = 0;
        m_nPort        = 0;
        m_nHttpPort    = 0;
        m_ServerFlag    = ESF_GOOD;
        m_CornerMark    = 0;
        m_uLastUpdate   = 0;
        m_nMinVersion  = 0;
        m_nMaxVersion  = 0;
        m_nMaxOnline   = 0;    //最大在线人数
        m_nCurOnline   = 0;    //当前最大人数
        m_nTotalNum    = 0;    //总注册人数
        m_nCacheNum    = 0;    //当前缓存人数
        m_uSvrOpenTime  = 0;
        m_nErrorCnt    = 0;
        m_nFileVer     = 0;
        m_eChangeStatus = EUS_NONE;
        m_ServerStatus  = ESS_SVR_OFFLINE;
    }

    BOOL CheckIP(UINT32 dwIPaddr);

    BOOL CheckChannel(INT32 nChannel);

    BOOL CheckVersion(std::string strVersion);

    INT32       m_nConnID;     //连接ID
    INT32       m_nServerID;   //服务器ID
    INT32       m_nPort;       //游戏端口号
    INT32       m_nHttpPort;   //http端口
    INT32       m_ServerFlag;   //服务器标记
    INT32       m_CornerMark;   //服务器角标
    INT32       m_nMinVersion; //最小可见版本
    INT32       m_nMaxVersion; //最大可见版本
    UINT64      m_uSvrOpenTime; //开服时间
    std::string m_strSvrName;   //服务器名字
    std::string m_strOuterAddr; //服务器的外网地址
    std::string m_strInnerAddr; //服务器的内网地址
    UINT64      m_uLastUpdate;  //服务器最后更新时间
    std::set<INT32> m_CheckIpList;   //IP白名单
    std::set<INT32>  m_CheckChannelList; //渠道白名单
    std::set<UINT64>  m_CheckAccountList; //账号白名单
    EServerStatus    m_ServerStatus;   //服务器状态

    INT32       m_nMaxOnline;   //最大在线人数
    INT32       m_nCurOnline;   //当前最大人数
    INT32       m_nTotalNum;    //总注册人数
    INT32       m_nCacheNum;    //当前缓存人数
    INT32       m_nErrorCnt;    //数据库写失败次数
    INT32       m_nFileVer;     //服务器文件版本

    EUpdateStatus       m_eChangeStatus;

};

struct GameParamNode
{
    INT32 m_nChannel;  //渠道
    std::string m_strParamKey;//
    std::string m_strParamValue; //实际服务器ID
};

class LogicSvrManager : public std::map<INT32, LogicServerNode*>
{
public:
    LogicSvrManager(void);
    ~LogicSvrManager(void);

public:
    BOOL    Init();

    BOOL    Uninit();

    BOOL    RegisterLogicServer(INT32 nConnID, INT32 nServerID, INT32 nPort, INT32 nHttpPort, const std::string& strSvrName, const std::string& strInnderIp);

    BOOL    UnregisterLogicServer(INT32 nConnID, INT32 nServerID);

    BOOL    UpdateLogicServerInfo(INT32 nServerID, INT32 nMaxOnline, INT32 nCurOnline, INT32 nTotal, INT32 nCacheNum, INT32 nStatus, INT32 nErrorCount, const std::string& strSvrName);

    BOOL    ReloadServerList(INT32 nServerID = 0);

    BOOL    SaveLogicServerThread();

    INT32   GetLogicConnID(INT32 nServerID);

    LogicServerNode* GetSuggestServer(BOOL bReview, INT32 nChannel, UINT32 dwIpaddr);

    LogicServerNode* GetLogicServerInfo(INT32 nServerID);

    BOOL    OnCloseConnect(INT32 nConnID);

    BOOL                m_IsRun;

    std::thread*        m_pThread;

    ArrayLockFreeQueue<LogicServerNode*, 1024>      m_ArrChangedNode;
    std::vector<GameParamNode>  m_vtGameParam;
};

#endif //_LOGIC_SEVER_MANAGERH_