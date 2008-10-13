// ResMgr.h: interface for the ResMgr class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <TCHAR.h>
#include <Windows.h>
#pragma warning(disable:4786) 
#include <map>
#include <string>
using namespace std;

typedef basic_string<TCHAR> tstring;

typedef struct tagSPRITEINFOHEADER32 
{
	long			nWidth;
	long			nHeight;
	long			nFrame;
	unsigned long	colorKey;
	long			mspf;
	long			nSpriteSize16;
	long			nSpriteSize32;
	unsigned long	dwReserved1;
	unsigned long	dwReserved2;
	tagSPRITEINFOHEADER32():nWidth(0),nHeight(0),nFrame(0),colorKey(0),mspf(0),nSpriteSize16(0),nSpriteSize32(0),dwReserved1(0),dwReserved2(0){}
} SPRITEINFOHEADER32, *PSPRITEINFOHEADER32;

struct RES_ANI
{
	int					TexID;
	SPRITEINFOHEADER32	sprInfo;
	int	CenterX;	// ���� ���Ŀ� ������ �������ּ��� ������ . 
	int CenterY;
	RES_ANI():TexID(-1),CenterX(0),CenterY(0){}

};// ȭ�鿡 �Ѹ��� ���� �ּ��� ����


// �ε�. �ε��� �ڵ��ȯ(int).
// ����. �ش� �ڵ��� ������ �׿� �ش��ϴ� ���ҽ��� ����
// ����. �ش� �ڵ鰪�� ������ �׿� �ش��ϴ� ���ҽ��� �����͸� ��ȯ 
// 
class ResMgr  
{

private:
	typedef map< tstring, RES_ANI* >	RES_LIST;
	typedef RES_LIST::iterator			RES_ITOR;

	RES_LIST	m_UnitAniList;
	RES_LIST	m_BlockAniList;
	RES_LIST	m_ItemAniList;

	DWORD	m_BlockStyle[256];

public:
	void ReleaseAllUnitAni();
	void ReleaseAllBlockAni();
	void ReleaseAllItemAni();

	RES_ANI*	LoadItemAni (	const TCHAR* fName	);
	RES_ANI*	LoadBlockAni(	const TCHAR* fName	);
	RES_ANI*	LoadUnitAni (	const TCHAR* fName	);

	void SetBlockStyle(int ID,DWORD Style ){ m_BlockStyle[ID] = Style;  }
	DWORD		GetRes_BlockStyle( int ID ){ return m_BlockStyle[ID];  }

	ResMgr();
	virtual ~ResMgr();
	void ReleaseAll(void);
};
extern ResMgr			g_ResMgr;
