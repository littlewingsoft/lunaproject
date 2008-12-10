// crc32Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <map>
#include <cassert>

#include "crc32.h"

using namespace std;

/*
-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
	- 제공되는 CRC32테이블( CRC32_SRC ) 내용.
	0. HEADER
	   {
			DWORD	svnRevision; 
			DWORD	recordCnt;
	   };
	for( recordCnt )
	{
		 부모경로/파일명 (4바이트 체크섬 키)
		 파일자체의내용. (4바이트 체크섬 값)
	}

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
	제공되는 체크섬 테이블 파일에서
	map< DWORD,DWORD > 로 된 현재 게임 폴더의 crc32 테이블( CRC32_INSTALL )을 만듦  

	- 런쳐에서 생성할 파일목록 의 내용.
	인스톨된 게임의 모든 키를 다 순회하며
	for( 모든파일의 키 )
	{
		if( CRC32_SRC 에 키가 존재?  )
		{
			if( CRC32_SRC의 값과 CRC32_INSTALL 의 값이 같음? )
				CRC32_SRC 에서 현재 키를 삭제.
			else
				CRC32_SRC 와 값이 다르므로 변조 되었음 게임실행불가. UnInstall후 재설치또는 수동설치 권장.
		}
	}

	if( CRC32_SRC 에 키가 남았다면? )
	{
		CRC32_SRC 와 값이 다르므로 변조 되었음 게임실행불가. UnInstall후 재설치또는 수동설치 권장.
	}

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

*/


wstring InstallPath = L"";
map< DWORD, DWORD> convmap;

	void SaveXml( const string& fileName )
	{
		// xml 로 저장.
	}

	void LoadBin( map<DWORD,DWORD>& crcData, const wstring& fileName )
	{
		HANDLE hFile = CreateFile( fileName.c_str(),GENERIC_READ, 0,  NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	}

	void SaveBin( const wstring& fileName )
	{
		HANDLE hFile = CreateFile( fileName.c_str(),GENERIC_WRITE, 0,  NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		assert( hFile != INVALID_HANDLE_VALUE );
		DWORD dwWritten=0;
		DWORD revision = 0x00ff00ff;
		size_t cnt = convmap.size();
		WriteFile( hFile, &revision ,sizeof( revision ), &dwWritten,0);
		WriteFile( hFile, &cnt ,sizeof( cnt ),&dwWritten,0);
		
		map<DWORD,DWORD>::iterator it = convmap.begin();
		while( it != convmap.end() )
		{
			DWORD key = (*it).first;
			DWORD value = (*it).second;
			WriteFile( hFile, &key, sizeof( key ),&dwWritten,0);
			WriteFile( hFile, &value, sizeof( value ),&dwWritten,0);
			it++;
		}
		CloseHandle( hFile );
	}



	void RecursiveDir(const wstring& rootPath )
	{
		// 현재 지정된currPath 의 모든 파일과 폴더를 돌아요.
		WIN32_FIND_DATA lpFindFileData;
		HANDLE hFirstFile = FindFirstFile( rootPath.c_str(), &lpFindFileData );
		assert( hFirstFile != INVALID_HANDLE_VALUE );

			size_t offset = rootPath.find_first_of( L"*" );
			wstring fullpath  = rootPath.substr( 0,offset );
			wstring name = lpFindFileData.cFileName;
			fullpath += name;

		while( 1 )
		{
			if( name == L"." || name == L".." )
				goto findNext;

			if( lpFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) // directory
			{
				wstring searchPath = fullpath + L"\\*";
				RecursiveDir( searchPath );
			}
			else //file
			{
				//make crc

				
				wstring keyStr = fullpath.substr( fullpath.find_first_not_of( InstallPath ) );
				wprintf( L"fullPath: %s \n" , keyStr.c_str()  );
				//DWORD key = GetCRC32Code( (void*)keyStr.c_str(), keyStr.length() );
				DWORD key = update_crc( 0x05181818, (char*)keyStr.c_str(), keyStr.length() );
				DWORD value = FileTocrc32( fullpath );
				wprintf( L"fullPath: [0x%08x]\tcontents: [0x%08x]\t \n" , key, value );
				assert( convmap.find( key ) == convmap.end() );
				convmap[key]=value;

				
			}

findNext:
			if( FindNextFile( hFirstFile, &lpFindFileData ) == FALSE )
			{
				break;
			}

			size_t offset = rootPath.find_first_of( L"*" );
			fullpath  = rootPath.substr( 0,offset );
			name = lpFindFileData.cFileName;
			fullpath +=  name;
		}
	}


#include <iostream>   
#include <locale>   
#include <io.h>   
#include <fcntl.h>   
  

int _tmain(int argc, _TCHAR* argv[] )
{
	// root 부터 SubDirectory의 LeafNode 까지 돌면서 crc값을 만들어 셋팅.
	// 모두 남김없이 돌아.
	// 텍스트 파일로 저장.
	// bin 파일로도 저장.

	//std::wcout.imbue( std::locale("kor") );

	_setmode(_fileno(stdout), _O_U16TEXT);   

	wstring rootPath = L"";

	wstring tt = L"List.obj";
	
//	DWORD dd= GetCRC32Code( (void*)L"launcher\\List.obj", 17 );
//	dd= GetCRC32Code( (void*)L"launcher\\md5c.obj", 17 );
	DWORD dd= update_crc( 0x05181818, "launcher\\List.obj", 17 );
	dd= update_crc( 0x05181818, "launcher\\md5c.obj", 17 );

	if( argc <= 1 )
	{
		printf( "you must Include Root Directory " );
		rootPath = L"c:\\*";
	}else
		rootPath = argv[1];

	InstallPath = rootPath.substr( 0, rootPath.find_first_of( L"*") );

	RecursiveDir( rootPath  );
	SaveXml( "crc32Checksum.xml"  );
	SaveBin( L"crc32Checksum.bin" );

	

	return 0;
}

