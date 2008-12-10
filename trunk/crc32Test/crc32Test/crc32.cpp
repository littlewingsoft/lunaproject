
#include <stdafx.h>
#include <windows.h>
#include <string>
#include "crc32.h"
DWORD crc32_table[256]={0,};





DWORD CRC32Reflect( unsigned int ref, unsigned int  bit_Value )
{
	DWORD value = 0,cnt=0;

	for( cnt = 1 ; cnt < (bit_Value+1); cnt++)
	{
		if( ref&1 )
			value |=1 << (bit_Value-cnt);
		ref >>= 1;
	}

	return value;

}



#define CRC32_POLYNOMIAL 0x04c11db7

void CRC32Table( void )
{
	DWORD polynomial = CRC32_POLYNOMIAL;
	DWORD cnt1,cnt2;

	for( cnt1 = 0 ; cnt1 < 256; cnt1++)
	{
		crc32_table[cnt1]= CRC32Reflect( cnt1, 8 ) << 24;
		for( cnt2=0; cnt2<8; cnt2++)
		{
			crc32_table[cnt1]= (crc32_table[cnt1] << 1 ) ^ (crc32_table[cnt1]&( 1<<31 ) ? polynomial:0 );
		}

		crc32_table[cnt1]= CRC32Reflect(crc32_table[cnt1],32);
		
	}
}

#define CRC32_INIT 0xffffffff

DWORD GetCRC32Code( const void* pkSrc, size_t length )
{
	DWORD crc_code= CRC32_INIT;
	unsigned char* pkBuff= (unsigned char* )pkSrc;
	
	while( length -- )
	{
		crc_code = (crc_code>>8) ^ crc32_table[ (crc_code&0xff)^ *pkBuff++];
	}

	return crc_code^ CRC32_INIT;
}


DWORD FileTocrc32( const std::wstring& name )
{
	HANDLE hFile, hFMap;
	void *PtrInFile;

	hFile = CreateFile( name.c_str(), GENERIC_READ, 0,  NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD checkSum =0 ;
	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("Not found file\n");
	}
	else
	{
		// 파일 맵핑 오브젝트를 만든다.
		hFMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		// 주소 공간에 맵한다.
		PtrInFile = (void*)MapViewOfFile(hFMap, FILE_MAP_READ, 0, 0, 0);
		DWORD dwlength = GetFileSize(hFile, 0 );
		//checkSum = GetCRC32Code( PtrInFile, dwlength );
		checkSum = update_crc( 0x05ff00ff, (char*)PtrInFile,dwlength );
		//printf("checkSum:%x\n", checkSum );
		UnmapViewOfFile(PtrInFile);
		CloseHandle(hFMap);
		CloseHandle(hFile);

	}	

	return checkSum;	
}




#define POLYNOMIAL	0x04c11db7L
static unsigned long crc_table[256];

/* generate the table of CRC remainders for all possible bytes */
void gen_crc_table()
{
	register int i, j;
	register unsigned long crc_accum;

	for(i=0; i<256; i++)
	{
		crc_accum = ((unsigned long) i << 24);
		for(j=0; j<8; j++)
		{
			if(crc_accum & 0x80000000L)
				crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
			else
				crc_accum = (crc_accum << 1);
		}
		crc_table[i] = crc_accum;

		//printf("%03d=%lx, ", i, crc_accum);
		if(i%7 == 0)
			printf("\n");
	}
	return;
}


/* update the CRC on the data block one byte at a time */
unsigned long update_crc(unsigned long crc_accum, char *data_blk_ptr,int data_blk_size)
{
	register int i, j;

	for(j=0; j<data_blk_size; j++)
	{
		i = ((int)(crc_accum >> 24) ^ *data_blk_ptr++) & 0xff;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	return crc_accum;
}



struct init_t
{
	init_t()
	{
		CRC32Table();
		gen_crc_table();
	}
};

static init_t tt;
