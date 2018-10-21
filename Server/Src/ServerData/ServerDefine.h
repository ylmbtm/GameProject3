#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__

//货币数量
#define MONEY_NUM			15

//行动力数量
#define ACTION_NUM			4

//角色属性个数
#define PROPERTY_NUM		21

//角色的最大等级
#define MAX_ROLE_LEVEL		150

//服务器帧率
#define FPS_TIME_TICK		20

//公会名长度
#define GUILD_NAME_LEN		255
#define GUILD_NOTICE_LEN	1024

//语言数
#define MAX_LANGUAGE_NUM	15

#define MAX_VIP_LEVEL		20

#define SQL_BUFF_LEN		1024

//邮件
#define MAIL_CONTNET_LEN	2048
#define MAIL_TITLE_LEN		255
#define MAIL_ITEM_COUNT		10

#define ROLE_NAME_LEN		255

#define EQUIP_MAX_NUM	    8

#define PARTNER_MAX_NUM		2

//副本战斗结果
enum ECopyResult
{
	CR_NONE = 0,	//未结算
	CR_WIN,         //胜利
	CR_LOST,		//失败
	CR_DRAW			//平局
};

enum EMouduleType
{
	MT_ROLE,
	MT_COPY,
	MT_BAG,
	MT_EQUIP,
	MT_GEM,
	MT_PET,
	MT_PARTNER,
	MT_TASK,
	MT_MOUNT,
	MT_ACTIVITY,
	MT_COUNTER,
	MT_STORE,
	MT_END
};

enum EAddWay
{
	Add_ = 1,
	Cost_
};

enum EActivtyType
{
	ACT_NONE		= 0,
	ACT_LOGINAWARD	= 1,
	ACT_SINCHRAGE	= 2,
	ACT_SUMCHARGE	= 3,
	ACT_OPEN_FUND	= 4,
	ACT_DISCOUNT_SALE = 5,
};

enum EShareData
{
	ESD_ROLE = 1,
	ESD_GLOBAL,
	ESD_BAG,
	ESD_COPY,
	ESD_EQUIP,
	ESD_GEM,
	ESD_PET,
	ESD_PARTNER,
	ESD_GUILD,
	ESD_GUILD_MEMBER,
	ESD_TASK,
	ESD_MOUNT,
	ESD_MAIL,
	ESD_GROUP_MAIL,
	ESD_ACTIVITY,
	ESD_COUNTER,
	ESD_FRIEND,
	ESD_CHAPTER,
};

enum EGuildPos
{
	EGP_MEMBER = 0,
	EGP_LEADER = 1,
};

#endif //__SERVER_STRUCT_H__