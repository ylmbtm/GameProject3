#ifndef _LOGIC_SEVER_MANAGERH_
#define _LOGIC_SEVER_MANAGERH_
#include "DBInterface/CppMysql.h"
#include "LockFreeQueue.h"

enum EStatusFlag
{
	ESF_NONE		= 0,//未知
	ESF_GOOD		= 1,//流畅
	ESF_BUSY		= 2,//拥挤
	ESF_FULL        = 3,//爆满
	ESF_MAINTAIN	= 4,//维护
	ESF_SHUTDOWN	= 5,//关闭
	ESF_REVIEW		= 6,//评审
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
		m_dwConnID      = 0;
		m_dwServerID    = 0;
		m_dwPort        = 0;
		m_dwHttpPort    = 0;
		m_dwWatchPort   = 0;
		m_ServerFlag    = ESF_GOOD;
		m_CornerMark    = 0;
		m_uLastUpdate   = 0;
		m_dwMinVersion  = 0;
		m_dwMaxVersion  = 0;
		m_dwMaxOnline   = 0;    //最大在线人数
		m_dwCurOnline   = 0;    //当前最大人数
		m_dwTotalNum    = 0;    //总注册人数
		m_dwCacheNum    = 0;    //当前缓存人数
		m_uSvrOpenTime  = 0;
		m_dwErrorCnt    = 0;
		m_eChangeStatus = EUS_NONE;
		m_ServerStatus  = ESS_SVR_OFFLINE;
	}

	BOOL CheckIP(UINT32 dwIPaddr);

	BOOL CheckChannel(UINT32 dwChannel);

	BOOL CheckVersion(std::string strVersion);

	UINT32		m_dwConnID;     //连接ID
	UINT32		m_dwServerID;   //服务器ID
	UINT32		m_dwPort;       //游戏端口号
	UINT32      m_dwHttpPort;   //http端口
	UINT32      m_dwWatchPort;  //监视端口
	UINT32		m_ServerFlag;   //服务器标记
	UINT32		m_CornerMark;   //服务器角标
	UINT32		m_dwMinVersion; //最小可见版本
	UINT32		m_dwMaxVersion; //最大可见版本
	UINT64      m_uSvrOpenTime; //开服时间
	std::string m_strSvrName;   //服务器名字
	std::string m_strOuterAddr;    //服务器的外网地址
	std::string m_strInnerAddr; //服务器的内网地址
	UINT64		m_uLastUpdate;  //服务器最后更新时间
	std::set<UINT32> m_CheckIpList;	  //过滤的IP地址
	std::set<INT32>  m_CheckChannelList; //可以看见的渠道
	EServerStatus    m_ServerStatus;   //服务器状态

	UINT32      m_dwMaxOnline;   //最大在线人数
	UINT32      m_dwCurOnline;   //当前最大人数
	UINT32      m_dwTotalNum;    //总注册人数
	UINT32      m_dwCacheNum;    //当前缓存人数
	UINT32      m_dwErrorCnt;    //数据库写失败次数

	EUpdateStatus       m_eChangeStatus;

};

class LogicSvrManager : public std::map<UINT32, LogicServerNode*>
{
public:
	LogicSvrManager(void);
	~LogicSvrManager(void);

public:
	BOOL	Init();

	BOOL	Uninit();

	BOOL	RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwPort, UINT32 dwHttpPort, UINT32 dwWatchPort, const std::string& strSvrName, const std::string& strInnderIp);

	BOOL	UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID);

	BOOL    UpdateLogicServerInfo(UINT32 dwServerID, UINT32 dwMaxOnline, UINT32 dwCurOnline, UINT32 dwTotal, UINT32 dwCacheNum, UINT32 dwStatus, UINT32 dwErrorCount, const std::string& strSvrName);

	BOOL	ReloadServerList(UINT32 dwServerID = 0);

	BOOL    SaveLogicServerThread();

	UINT32	GetLogicConnID(UINT32 dwServerID);

	LogicServerNode* GetSuggestServer(BOOL bReview, UINT32 dwChannel, UINT32 dwIpaddr);

	LogicServerNode* GetLogicServerInfo(UINT32 dwServerID);

	BOOL    OnCloseConnect(UINT32 dwConnID);

	BOOL				m_IsRun;

	std::thread*  		m_pThread;

	ArrayLockFreeQueue<LogicServerNode*, 1024>		m_ArrChangedNode;
};

#endif //_LOGIC_SEVER_MANAGERH_