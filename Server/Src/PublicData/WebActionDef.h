#ifndef __WEB_ACTION_DEFINE_H__
#define __WEB_ACTION_DEFINE_H__

enum EWebAction
{
    EWA_NONE = 0,               //无效动作
    EWA_SEAL_ACCOUNT,           //账号的封禁与解封
    EWA_SEAL_ROLE,              //封禁角色
    EWA_SERVER_CHNAGE,          //服务器配制改变
    EWA_REVIEW_CHANGE,          //评审信息改变
    EWA_RELOAD_TABLE,           //重新加载配制表
    EWA_SINGLE_MAIL,            //单发邮件
    EWA_GROUP_MAIL,             //群发邮件
    EWA_OPEN_SVR_TIME,          //修改开服时间
    EWA_SERVER_START,           //启动服务器
    EWA_SERVER_STOP,            //停止服务器
    EWA_SERVER_UPDATE,          //更新服务器
    EWA_SERVER_INFO,            //服务器信息
    EWA_GM_COMMAND,             //执行GM指令
    EWA_KICKOFF_PLAYER,         //踢玩家下线
    EWA_PAY_CALLBACK,           //充值回调通知
    EWA_DELTE_MAIL,             //删除邮件
    EWA_NOTICE_POPUP,           //游戏弹窗公告
    EWA_ACTIVITY_CHANGE,        //活动控制更新
    EWA_DELETE_ROLE,            //删除角色
    EWA_REMOVE_ITEM,            //删除角色道具
    EWA_MERGE_CHANGE,           //合服配制发生改变
    EWA_GAMEPARAM_CHANGE,       //游戏参数发生改变
};


enum EWebResult
{
    EWR_SUCCESSED = 0,          //成功
    EWR_FAILURE,                //失败
    EWR_INVALID_ACT,            //无效的操作
};


#endif //__WEB_ACTION_DEFINE_H__