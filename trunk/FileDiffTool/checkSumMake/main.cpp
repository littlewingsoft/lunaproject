// crc32Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
// multibyte�� ���� 
inline char* W2M(  const wstring& srcUniString  )
{
	static char multiBuff[1024] ={0,}; 
	// unicode�� ���� 
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
		printf( "Root Directory �� �Է����ּ���. \nex) crc32Text.exe c:\\*\n" );

		return -1;
	}
	else
	{
		wstring rootPath = L"";
		rootPath = argv[1];
		_setmode(_fileno(stdout), _O_U16TEXT);   


		InstallPath = rootPath.substr( 0, rootPath.find_first_of( L"*") );

		RecursiveDir( rootPath  );
		SaveBin( L"crc32Checksum.bin" ); // �ַ� ����� ���̳ʸ� ����. �뷮�� ����.
		SaveXml( "crc32Checksum.xml"  ); // �ϴ� ������. ���߿� �ٸ������� ��뵵 ������.

	}


	return 0;
}

