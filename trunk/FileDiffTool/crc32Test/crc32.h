#pragma once

// �ϳ��� ������ 4����Ʈ crc32 üũ�������� ����
DWORD FileTocrc32( const std::wstring& name );

// ������ ���̸�ŭ�� ������ 4����Ʈ crc32 üũ�������� ����.
DWORD GetCRC32Code( const void* pkSrc, size_t length );

unsigned long update_crc(unsigned long crc_accum, char *data_blk_ptr,int data_blk_size);