#pragma once

// �ϳ��� ������ 4����Ʈ crc32 üũ�������� ����
DWORD FileTocrc32( const std::wstring& name );

// ������ ���̸�ŭ�� ������ 4����Ʈ crc32 üũ�������� ����.
DWORD GetTableCRC32(const char * buf, size_t len);

