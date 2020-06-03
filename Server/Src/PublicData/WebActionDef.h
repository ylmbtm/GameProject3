#ifndef __WEB_ACTION_DEFINE_H__
#define __WEB_ACTION_DEFINE_H__

enum EWebAction
{
	EWA_NONE = 0,               //无效动作
	EWA_SEAL_ACCOUNT,           //账号的封禁与解封
	EWA_SEAL_ROLE,              //封禁角色
	EWA_SERVER_CHNAGE,          //服务器配制改变
	EWA_RELOAD_TABLE,           //重新加载配制表
};

#endif //__WEB_ACTION_DEFINE_H__