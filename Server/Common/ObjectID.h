#ifndef __OBJECT_ID_DEFINE_H__
#define __OBJECT_ID_DEFINE_H__

//以下是对象类型

enum OBJECT_TYPE
{
	OT_Unknow,
	OT_Player,
};



//1-8 表示对象类型  8-32 表示索引  32-64 账户ID

//取对象类型
#define GET_OBJ_TYPE(charid)			((UINT8)(charid>>56))

//取玩家的用户ID 
#define GET_ACCOUNT_ID(charid)			((UINT32)charid)

//取账户的角色索引
#define GET_ROLE_INDEX(charid)			((charid&0x00ffffff00000000)>>32)

//合成玩家ID
#define MAKE_PLAYER_ID(charindex, accountid) (((UINT64)OT_Player<<56)|((UINT64)charindex<<32)|((UINT64)accountid))

#define CHECK_PAYER_ID(charid) ASSERT(GET_OBJ_TYPE(charid) == OT_Player);ASSERT(GET_ROLE_INDEX(charid) < 10);

////////////////////////////////////////////


#endif //__OBJECT_ID_DEFINE_H__
