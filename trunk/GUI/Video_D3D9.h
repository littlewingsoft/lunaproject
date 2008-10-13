// D3D.h: interface for the CD3D class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma comment(lib, "dxguid.lib"  )
#pragma comment(lib, "dxerr9.lib"  )	
#pragma comment(lib, "d3d9.lib"  )
#pragma comment(lib, "d3dx9.lib" )

#include <cassert>
#include <d3dx9.h>
#include <TCHAR.h>

#pragma warning(disable: 4786) 
#include <string>
#include <map>
using namespace std;

typedef basic_string<TCHAR> tstring;

#define DEF_WINWIDTH	800
#define DEF_WINHEIGHT	600

#ifndef STRICT
#define STRICT
#endif
#define WIN32_LEAN_AND_MEAN
/*
#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION         0x0801
#endif	
*/
#define DX_PI    ((FLOAT)  3.141592654f)
#define DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

//한픽셀의 흑백비율 31 53 16
#define DRGBA(r,g,b,a)      ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define DRGB(r,g,b)      ((D3DCOLOR)(((0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define GETA(rgb) ((BYTE)((rgb)>>24))
#define GETR(rgb) ((BYTE)((rgb)>>16))
#define GETG(rgb) ((BYTE)(((WORD)(rgb)) >> 8))
#define GETB(rgb) ((BYTE)(rgb))

#define GETRGB(rgb) ((DWORD)(rgb<<8) )

struct UNION_ARGB
{

      union {
			struct { 
						DWORD _B:8;
				        DWORD _G:8;
						DWORD _R:8;
						DWORD _A:8;
					};
			DWORD _ARGB;
       };

	UNION_ARGB(BYTE A,BYTE R, BYTE G, BYTE B)
	{
		_A=A;
		_R=R;
		_G=G;
		_B=B;
	}

	//UNION_ARGB(){}
	UNION_ARGB( DWORD clr )
	{		
		_ARGB = clr; 
	}
	void operator = (DWORD argb)
	{ 
	  _ARGB = argb; 
	}


};
	//UNION_ARGB aaa = 0xffaabbcc;
	//aaa = 0xaaaaaaaa;
	//UNION_ARGB ttt = aaa;

#define D3D_MIRRORNONE			0 //  안바꿈 
#define D3D_MIRRORLEFTRIGHT		1 //  좌우를 바꿈
#define D3D_MIRRORUPDOWN		2 //  위아래를 바꿈 

#define FC    __fastcall
/*
#define MAX_TEXTURE		640
#define MAX_DRAWPARAM		792
#define MAX_LAYERCNT		7
*/
#define D3D_MAX_FAR_LAYER0		0 //  맨뒤, 내눈에서 먼쪽방향으로의 최고끝 레이어 
#define D3D_MAX_FAR_LAYER1		1
#define D3D_MAX_FAR_LAYER2		2
#define D3D_Y_SORT_LAYER0		3 //  y축 정렬이 필요한 레이어
#define D3D_INTERFACE_LAYER0	4	// GUI 들..
#define D3D_INTERFACE_LAYER1	5	//
#define D3D_INTERFACE_LAYER2	6	// 포커스 올라간 GUI


enum D3DCONST
{	

	D3D_NORMALSTATE=0,D3D_GASANSTATE=1,D3D_ALPHASTATE=2	,
	MAX_TEXTURE=	640, MAX_DRAWPARAM=	792,MAX_LAYERCNT=7,
};


class IDrawObj
{
public:
	IDrawObj(){}
	~IDrawObj(){}
	virtual void Draw() = 0;
};



class CBltSprite : public IDrawObj 
{
protected :
	int				m_TextureIndex;		// 텍스쳐의 인덱스 
	RECT			m_srcRt;			// 원본에서 떼어올 사각형값 
	RECT			m_destRt;
	D3DCOLOR		m_Color;			// 알파와 모듈레이팅 하는 값 
//	D3DXVECTOR2		m_Scaling;			// 확대및 축소를 할 값
//	D3DXVECTOR2		m_Center;			// 회전,이동시 쓰일 중점의 값 기본은 0,0 임
//	float			m_Rotation;			// 회전값 
//	D3DXVECTOR2		m_Translation;		// 이동하는값 
	
	DWORD			m_hState;			// 그리기 상태 핸들값
public:
	static int BltSpriteCnt;
	inline float GetSortY(void){ return (float)m_destRt.bottom; /*m_Translation.y;*/ }//m_SortY
	CBltSprite(){}
	~CBltSprite(){}
	//
	//void BltSprite(	int TxtNum, RECT& SrcRect, RECT& DestRect );
	//void BltSprite(	int TxtNum, float x,float y,D3DCONST RenderMODE = D3D_NORMALSTATE );
	//void BltSprite(	int TxtNum, RECT& SrcRect, RECT& DestRect,D3DCOLOR Clr,D3DCONST RenderMODE = D3D_NORMALSTATE  );

	//void BltSprite(	int TxtNum, RECT &SrcRect, RECT& DestRect,float rad,	 
	//				POINT rcenter, D3DCOLOR Clr, D3DCONST RenderMODE = D3D_NORMALSTATE  );

//	void BltSprite(	int TxtNum, const RECT& SrcRect, const RECT & DestRect, 
//				DWORD dwFlags, D3DCOLOR Modulate, float rad, POINT *pCenter, D3DCONST RenderMODE /*D3D_NORMALSTATE*/ ); //= D3D_NORMALSTATE

	void BltSprite(	int TxtNum, const RECT& SrcRect, const RECT & DestRect, D3DCOLOR Modulate, D3DCONST RenderMODE /*D3D_NORMALSTATE*/ ); //= D3D_NORMALSTATE

	//////////////////// abstract Func
	void Draw();
};


///////////////////////////////////// Sprite Declare End

class CBltText: public IDrawObj
{
protected:
	RECT		m_BoundRect;
	D3DCOLOR	m_Clr;
	TCHAR		m_StrBuff[256];
	DWORD		m_Flag ;
public:
	static int BltTextCnt;

	CBltText(){}
	~CBltText(){}

	void BltText( int x, int y, D3DCOLOR rgbFontColour, const TCHAR* pText );
	void BltText( RECT& rt, D3DCOLOR rgbFontColour, const TCHAR* pText );
	void Draw();
};
///////////////////////////////////// Font Declare End


struct SIMPLETEXTURE
{
	LPDIRECT3DTEXTURE9	lpTexture;
	int					RefCnt;

	void Release();
};

#define rhw
struct SpriteVertex
{
#ifdef rhw
	enum {D3DFVF_SPRITEVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	D3DXVECTOR4 Pos;
	D3DCOLOR Clr;
	float tu,tv;
#else
	D3DXVECTOR3 Pos;
	enum {D3DFVF_SPRITEVERTEX = D3DFVF_XYZ };
#endif
};


class CD3D  
{

public:

	bool IsTransparentColor(int TextureID, int x, int y,DWORD ClrKey=0x00000000 );

	
	D3DXMATRIX m_matTex;
	D3DXMATRIX m_matMoveScale;

	SpriteVertex m_vtx[4];


	void ReleaseANI(int TexID);

	CD3D();
	virtual ~CD3D();
	void	ApplyStateBlock( DWORD Block )
	{
		HRESULT hr = m_hStateBlock[ Block ]->Apply(); assert( hr == D3D_OK); 
	}
	void 	ClearAllParam(void);
	void 	Present(void);
	int		GetTextureCNT(void);
	TCHAR*	GetD3DErr(HRESULT hr );
	HWND	GethWnd(){ return m_hWnd; }
	int		ScreenShot( const TCHAR* fName );	// 현재 화면을 스샷으로 저장해줌 
	void	SetGamma( long Gamma ); // Gamma 0~255
	void 	FillScreen( D3DCOLOR Clr= D3DCOLOR_XRGB(0,128,255) );
	void	ReleaseAll(void);
	
	HRESULT				InitD3D(int wWidth, int wHeight, bool bFull );
	void InitFont( const TCHAR* pFontFace, int nHeight=12, 
					bool fBold=false, bool fItalic=false, bool fUnderlined=false );

//	inline LPD3DXSPRITE		GetSprite(){ return m_Sprite;  }
	inline LPDIRECT3DDEVICE9&	GetDevice(){ return m_D3DDevice; }
	
	int	LoadAni( const TCHAR* fName, RES_ANI& pObj );

	int	RegistWindow( TCHAR* lpSz, HICON hIcon,  WNDPROC WndProc );		
	int GetWinWidth() { return m_WinWidth;  }
	int GetWinHeight(){ return m_WinHeight; }
	inline LPDIRECT3DTEXTURE9&	GetTexture(int Index){ return m_Texture[Index].lpTexture; }
	inline SIZE GetActualImgSize(int Index){ return m_ActualImgSize[Index]; }
	bool	IsFull(void){ return m_bFull; }
	HRESULT Resize3DEnvironment();
	HRESULT ToggleScreen(bool bFull );

	HRESULT MakeStateBlock();
	void DeleteAllStateBlock();
	
	//inline HRESULT Draw(	int Index,  const RECT* pSrcRect,  const D3DXVECTOR2* pScaling,  
	//	const D3DXVECTOR2* pRotationCenter,	float Rotation,	
	//	const D3DXVECTOR2*  pTranslation,  D3DCOLOR Color );
	inline HRESULT Draw(int Index,  const RECT& srcRt, const RECT& destRt, D3DCOLOR Color );
protected:
//	D3DMATRIX* BuildMatrix( D3DMATRIX* pOut, D3DXVECTOR2* pCenter, D3DXVECTOR2* scaling, float angle );
//	void TransformVertices(D3DTLVERTEX vertices[], D3DXVECTOR2* pCenter, D3DXVECTOR2* pScaling,float angle);
//	void PolyBlit(int Index, RECT* pSrc, D3DXVECTOR2* pScaling=NULL,
//	D3DXVECTOR2* pCenter=NULL,  float Rotate=0.0f,D3DXVECTOR2* Trans=NULL, D3DCOLOR colour = 0xffFFFFFF );
public:
	void BltSprite(	BYTE Layer, int TxtNum, float x,float y,D3DCONST RenderMODE = D3D_NORMALSTATE );
	void BltSprite(	BYTE Layer, int TxtNum, RECT *pSrcRect , const RECT&pDestRec );
	void BltSprite(	BYTE Layer, int TxtNum, RECT *pSrcRect ,const RECT &pDestRect,  D3DCOLOR Clr,D3DCONST RenderMODE = D3D_NORMALSTATE);

//	void BltSprite( BYTE Layer, int TxtNum,RECT *pSrcRect ,const  RECT& pDestRect, float rad,	 POINT rcenter, 
//					D3DCOLOR Clr, D3DCONST RenderMODE = D3D_NORMALSTATE );

//	void BltSpriteEx( BYTE Layer, int TxtNum, RECT *pDestRect, RECT *pSrcRect, 
//		DWORD dwFlags, D3DCOLOR Modulate, float rad, POINT *pCenter  );

	void CreateSurfaceFromFile(LPDIRECT3DSURFACE9* ppSurface, SIZE* pSize,
							const TCHAR* szFileName, D3DCOLOR colourKey = 0xFFFF00FF);

	void CreateTextureFromSurface(LPDIRECT3DSURFACE9 pSurface, RECT* pSrcRect,
                              LPDIRECT3DTEXTURE9* ppTexture);

	inline void FillRect(RECT *pRect, D3DCOLOR fillColor);
/*	inline	int	GetTexutreINDEX( TCHAR* tName)
	{ 
			if( TextureMap[ tName ] != NULL ) 
			return 	TextureMap[ tName ]->TexID; 

			return NULL;
	}*/

	HRESULT Texture2Texture( LPDIRECT3DTEXTURE9 Dest, LPDIRECT3DTEXTURE9 Src);
	HRESULT Texture2Texture( int DestIndex, POINT&DestPt, int SrcIndex );
/////////////////////////////////////////////////
//DX 폰트 관련 몽땅 
/////////////////////////////////////////////////
	void	BltText( BYTE Layer,int x, int y, D3DCOLOR rgbFontColour, const TCHAR* pText );
	void	BltText( BYTE Layer,RECT& rt, D3DCOLOR rgbFontColour, const TCHAR* pText );
	HFONT	GetGDIFont(void){ return m_hFont; }
	void	DrawText(RECT& rt, D3DCOLOR rgbFontColour,DWORD Flag,  const TCHAR* pText );
	void	DrawText_CalcRECT( int x, int y, RECT&rt, const TCHAR* pText );

	//스트링을 텍스쳐에올림.외곽,글씨색깔.=0x00000000
	int		Str2Texture( TCHAR* szStr,D3DCOLOR OutLineClr, D3DCOLOR FrontClr, D3DCOLOR BackGroundClr=0x00000000 ); 
	int		Str2Texture_backup( TCHAR* szStr,D3DCOLOR OutLineClr, D3DCOLOR FrontClr, D3DCOLOR BackGroundClr=0x00000000 ); 
	
//	int    pStr2Texture_dev(TCHAR* szStr,D3DCOLOR OutLineClr,D3DCOLOR FrontClr, D3DCOLOR BackGroundClr ); // 
//	int     (CD3D::*Str2Texture)(TCHAR* szStr,D3DCOLOR OutLineClr,D3DCOLOR FrontClr, D3DCOLOR BackGroundClr );

	bool	IsDeviceLost( void) ;
protected: 
	bool	m_bDeviceLost;
	D3DDISPLAYMODE		m_D3DDisplayMode;
	// 그리기 클래스는 256개씩 미리 잡아놈
	CBltSprite			m_BltSprite[ MAX_DRAWPARAM ];
	CBltText			m_BltText  [ MAX_DRAWPARAM ];
	
	IDrawObj*			m_DrawObj   [ MAX_LAYERCNT ][ MAX_DRAWPARAM ];
	int					m_DrawObjCnt[ MAX_LAYERCNT ];

//	DWORD				m_hStateBlock[ 3 ];
	IDirect3DStateBlock9* 	m_hStateBlock[ 3 ];
	D3DVIEWPORT9		m_ViewPort;

	void quick_sort( CBltSprite** Arr, int Size ); // 구조체 버젼 
	int WriteTGAFile(const TCHAR* filename, short int width, short int height, unsigned char* imageData);

	int	LoadIMG( const TCHAR* fName, RES_ANI& pAni );
	int	LoadTGA( const TCHAR* fName, RES_ANI& pAni );

	int						m_DrawParamCnt[ MAX_LAYERCNT ];
	int						m_WinWidth;
	int						m_WinHeight;
	int						m_AvailTextureCnt;
	bool					m_bFull;
	HWND					m_hWnd;
	LPDIRECT3D9				m_D3D;
	LPDIRECT3DDEVICE9		m_D3DDevice;

	SIMPLETEXTURE			m_Texture      [ MAX_TEXTURE ]; // 참조 카운트 추가..
	SIZE					m_ActualImgSize[ MAX_TEXTURE ];
//	LPD3DXSPRITE			m_Sprite;
	LPD3DXFONT				m_DxFont;
	HFONT					m_hFont;
	D3DPRESENT_PARAMETERS	m_d3dpp;


public:
	bool	CreateBlankTexture(int&paramIndex, int Width,int Height );
	void	LoadStream(int & Index,const string&imgstream );
	void	ReleaseTexture(int Index);
	//RES_ANI* GetResAni( const TCHAR* fName );
	RES_ANI* GetResAni( const TCHAR* fName );

};
extern CD3D				g_D3D;
extern LPDIRECT3DDEVICE9		g_p3Dev;

#include <ddraw.h>
void GetVideMem(DWORD & Total, DWORD & Free);



