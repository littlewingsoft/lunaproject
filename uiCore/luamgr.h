
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

	double		GetNumberGlobalValue(const char* pszValueName);
	const char*	GetStringGlobalValue(const char* pszValueName);

	void		SetString(const char* pszString);
	void		SetNumber(double dValue);

	double		OnLuaFunction(const char* pszFunctionName, int nNum, ...);

private:

	lua_State* m_pScriptContext;
};




