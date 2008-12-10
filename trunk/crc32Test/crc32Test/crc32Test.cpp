// crc32Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
	- �����Ǵ� CRC32���̺�( CRC32_SRC ) ����.
	0. HEADER
	   {
			DWORD	svnRevision; 
			DWORD	recordCnt;
	   };
	for( recordCnt )
	{
		 �θ���/���ϸ� (4����Ʈ üũ�� Ű)
		 ������ü�ǳ���. (4����Ʈ üũ�� ��)
	}

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
	�����Ǵ� üũ�� ���̺� ���Ͽ���
	map< DWORD,DWORD > �� �� ���� ���� ������ crc32 ���̺�( CRC32_INSTALL )�� ����  

	- ���Ŀ��� ������ ���ϸ�� �� ����.
	�ν���� ������ ��� Ű�� �� ��ȸ�ϸ�
	for( ��������� Ű )
	{
		if( CRC32_SRC �� Ű�� ����?  )
		{
			if( CRC32_SRC�� ���� CRC32_INSTALL �� ���� ����? )
				CRC32_SRC ���� ���� Ű�� ����.
			else
				CRC32_SRC �� ���� �ٸ��Ƿ� ���� �Ǿ��� ���ӽ���Ұ�. UnInstall�� �缳ġ�Ǵ� ������ġ ����.
		}
	}

	if( CRC32_SRC �� Ű�� ���Ҵٸ�? )
	{
		CRC32_SRC �� ���� �ٸ��Ƿ� ���� �Ǿ��� ���ӽ���Ұ�. UnInstall�� �缳ġ�Ǵ� ������ġ ����.
	}

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

*/


wstring InstallPath = L"";
map< DWORD, DWORD> convmap;

	void SaveXml( const string& fileName )
	{
		// xml �� ����.
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
		// ���� ������currPath �� ��� ���ϰ� ������ ���ƿ�.
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
	// root ���� SubDirectory�� LeafNode ���� ���鼭 crc���� ����� ����.
	// ��� ������� ����.
	// �ؽ�Ʈ ���Ϸ� ����.
	// bin ���Ϸε� ����.

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

