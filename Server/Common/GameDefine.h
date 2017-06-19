#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__

#define MAX_PATH_LEN	  	        (260)
#define MAX_NAME_LEN				(32)
#define CONST_BUFF_SIZE				(8192)

/***************************************
//以下是
***************************************/
#define UPDATE_FLAG_CHANGE			0
#define UPDATE_FLAG_CREATE			1

#define UPDATE_TO_OTHERS			1
#define UPDATE_TO_SERVER			2
#define UPDATE_TO_MYSELF			3

//以下是服务器
#define INVALIDE_SERVERID			(0xffffffff)

enum  UpdateStatusEnum
{
	UT_None,
	UT_Update,
	UT_New,
	UT_Delete
};

enum ClientState
{
	LS_Connect,
	PS_Connect,
	WS_Connect,
	GS_Connect,
	DS_Connect
};

enum LeaveGameReason
{
	LGR_Disconnect,		//断线退出
	LGR_Quit			//主动退出
};



////////////////////////////////////////////


#endif //__COMMON_DEFINE_H__
