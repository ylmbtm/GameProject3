/*
** Lua binding: LuaScript
** Generated automatically by tolua++-1.0.93-lua53 on Wed Jun  3 13:15:21 2020.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_LuaScript_open (lua_State* tolua_S);

#include  "PlayerObject.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CPlayerObject");
}

/* method: GetObjectID of class  CPlayerObject */
#ifndef TOLUA_DISABLE_tolua_LuaScript_CPlayerObject_GetObjectID00
static int tolua_LuaScript_CPlayerObject_GetObjectID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CPlayerObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CPlayerObject* self = (CPlayerObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetObjectID'", NULL);
#endif
  {
   unsigned long long tolua_ret = ( unsigned long long)  self->GetObjectID();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetObjectID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCityCopyID of class  CPlayerObject */
#ifndef TOLUA_DISABLE_tolua_LuaScript_CPlayerObject_GetCityCopyID00
static int tolua_LuaScript_CPlayerObject_GetCityCopyID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CPlayerObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CPlayerObject* self = (CPlayerObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCityCopyID'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->GetCityCopyID();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCityCopyID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CheckCopyConditoin of class  CPlayerObject */
#ifndef TOLUA_DISABLE_tolua_LuaScript_CPlayerObject_CheckCopyConditoin00
static int tolua_LuaScript_CPlayerObject_CheckCopyConditoin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CPlayerObject",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CPlayerObject* self = (CPlayerObject*)  tolua_tousertype(tolua_S,1,0);
  unsigned int dwCopyID = (( unsigned int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CheckCopyConditoin'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->CheckCopyConditoin(dwCopyID);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CheckCopyConditoin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_LuaScript_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"CR_NONE",CR_NONE);
  tolua_constant(tolua_S,"CR_WIN",CR_WIN);
  tolua_constant(tolua_S,"CR_LOST",CR_LOST);
  tolua_constant(tolua_S,"CR_DRAW",CR_DRAW);
  tolua_cclass(tolua_S,"CPlayerObject","CPlayerObject","",NULL);
  tolua_beginmodule(tolua_S,"CPlayerObject");
   tolua_function(tolua_S,"GetObjectID",tolua_LuaScript_CPlayerObject_GetObjectID00);
   tolua_function(tolua_S,"GetCityCopyID",tolua_LuaScript_CPlayerObject_GetCityCopyID00);
   tolua_function(tolua_S,"CheckCopyConditoin",tolua_LuaScript_CPlayerObject_CheckCopyConditoin00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_LuaScript (lua_State* tolua_S) {
 return tolua_LuaScript_open(tolua_S);
};
#endif

