#include <stdafx.h>


namespace Registry
{
	void DisplayErrorMessage(CString cstrMethodName)
	{
		LPVOID lpMsgBuf;
		CString cstrCaption;

		if (GetLastError() != 0)
		{
			cstrCaption.Format( L"Utility Class Error in %s (%d)", cstrMethodName, GetLastError());

			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,
				0, NULL);

			MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)cstrCaption, MB_OK);
		}
	};

	// Get a DWORD value from the registry
	DWORD GetRegDWORD(CString cstrKeyName, CString cstrValueName, 	DWORD dwDefault , HKEY hBaseKey )
	{
		HKEY hKey;
        DWORD dwBufLen = sizeof(DWORD);
		DWORD dwReturn = dwDefault;
		BOOL bError = TRUE;

        if (RegOpenKeyEx(hBaseKey, cstrKeyName, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, cstrValueName, NULL, NULL, (LPBYTE) &dwReturn, &dwBufLen) == ERROR_SUCCESS) 
				bError = FALSE;

			RegCloseKey(hKey);
		}
	
		if (bError) DisplayErrorMessage( L"GetRegDWORD");

		return dwReturn;
	};

	// Set a DWORD value in the registry
	void SetRegDWORD(CString cstrKeyName, CString cstrValueName, DWORD dwValue, HKEY hBaseKey )
	{
		HKEY hKey;
		LONG lRet;
		BOOL bError = TRUE;

		lRet = RegOpenKeyEx(hBaseKey, cstrKeyName, 0, KEY_SET_VALUE, &hKey);
        if (lRet != ERROR_SUCCESS)
			lRet = RegCreateKeyEx(hBaseKey, cstrKeyName, 0, L"", 0, KEY_READ | KEY_WRITE, NULL, &hKey, NULL); 
		
		if (lRet == ERROR_SUCCESS)
		{
			if (RegSetValueEx(hKey, cstrValueName, NULL, REG_DWORD, (LPBYTE) &dwValue, sizeof(DWORD)) == ERROR_SUCCESS)
				bError = FALSE;

			RegCloseKey(hKey);
		}

		if (bError) DisplayErrorMessage( L"SetRegDWORD");
	};

	// Get a string value from the registry
	CString GetRegString(CString cstrKeyName, CString cstrValueName, CString cstrDefault , HKEY hBaseKey )
	{
		HKEY hKey;
        char szValue[MAX_PATH + 1];
		CString cstrReturn = cstrDefault;
		DWORD dwBufLen = MAX_PATH + 1;
		BOOL bError = TRUE;

        if (RegOpenKeyEx(hBaseKey, cstrKeyName, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, cstrValueName, NULL, NULL, (LPBYTE) szValue, 
				&dwBufLen) == ERROR_SUCCESS)
			{
				cstrReturn = szValue;
				bError = FALSE;
			}

			RegCloseKey(hKey);
		}

		if (bError) DisplayErrorMessage( L"GetRegString");

		return cstrReturn;
	};

	// Set a string value in the registry
	void SetRegString(CString cstrKeyName, CString cstrValueName, CString cstrValue, HKEY hBaseKey )
	{
		HKEY hKey;
		LONG lRet;
		BOOL bError = TRUE;

		lRet = RegOpenKeyEx(hBaseKey, cstrKeyName, 0, KEY_SET_VALUE, &hKey);
        if (lRet != ERROR_SUCCESS)
			lRet = RegCreateKeyEx(hBaseKey, cstrKeyName, 0, L"", 0, KEY_READ | KEY_WRITE, NULL, &hKey, NULL); 
		
		if (lRet == ERROR_SUCCESS)
		{
			if (RegSetValueEx(hKey, cstrValueName, NULL, REG_SZ, (LPBYTE) cstrValue.GetBuffer(), cstrValue.GetLength()) == ERROR_SUCCESS)
				bError = FALSE;

			RegCloseKey(hKey);
		}

		if (bError) DisplayErrorMessage( L"SetRegString");
	};

	// Delete a value or key from the registry
	BOOL RegDelete(CString cstrKeyName, CString cstrValueName , CString cstrSubKeyName , 	HKEY hBaseKey )
	{
		HKEY hKey;
		BOOL bRetVal = FALSE;
		BOOL bError = TRUE;

		if (RegOpenKeyEx(hBaseKey, cstrKeyName, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
		{
			if (!cstrValueName.IsEmpty())
			{
				// delete a value
				if (RegDeleteValue(hKey, cstrValueName) == ERROR_SUCCESS)
				{
					bRetVal = TRUE;
					bError = FALSE;
				}
			}
			else
			{
				if (!cstrSubKeyName.IsEmpty())
				{
					// delete an entire key
					if (SHDeleteKey(hKey, cstrSubKeyName) == ERROR_SUCCESS)
					{
						bRetVal = TRUE;
						bError = FALSE;
					}
				}
			}

			RegCloseKey(hKey);
		}

		if (bError) DisplayErrorMessage( L"RegDelete");

		return bRetVal;
	}
}