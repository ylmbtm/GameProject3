#include "stdafx.h"
#include "LuaManager.h"
#include "CommonFunc.h"

LuaManager::LuaManager()
{
	m_pLuaState = NULL;
}

LuaManager::~LuaManager()
{
	Close();
}


BOOL LuaManager::Close()
{
	if(m_pLuaState != NULL)
	{
		lua_close(m_pLuaState);
		m_pLuaState = NULL;
	}

	return TRUE;
}

BOOL LuaManager::Init()
{
	if(m_pLuaState == NULL)
	{
		m_pLuaState = luaL_newstate();
		if (m_pLuaState == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		luaL_openlibs(m_pLuaState);
	}

	return TRUE;
}

BOOL LuaManager::Attach(lua_State* L)
{
	m_pLuaState = L;

	return TRUE;
}

BOOL LuaManager::Deattch()
{
	m_pLuaState = NULL;

	return TRUE;
}

BOOL LuaManager::LoadAllLua(const char* pszDir)
{
	std::vector<std::string> vtFiles;

	CommonFunc::GetDirFiles(pszDir, "*.lua", vtFiles, TRUE);

	return LoadScriptFile(vtFiles);
}

lua_State* LuaManager::GetLuaState()
{
	return m_pLuaState;
}

INT32 LuaManager::GetGlobalVarInt( const char* pszVarName )
{
	ASSERT(m_pLuaState != NULL);

	lua_getglobal(m_pLuaState, pszVarName);

	if(!lua_isnumber(m_pLuaState, -1))
	{
		lua_settop(m_pLuaState, 0);

		return 0;
	}

	return (INT32)lua_tointeger(m_pLuaState, -1);
}

BOOL LuaManager::GetGlobalVarBoolean( const char* pszVarName )
{
	ASSERT(m_pLuaState != NULL);

	lua_getglobal(m_pLuaState, pszVarName);

	if(!lua_isboolean(m_pLuaState, -1))
	{
		lua_settop(m_pLuaState, 0);

		return 0;
	}

	return lua_toboolean(m_pLuaState, -1);
}

const CHAR* LuaManager::GetGlobalVarString( const char* pszVarName )
{
	ASSERT(m_pLuaState != NULL);

	lua_getglobal(m_pLuaState, pszVarName);

	if(!lua_isstring(m_pLuaState, -1))
	{
		lua_settop(m_pLuaState, 0);

		return 0;
	}

	return lua_tostring(m_pLuaState, -1);
}

DOUBLE LuaManager::GetGlobalVarDouble( const char* pszVarName )
{
	ASSERT(m_pLuaState != NULL);

	lua_getglobal(m_pLuaState, pszVarName);

	if(!lua_isnumber(m_pLuaState, -1))
	{
		lua_settop(m_pLuaState, 0);

		return 0;
	}

	return lua_tonumber(m_pLuaState, -1);
}


BOOL LuaManager::GetStackValue_Ptr(INT32 nStackIndex, VOID*& ptrValue)
{
	if(!lua_isuserdata(m_pLuaState, nStackIndex))
	{
		return FALSE;
	}


	ptrValue = lua_touserdata(m_pLuaState, nStackIndex);

	return TRUE;
}

BOOL LuaManager::GetStackValue_Int(INT32 nStackIndex, INT32& intValue)
{
	if(!lua_isnumber(m_pLuaState, nStackIndex))
	{
		return FALSE;
	}

	intValue = (INT32)lua_tointeger(m_pLuaState, nStackIndex);

	return TRUE;
}

BOOL LuaManager::GetStackValue_Double(INT32 nStackIndex, DOUBLE& doubleValue)
{
	if(!lua_isnumber(m_pLuaState, nStackIndex))
	{
		return FALSE;
	}

	doubleValue = lua_tonumber(m_pLuaState, nStackIndex);

	return TRUE;
}

BOOL LuaManager::GetStackValue_String(INT32 nStackIndex, const CHAR*& strValue)
{
	if(!lua_isstring(m_pLuaState, nStackIndex))
	{
		return FALSE;
	}

	strValue = lua_tostring(m_pLuaState, nStackIndex);

	return TRUE;
}

BOOL LuaManager::CallLuaFunction( std::string strFuncName, char* pStrParamSig, ... )
{
	if(pStrParamSig == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	char* pInParam = pStrParamSig;
	char* pOutParam = strchr(pStrParamSig, '=');
	if(pOutParam == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	int nStackTop = lua_gettop(m_pLuaState);
	pOutParam += 1;

	va_list VarList;
	va_start(VarList, pStrParamSig);

	lua_getglobal(m_pLuaState, strFuncName.c_str());

	if(!lua_isfunction(m_pLuaState, -1))
	{
		va_end(VarList);
		lua_settop(m_pLuaState, nStackTop);
		ASSERT_FAIELD;
		return FALSE;
	}

	BOOL	bInParamEnd		= FALSE;
	int		nInParamCount	= 0;
	int		nOutParamCount  = (int)strlen(pOutParam);

	while(!bInParamEnd)
	{
		luaL_checkstack(m_pLuaState, 1, "too many arguments");
		char cParmSig = *pInParam;
		switch(cParmSig)
		{
			case 'd':
				lua_pushnumber(m_pLuaState, va_arg(VarList, double));
				nInParamCount++;
				break;
			case 'i':
				lua_pushinteger(m_pLuaState, va_arg(VarList, int));
				nInParamCount++;
				break;
			case 'f':
				lua_pushnumber(m_pLuaState, va_arg(VarList, double));
				nInParamCount++;
				break;
			case 's':
				lua_pushstring(m_pLuaState, va_arg(VarList, char*));
				nInParamCount++;
				break;
			case 'p':
				lua_pushlightuserdata(m_pLuaState, va_arg(VarList, void*));
				nInParamCount++;
				break;
			case 'b':
				lua_pushboolean(m_pLuaState, va_arg(VarList, bool));
				nInParamCount++;
				break;
			case '=':
				bInParamEnd = TRUE;
				break;
			default:
			{
				ASSERT_FAIELD;
			}
			break;
		}

		pInParam++;
	}

	if(lua_pcall(m_pLuaState, nInParamCount, nOutParamCount, 0) != 0)
	{
		const char* sresult = lua_tostring(m_pLuaState, -1);
		va_end(VarList);
		lua_settop(m_pLuaState, nStackTop);
		return FALSE;
	}

	INT32 nRetIndex = -nOutParamCount;
	BOOL bOutParamEnd = FALSE;
	while(!bOutParamEnd)
	{
		char cParmSig = *pOutParam;
		switch(cParmSig)
		{
			case 'd':
				ASSERT(lua_isnumber(m_pLuaState, nRetIndex));
				*va_arg(VarList, double*) = lua_tonumber(m_pLuaState, nRetIndex);
				break;
			case 'i':
				ASSERT(lua_isnumber(m_pLuaState, nRetIndex));
				*va_arg(VarList, int*) = (int)lua_tointeger(m_pLuaState, nRetIndex);
				break;
			case 'f':
				ASSERT(lua_isnumber(m_pLuaState, nRetIndex));
				*va_arg(VarList, float*) = (float)lua_tonumber(m_pLuaState, nRetIndex);
				break;
			case 's':
				ASSERT(lua_isstring(m_pLuaState, nRetIndex));
				*va_arg(VarList, const char**) = lua_tostring(m_pLuaState, nRetIndex);
				break;
			case 'p':
				ASSERT(lua_isuserdata(m_pLuaState, nRetIndex));
				*va_arg(VarList, void**) = lua_touserdata(m_pLuaState, nRetIndex);
				break;
			case 'b':
				ASSERT(lua_isboolean(m_pLuaState, nRetIndex));
				*va_arg(VarList, bool*) = lua_toboolean(m_pLuaState, nRetIndex);
				break;
			case 0:
				bOutParamEnd = TRUE;
				break;
			default:
			{
				ASSERT_FAIELD;
			}
			break;
		}

		nRetIndex++;
		pOutParam++;
	}

	va_end(VarList);
	lua_settop(m_pLuaState, nStackTop);

	return TRUE;
}

BOOL LuaManager::LoadScriptFile(const char* pszLuaFile)
{
	if(m_pLuaState == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(luaL_dofile(m_pLuaState, pszLuaFile))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL LuaManager::LoadScriptFile( std::vector<std::string>& vtScriptList )
{
	for(std::vector<std::string>::iterator itor = vtScriptList.begin(); itor != vtScriptList.end(); itor++)
	{
		if(!LoadScriptFile(itor->c_str()))
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL LuaManager::GetStackParams( char* pStrParamSig, ... )
{
	BOOL bRet		= TRUE;

	va_list VarList;
	va_start(VarList, pStrParamSig);

	int nArgNum = (int)strlen(pStrParamSig);
	if(nArgNum < 0)
	{
		va_end(VarList);
		lua_settop(m_pLuaState, 0);
		ASSERT_FAIELD;
		return FALSE;
	}

	for(int i = 0; i < nArgNum; i++)
	{
		luaL_checkstack(m_pLuaState, 1, "too many arguments");

		char cParmSig = pStrParamSig[i];

		switch(cParmSig)
		{
			case 'd':
			{
				if(!lua_isnumber(m_pLuaState, i + 1))
				{
					bRet = FALSE;
					break;
				}

				*va_arg(VarList, double*) = lua_tonumber(m_pLuaState, i + 1);
			}
			break;
			case 'i':
			{
				if(!lua_isnumber(m_pLuaState, i + 1))
				{
					bRet = FALSE;
					break;
				}

				*va_arg(VarList, int*) = (int)lua_tointeger(m_pLuaState, i + 1);
			}
			break;
			case 'f':
			{
				if(!lua_isnumber(m_pLuaState, i + 1))
				{
					bRet = FALSE;
					break;
				}

				*va_arg(VarList, float*) = (float)lua_tonumber(m_pLuaState, i + 1);
			}
			break;
			case 's':
			{
				if(!lua_isstring(m_pLuaState, i + 1))
				{
					bRet = FALSE;
					break;
				}

				*va_arg(VarList, const char**) = lua_tostring(m_pLuaState, i + 1);
			}
			break;
			case 'p':
			{
				if (!lua_isstring(m_pLuaState, i + 1))
				{
					bRet = FALSE;
					break;
				}
				*va_arg(VarList, void**) = lua_touserdata(m_pLuaState, i + 1);
			}
			break;
			case 'b':
			{
				if(!lua_isboolean(m_pLuaState, i + 1))
				{
					bRet = FALSE;
					break;
				}

				*va_arg(VarList, bool*) = (bool)lua_toboolean(m_pLuaState, i + 1);
			}
			break;
			default:
			{
				bRet = FALSE;
			}
			break;
		}
	}

	va_end(VarList);
	lua_settop(m_pLuaState, 0);

	return bRet;
}

BOOL LuaManager::RegisterFunction( const char* libname, const luaL_Reg* l )
{
	//luaL_register(m_pLuaState, libname, l);

	return TRUE;
}

BOOL LuaManager::RegisterFunction(const char* name, lua_CFunction fn)
{
	lua_register(m_pLuaState, name, fn);

	return TRUE;
}


/*
LuaManager luamgr;

luamgr.Init();
luamgr.RegisterFunction("TestAdd", ToLua_TestAdd);
luamgr.LoadAllLua("F:\\GameRoot\\Server\\Lua\\Debug");

int value = luamgr.GetGlobalVarInt("a");


int a = 10;
int b = 20;
int c = 0;
TestLuaClass LuaClass;
luamgr.CallLuaFunction("add", "iip=i", a, b, &LuaClass, &c);
*/