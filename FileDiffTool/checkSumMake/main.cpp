// crc32Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


#include <iostream>   
#include <locale>   
#include <io.h>   
#include <fcntl.h>   
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

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
// multibyte로 변경 
inline char* W2M(  const wstring& srcUniString  )
{
	static char multiBuff[1024] ={0,}; 
	// unicode로 변경 
	int Len = WideCharToMultiByte( 0, WC_NO_BEST_FIT_CHARS | WC_COMPOSITECHECK | WC_DEFAULTCHAR,//0,//
		(LPCWSTR)srcUniString.c_str(), (int)srcUniString.length(), multiBuff, 1024, NULL, NULL); 

	//destMultibyteString = multiBuff;
	multiBuff[Len]=0;
	return multiBuff;
}



wstring InstallPath = L"";
struct nameCrc
{
	string name;
	DWORD crc32;
};

map< DWORD, nameCrc > convmap;

#include "tinyxml/tinyxml.h"

void SaveXml( const string& fileName )
{
	TiXmlDocument doc( fileName.c_str() );

	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlElement * element = new TiXmlElement( "HEADER" );
	int ver = 0xff18;
	element->SetAttribute( "VER", ver );
	element->SetAttribute( "CNT", (int)convmap.size() );

	TiXmlComment* com = new TiXmlComment("crc32 converted");
	doc.LinkEndChild( com );

	doc.LinkEndChild( decl );

	map<DWORD, nameCrc>::iterator it = convmap.begin();

	while( convmap.end() != it )
	{
		nameCrc t = (*it).second;
		DWORD key = ((*it).first);
		char buff[256]={0,};
		//sprintf_s( buff,256,"0x%0x", key );
		TiXmlElement * tmpElement 	= new TiXmlElement ( "ELEMENT" );
		sprintf_s( buff,256,"0x%0x" ,t.crc32 );
		tmpElement ->SetAttribute( "crc32",buff  );
		tmpElement ->SetAttribute( "fileName", t.name.c_str() );

		element->LinkEndChild( tmpElement );				
		it++;
	}

	doc.LinkEndChild( element );		

	doc.SaveFile();
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

	map<DWORD,nameCrc>::iterator it = convmap.begin();
	while( it != convmap.end() )
	{
		DWORD key = (*it).first;
		DWORD value = (*it).second.crc32;
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
		if( name == L"." || name == L".." || name == L".svn" )
			goto findNext;

		if( lpFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) // directory
		{
			wstring searchPath = fullpath + L"\\*";
			RecursiveDir( searchPath );
		}
		else //file
		{
			//make crc


			wstring keyStr = fullpath.substr( fullpath.find_first_not_of( InstallPath ) , fullpath.length() );
			wprintf( L"fullPath: %s \n" , keyStr.c_str()  );
			const char* szTmpStr = W2M( keyStr.c_str() );
			size_t orglen = fullpath.size();
			orglen = fullpath.length();
			size_t len = strlen( szTmpStr );
			//DWORD key = GetCRC32Code( (void*)keyStr.c_str(), len );
			DWORD key = GetTableCRC32( szTmpStr,len );

			DWORD value = FileTocrc32( fullpath );
			wprintf( L"fullPath: [0x%08x]\tcontents: [0x%08x]\t \n" , key, value );
			assert( convmap.find( key ) == convmap.end() );
			nameCrc tmpT;
			tmpT.crc32 = value;
			tmpT.name = szTmpStr;
			convmap[key]=tmpT;


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




int _tmain(int argc, _TCHAR* argv[] )
{
	if( argc <= 1 )
	{
		printf( "Root Directory 를 입력해주세요. \nex) crc32Text.exe c:\\*\n" );

		return -1;
	}
	else
	{
		wstring rootPath = L"";
		rootPath = argv[1];
		_setmode(_fileno(stdout), _O_U16TEXT);   


		InstallPath = rootPath.substr( 0, rootPath.find_first_of( L"*") );

		RecursiveDir( rootPath  );
		SaveBin( L"crc32Checksum.bin" ); // 주로 사용할 바이너리 파일. 용량이 적다.
		SaveXml( "crc32Checksum.xml"  ); // 일단 검증용. 나중에 다른툴에서 사용도 가능함.

	}


	return 0;
}

