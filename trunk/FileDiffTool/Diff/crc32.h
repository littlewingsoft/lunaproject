#pragma once

// 하나의 파일을 4바이트 crc32 체크섬값으로 만듦
DWORD FileTocrc32( const std::wstring& name );

// 정해진 길이만큼의 내용을 4바이트 crc32 체크섬값으로 만듦.
DWORD GetTableCRC32(const char * buf, size_t len);

