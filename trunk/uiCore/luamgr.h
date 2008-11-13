
/*

*/

#pragma once

#include <stdio.h>

struct lua_State;
extern "C" { typedef int (*LuaFunctionType) (struct lua_State* pLuaState); };

class luamgr
{
public:
	luamgr();
	~luamgr();
	bool		Load(const char* pszFileName);
	bool		LoadBinary(const char *pszFileName);

	void		glueFunction(const char* pszFunctionName, LuaFunctionType fpFunction);

	double      GetNumberArgument(int nNum, double dDefault = 0.0f );
	const char* GetStringArgument(int nNum, const char* pszDefault = NULL );
	double		GetNumberGlobalValue(const char* pszValueName);
	const char*	GetStringGlobalValue(const char* pszValueName);

	void		PushString(const char* pszString);
	void		PushNumber(double dValue);

	double		OnLuaFunction(const char* pszFunctionName, int nNum, ...);

private:

	lua_State* m_pScriptContext;
};




