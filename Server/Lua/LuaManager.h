#ifndef __LUA_MANAGER_H__
#define __LUA_MANAGER_H__

class LuaManager
{
public:
	LuaManager(void);
	~LuaManager(void);

public:
	BOOL	Close();

	BOOL	Init();

	BOOL	LoadScriptFile(const char *pszLuaFile);

	BOOL	LoadScriptFile(std::vector<std::string> &vtScriptList);

	BOOL	CallLuaFunction(std::string strFuncName, char *pStrParamSig, ...);

	BOOL	GetStackParams(char *pStrParamSig, ...);

	BOOL    RegisterFunction(const char *libname, const luaL_Reg *l);

	//获取堆栈脚本变量
	BOOL	GetStackValue_Ptr(INT32 nStackIndex, VOID* &ptrValue);
	BOOL	GetStackValue_Int(INT32 nStackIndex, INT32 &intValue);
	BOOL	GetStackValue_Double(INT32 nStackIndex, DOUBLE &doubleValue);
	BOOL    GetStackValue_String(INT32 nStackIndex, const CHAR* &strValue);

	//获取全局脚本变量
	INT32	GetGlobalVarInt(const char *pszVarName);
	BOOL	GetGlobalVarBoolean(const char *pszVarName);
	DOUBLE  GetGlobalVarDouble(const char *pszVarName);
	const CHAR*	GetGlobalVarString(const char *pszVarName);
	lua_State* GetLuaState();
	
protected:
	lua_State*			m_pLuaState;
};

#endif