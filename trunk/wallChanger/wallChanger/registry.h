#pragma once

#include "shlwapi.h"

/*
	Class to encapsulate registry functions and make 
	them a little easier to use.
*/
namespace Registry
{
	// Get a DWORD value from the registry
	DWORD GetRegDWORD(CString cstrKeyName, CString cstrValueName, 
		DWORD dwDefault = 0, HKEY hBaseKey = HKEY_CURRENT_USER);
	

	// Set a DWORD value in the registry
	void SetRegDWORD(CString cstrKeyName, CString cstrValueName, DWORD dwValue, HKEY hBaseKey = HKEY_CURRENT_USER);

	// Get a string value from the registry
	CString GetRegString(CString cstrKeyName, CString cstrValueName, CString cstrDefault = _T(""), HKEY hBaseKey = HKEY_CURRENT_USER);

	// Set a string value in the registry
	void SetRegString(CString cstrKeyName, CString cstrValueName, CString cstrValue, HKEY hBaseKey = HKEY_CURRENT_USER);

	// Delete a value or key from the registry
	BOOL RegDelete(CString cstrKeyName, CString cstrValueName = _T(""), CString cstrSubKeyName = _T(""), HKEY hBaseKey = HKEY_CURRENT_USER);
	
};