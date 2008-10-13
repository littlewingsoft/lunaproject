
//////////////////////////////////////////////////////////////////////
//
// D3D.cpp: implementation of the CD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


CD3D				g_D3D;
LPDIRECT3DDEVICE9		g_p3Dev;
//#define D3DFVF_TLVERTEX D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1

/*
//-----------------------------------------------------------------------------
// Name: D3DUtil_CreateTexture()
// Desc: Helper function to create a texture. It checks the root path first,
//       then tries the DXSDK media path (as specified in the system registry).
//-----------------------------------------------------------------------------
HRESULT D3DUtil_CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
                               LPDIRECT3DTEXTURE9* ppTexture,
                               D3DFORMAT d3dFormat = D3DFMT_UNKNOWN );

//-----------------------------------------------------------------------------
// Name: D3DUtil_SetColorKey()
// Desc: Changes all texels matching the colorkey to transparent, black.
//-----------------------------------------------------------------------------
HRESULT D3DUtil_SetColorKey( LPDIRECT3DTEXTURE9 pTexture, DWORD dwColorKey );

//-----------------------------------------------------------------------------
// Name: D3DUtil_CreateTexture()
// Desc: Helper function to create a texture. It checks the root path first,
//       then tries the DXSDK media path (as specified in the system registry).
//-----------------------------------------------------------------------------
HRESULT D3DUtil_CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
                               LPDIRECT3DTEXTURE9* ppTexture, D3DFORMAT d3dFormat )
{
    // Create the texture using D3DX
    return D3DXCreateTextureFromFileEx( pd3dDevice, strTexture, 
                D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, d3dFormat, 
                D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, ppTexture );
}

//-----------------------------------------------------------------------------
// Name: D3DUtil_SetColorKey()
// Desc: Changes all texels matching the colorkey to transparent, black.
//-----------------------------------------------------------------------------
HRESULT D3DUtil_SetColorKey( LPDIRECT3DTEXTURE9 pTexture, DWORD dwColorKey )
{
    // Get colorkey's red, green, and blue components
    DWORD r = ((dwColorKey&0x00ff0000)>>16);
    DWORD g = ((dwColorKey&0x0000ff00)>>8);
    DWORD b = ((dwColorKey&0x000000ff)>>0);

    // Put the colorkey in the texture's native format
    D3DSURFACE_DESC d3dsd;
    pTexture->GetLevelDesc( 0, &d3dsd );
    if( d3dsd.Format == D3DFMT_A4R4G4B4 )
        dwColorKey = 0xf000 + ((r>>4)<<8) + ((g>>4)<<4) + (b>>4);
    else if( d3dsd.Format == D3DFMT_A1R5G5B5 )
        dwColorKey = 0x8000 + ((r>>3)<<10) + ((g>>3)<<5) + (b>>3);
    else if( d3dsd.Format != D3DFMT_A8R8G8B8 )
        return E_FAIL;

    // Lock the texture
    D3DLOCKED_RECT  d3dlr;
    if( FAILED( pTexture->LockRect( 0, &d3dlr, 0, 0 ) ) )
        return E_FAIL;

    // Scan through each pixel, looking for the colorkey to replace
    for( DWORD y=0; y<d3dsd.Height; y++ )
    {
        for( DWORD x=0; x<d3dsd.Width; x++ )
        {
            if( d3dsd.Format==D3DFMT_A8R8G8B8 )
            {
                // Handle 32-bit formats
                if( ((DWORD*)d3dlr.pBits)[d3dsd.Width*y+x] == dwColorKey )
                    ((DWORD*)d3dlr.pBits)[d3dsd.Width*y+x] = 0x00000000;
            }
            else
            {
                // Handle 16-bit formats
                if( ((WORD*)d3dlr.pBits)[d3dsd.Width*y+x] == dwColorKey )
                    ((WORD*)d3dlr.pBits)[d3dsd.Width*y+x] = 0x0000;
            }
        }
    }

    // Unlock the texture and return OK.
    pTexture->UnlockRect(0);
    return S_OK;
}
*/

typedef map< tstring, RES_ANI >		TEXTUREMAP; 
typedef TEXTUREMAP::iterator			TEXTUREMAPITOR;
TEXTUREMAP	TextureMap;


void SIMPLETEXTURE::Release()
{ 
	RefCnt--;
	if( RefCnt <=0 ) 
	{
		SAFE_RELEASE( lpTexture )
		RefCnt= 0;
	}
}


int CBltSprite::BltSpriteCnt = 0;
int CBltText::BltTextCnt = 0;

void CBltSprite::Draw() // Begin End 사이에 넣어둔다.
{
	g_D3D.ApplyStateBlock( m_hState );
	assert( m_TextureIndex != -1  );
	g_D3D.Draw( m_TextureIndex, this->m_srcRt, this->m_destRt, m_Color ); // 렌더링 상태 까지...
//	g_D3D.Draw( m_TextureIndex,&m_SrcRect, &m_Scaling, &m_Center, m_Rotation,	&m_Translation,	m_Color );
}
         

void CBltSprite::BltSprite(	int TxtNum, const  RECT& SrcRect, const RECT& DestRect,D3DCOLOR Clr,D3DCONST RenderMODE )
{
	assert( BltSpriteCnt < MAX_DRAWPARAM );
 	 int CurrParamCnt = BltSpriteCnt;

	 m_TextureIndex = TxtNum;
	 m_srcRt      =  SrcRect;
	 m_destRt     =  DestRect;
	 m_Color        = Clr;
	 m_hState = RenderMODE;
}

///////////////////////////////////////////////////// 블리트폰트 시작 입니당.

void CBltText::Draw()
{
	g_D3D.DrawText( m_BoundRect, m_Clr,m_Flag, m_StrBuff );
}

void CBltText::BltText( int x, int y, D3DCOLOR rgbFontColour, const TCHAR* pText )
{ 
	  m_BoundRect.right = 0;
		m_BoundRect.bottom = 0;
		m_BoundRect.left = 0;
		m_BoundRect.top  = 0;
	g_D3D.DrawText_CalcRECT( x,y, m_BoundRect,pText);
	m_Clr = rgbFontColour;
	_tcscpy( m_StrBuff, pText );
	m_Flag = DT_LEFT ;
}

void CBltText::BltText( RECT& rt, D3DCOLOR rgbFontColour, const TCHAR* pText )
{
	m_BoundRect = rt;
	m_Clr = rgbFontColour;
	_tcscpy( m_StrBuff, pText );
	m_Flag = DT_LEFT | DT_VCENTER | DT_WORDBREAK ;
}

///////////////////////////////////////////////////// 블리트폰트 끝 입니당.

//-----------------------------------------------------------------------
CD3D::CD3D()
{
		m_WinWidth		= DEF_WINWIDTH;
		m_WinHeight		= DEF_WINHEIGHT;
		m_AvailTextureCnt	= 0;

		m_bFull			= false;
		m_hWnd			= NULL;
		m_hFont			= NULL;

		m_D3D			= NULL;
		m_D3DDevice		= NULL; // 장치관리자
		m_DxFont		= NULL;

		for( DWORD i=0; i< MAX_TEXTURE; i++)
		{
			m_Texture[i].lpTexture	= NULL;
			m_Texture[i].RefCnt		= 0;
		}
}


CD3D::~CD3D()
{
//	ReleaseAll();
	int a=0;
}

void CD3D::DeleteAllStateBlock()
{
	if( m_D3DDevice == NULL ) return;
	
	for( int i=0; i< 3; i++)
	{
		SAFE_RELEASE( m_hStateBlock[i]);
	}
}

void CD3D::ReleaseAll()
{
	if( m_hFont != NULL )
	{
		DeleteObject((HFONT) m_hFont);
		m_hFont = NULL;
	}

	DeleteAllStateBlock();
//	SAFE_RELEASE( m_Sprite );
	// 텍스쳐 모두 날립니다.
	for( int i=0; i<MAX_TEXTURE;i++)
	{
		SAFE_RELEASE( m_Texture[i].lpTexture );
		m_Texture[i].RefCnt = 0;
	}
	m_AvailTextureCnt = 0; // 모든 텍스쳐는 날아감 
	SAFE_RELEASE( m_DxFont );	
#ifdef _DEBUG
	if( m_D3DDevice != NULL)
	{
		int remain = m_D3DDevice->Release();
		if( remain >0)
			MessageBox( 0,"자원해제 완전히 안됨","D3D ERROR",MB_OK);
		m_D3DDevice= NULL;
	}
#else
		SAFE_RELEASE( m_D3DDevice );
#endif

	SAFE_RELEASE( m_D3D );


}
int	CD3D::GetTextureCNT(void)
{
	m_AvailTextureCnt = 0;
	for( int i=0; i< MAX_TEXTURE; i++ )
	{
		if( m_Texture[ i ].lpTexture != NULL ) 
			m_AvailTextureCnt++;
	}
	return m_AvailTextureCnt; 
}

HRESULT CD3D::InitD3D( int wWidth, int wHeight, bool bFull )
{
	m_bDeviceLost= false;
	m_bFull = bFull;
    // Direct3D 버젼 쿼리 
	SAFE_RELEASE( m_D3D )
    if(!(m_D3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(m_hWnd,  _T("제발 DirectX를 9.0b이상의 최신버젼으로 업그레이드해주세요"),_T("초기화에러"),MB_OK );
#ifdef _HTMLLOG
		g_HtmlLog.LogError( _T("File: %s , Line: %d,  Err: %s"), __FILE__, __LINE__, 
			                _T("제발 DirectX를 9.0b이상의 최신버젼으로 업그레이드해주세요") );
#endif
        return E_FAIL;  
	}
    // 디스플레이 모드 가져오기 
    
    if( FAILED( m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&m_D3DDisplayMode) ) )
	{
		MessageBox(m_hWnd, _T("현재 디스플레이 정보가가져오기 실패"), _T("초기화에러"), MB_OK );
#ifdef _HTMLLOG
		g_HtmlLog.LogError( _T("File: %s , Line: %d,  Err: %s"), __FILE__, __LINE__, 
			                _T("현재 디스플레이 정보가가져오기 실패") );
#endif
        return E_FAIL;  
	}
	m_WinWidth = wWidth;
	m_WinHeight = wHeight;
    ZeroMemory( &m_d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
//	if( m_bFull == true )
	{
		m_d3dpp.BackBufferWidth		= m_WinWidth;
		m_d3dpp.BackBufferHeight	= m_WinHeight;
		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//D3DPRESENT_RATE_UNLIMITED;////;
	}
	    m_d3dpp.Windowed          = m_bFull ? FALSE : TRUE; // D3DSWAPEFFECT_COPY_VSYNC;// 
//		m_d3dpp.SwapEffect        = m_d3dpp.Windowed ? D3DSWAPEFFECT_FLIP :D3DSWAPEFFECT_COPY  ;//D3DSWAPEFFECT_DISCARD;// |     D3DSWAPEFFECT_DISCARD    
		m_d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.hDeviceWindow     = m_hWnd;
		m_d3dpp.BackBufferFormat  = m_bFull ?D3DFMT_R5G6B5 : m_D3DDisplayMode.Format;
		m_d3dpp.BackBufferCount   = 1;
		m_d3dpp.EnableAutoDepthStencil = FALSE;
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_ONE;//D3DPRESENT_INTERVAL_DEFAULT  
		m_d3dpp.EnableAutoDepthStencil= FALSE;
		m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
//		m_d3dpp.Flags = 0;
		m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; // SetDialogBoxMode 를 쓰려면 어쩔수 없음 

	assert( m_hWnd != NULL );
	// 장치만들기 
	HRESULT hr=D3D_OK;
	// Set default settings
	UINT AdapterToUse=D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL;
	// Look for 'NVIDIA NVPerfHUD' adapter
	// If it is present, override default settings
	for (UINT Adapter=0;Adapter<m_D3D->GetAdapterCount();Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res=m_D3D->GetAdapterIdentifier(Adapter,0,&Identifier);
		if (strcmp(Identifier.Description,"NVIDIA NVPerfHUD")==0)
		{
			AdapterToUse=Adapter;
			DeviceType=D3DDEVTYPE_REF;
			break;
		}
	}
	DWORD beg = timeGetTime();
	Log( "비디오 초기화 [ 시작 ]\n");
	hr = m_D3D->CreateDevice( AdapterToUse,DeviceType,	m_hWnd,
									  D3DCREATE_HARDWARE_VERTEXPROCESSING,  &m_d3dpp, &m_D3DDevice );
	if( FAILED( hr ) )
	{
		hr = m_D3D->CreateDevice( AdapterToUse,DeviceType,	m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,  &m_d3dpp, &m_D3DDevice );
		if( FAILED( hr ) )
		{
			TCHAR* arr = GetD3DErr(hr);
			char ccc[32]={0,};
			wsprintf(ccc,_T("mode enum.. %d"), m_D3DDisplayMode.Format );
			MessageBox( m_hWnd, arr,_T("D3D 초기화에러"),MB_OK);
			Log( "비디오 초기화 [ 실패 ]\n");
			Log( _T("File: %s , Line: %d,  Err: %s \n"), __FILE__, __LINE__,arr );
			Log( _T("mode enum.. %d"), m_D3DDisplayMode.Format );
			return FALSE;
		}
	}
	Log( "비디오 초기화 [ 끝:%d ]\n", timeGetTime()-beg);

	if( m_bFull)
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_WinWidth, m_WinHeight, SW_SHOW);

//  m_D3DDevice->GetViewport( &m_ViewPort );
	MakeStateBlock();

	D3DFORMAT fmt =	D3DFMT_A8R8G8B8; //D3DFMT_R5G6B5;// D3DFMT_A4R4G4B4;
	DWORD width =16,height=16;
	UINT pNumMipLevels =5;
//	hr = D3DXCheckTextureRequirements(m_D3DDevice, (UINT*)&width,(UINT*)&height, &pNumMipLevels,
//		D3DUSAGE_RENDERTARGET, &fmt, D3DPOOL_MANAGED );
	if( hr != D3D_OK)
	{
		char* aa = this->GetD3DErr( hr );
//		MessageBox(this->m_hWnd, aa,"그래픽 카드가 너무 구식입니다. 안전한 실행을 보장할수없습니다.\n",MB_OK);
		Log( "그래픽 카드가 너무 구식입니다. 안전한 실행을 보장할수없습니다.\n");
//		return hr;
	}
//	FillScreen();
 	m_D3DDevice->SetDialogBoxMode(TRUE); // 나중에 edit 박스나 html서버컨테이너 삽입할때나 생각해보자  D3DPRESENTFLAG_LOCKABLE_BACKBUFFER는 필수
	Log( "비디오 초기화 [ 성공 ]\n");
    return S_OK;    
}

void CD3D::InitFont( const TCHAR* pFontFace, int nHeight, bool fBold, bool fItalic, bool fUnderlined )
{
	int nWeight = FW_NORMAL;
	DWORD dwItalic = 0;
	DWORD dwUnderlined = 0;

	if( fBold )
	{
		nWeight = FW_BOLD;
	}

	if(fItalic)
	{
		dwItalic = 1;
	}

	if(fUnderlined)
	{
		dwUnderlined = 1;
	}

  //  m_hFont     = CreateFont( 	-nHeight, 
		//0, 
		//0, 
		//0, 
		//FW_LIGHT,
		//FALSE,	FALSE,	FALSE,
		//DEFAULT_CHARSET, 
		//OUT_DEFAULT_PRECIS,
  //      CLIP_DEFAULT_PRECIS, 
		//DEFAULT_QUALITY,
  //      DEFAULT_PITCH, pFontFace );
//	m_hFont = ChooseFONTSYLE();
//	D3DXCreateFont( m_D3DDevice, m_hFont, &m_DxFont );

	D3DXCreateFont( m_D3DDevice, 
		-nHeight,//11, //UINT         Height,
		0, //UINT                    Width,
		FW_ULTRALIGHT,//FW_LIGHT,//UINT                    Weight,
		1,//UINT                    MipLevels,
		FALSE, //BOOL                    Italic,
		DEFAULT_CHARSET, //DWORD                   CharSet,
		OUT_DEFAULT_PRECIS,//DWORD                   OutputPrecision,
		ANTIALIASED_QUALITY,//DEFAULT_QUALITY,// DWORD                   Quality,
		DEFAULT_PITCH,//DWORD                   PitchAndFamily,
		pFontFace,//LPCWSTR                 pFaceName,
		&m_DxFont ); //LPD3DXFONT*             ppFont

	HDC hdc = m_DxFont->GetDC();
	m_hFont = (HFONT)::GetCurrentObject(hdc,OBJ_FONT);
}

///////////////////////////////////////////////////////////////////////////
// 두개의 텍스쳐를 복사함.. 잘 될라나..
///////////////////////////////////////////////////////////////////////////
HRESULT CD3D::Texture2Texture( LPDIRECT3DTEXTURE9 Dest, LPDIRECT3DTEXTURE9 Src)
{
	HRESULT hr = E_FAIL;

	hr = m_D3DDevice->UpdateTexture( Src,Dest );
	return hr;
	// 원본과 대상은 크기와 포멧 모두 같아야함 
}

HRESULT CD3D::Texture2Texture( int DestIndex, POINT&DestPt, int SrcIndex )
{
	LPDIRECT3DTEXTURE9 Dest = GetTexture(DestIndex);
	LPDIRECT3DTEXTURE9 Src = GetTexture(SrcIndex);

	LPDIRECT3DSURFACE9 pDestSurf = NULL;
	Dest->GetSurfaceLevel(0,&pDestSurf);

	LPDIRECT3DSURFACE9 pSrcSurf = NULL;
	Src->GetSurfaceLevel(0,&pSrcSurf);

	RECT destrt={0,0, m_ActualImgSize[SrcIndex].cx, m_ActualImgSize[SrcIndex].cy };
	RECT srcrt={0,0, m_ActualImgSize[SrcIndex].cx, m_ActualImgSize[SrcIndex].cy };
	OffsetRect(&destrt,DestPt.x, DestPt.y );
	HRESULT hr = D3DXLoadSurfaceFromSurface( pDestSurf,NULL,&destrt, pSrcSurf,NULL,&srcrt,D3DX_FILTER_NONE,0 );//

	SAFE_RELEASE( pDestSurf );
	SAFE_RELEASE( pSrcSurf );
	return hr;
}


//32 비트 색을 16트로 바꿔주는 매크로 만듭시다.
#define _32_to_4444( _32, _4444 )\
	{\
		BYTE A=(BYTE)(( _32 & 0xff000000)>>24)&0xf0;\
		BYTE R=(BYTE)((_32 & 0x00ff0000)>>16)&0xf0;\
		BYTE G=(BYTE)((_32 & 0x0000ff00)>>8)&0xf0;\
		BYTE B=(BYTE)((_32 & 0x000000ff))&0xf0;\
		_4444 =WORD( (A<<8) | (R<<4) | (G)| (B>>4) );\
	}\

#define _32_to_1555( _32, _1555 )\
	{\
		BYTE A=(BYTE)( _32 & 0xff000000)>>24;\
		BYTE R=(BYTE)((_32 & 0x00ff0000)>>16) >> 4;\
		BYTE G=(BYTE)((_32 & 0x0000ff00)>>8) >> 4;\
		BYTE B=(BYTE)((_32 & 0x000000ff)) >> 4;\
		_1555 =WORD((A<<15) | (R<<10) | (G<<5)| (B));\
	}\

#define _32_to_565( _32, _565 )\
	{\
		BYTE R=(BYTE)((_32 & 0x00ff0000)>>16) >> 4;\
		BYTE G=(BYTE)((_32 & 0x0000ff00)>>8) >> 4;\
		BYTE B=(BYTE)((_32 & 0x000000ff)) >> 4;\
		_565 =WORD( (R<<11) | (G<<6)| (B) );\
	}\
\//	WORD Alpha = (BYTE(((0xff000000&currclr)>>24)) & 0xf0)<<8;

//#define ALPHASETTING(currclr)\
//	{\
//	hr = D3DXLoadSurfaceFromSurface(TexSurf,NULL,NULL,Work,NULL,NULL,D3DX_FILTER_NONE,CLRKEY );\
//	assert( hr == D3D_OK);\
//	D3DLOCKED_RECT lrt;\
//	TexSurf->LockRect( &lrt,NULL,0);\
//	int x=0,y=0;\
//	WORD* pTmp =(WORD*) lrt.pBits;\
//	WORD puretmp =0;\
//	WORD wClr =0, wRGB=0;\
//	_32_to_4444(currclr,wClr);\
//	WORD wA = wClr&0xf000;\
//	wRGB   = wClr&0x0fff;\
//	int Pixel=0;\
//	for( y = 0; y<rt.bottom+2; y++)\
//		for( x=0; x<rt.right+2; x++)\
//		{\
//			Pixel= y*width+x ;\
//			WORD  Clr = pTmp[ Pixel ];\
//			if( Clr != 0x0 )\
//			{\
//				if( (Clr & 0xf000)==0xf000 )\
//				{\
//					if( puretmp  == wRGB )\
//					{\
//						puretmp = 0x0fff&Clr;\
//						{\
//							pTmp[Pixel] = Clr&( wA |0x0fff) ;\
//						}\
//					}\
//				}\
//			}\
//		}\
//	TexSurf->UnlockRect();\
//	}\

void ALPHASETTING( LPDIRECT3DSURFACE9 TexSurf,LPDIRECT3DSURFACE9 Work,int width,D3DCOLOR currclr,RECT&rt )
{
	#define CLRKEY 0xffff00ff
	
	HRESULT hr = D3DXLoadSurfaceFromSurface(TexSurf,NULL,NULL,Work,NULL,NULL,D3DX_FILTER_NONE,CLRKEY );\
	assert( hr == D3D_OK);\
	if( GETA(currclr)==0xff) return;
	D3DLOCKED_RECT lrt;\
	TexSurf->LockRect( &lrt,NULL,0);\
	int x=0,y=0;\
	WORD* pTmp =(WORD*) lrt.pBits;\
	WORD puretmp =0;\
	WORD wClr =0, wRGB=0;\
	_32_to_4444(currclr,wClr);\
	WORD wA = wClr&0xf000;\
	wRGB   = wClr&0x0fff;\
	int Pixel=0;\
	for( y = 0; y<rt.bottom+2; y++)\
		for( x=0; x<rt.right+2; x++)\
		{\
			Pixel= y*width+x ;\
			WORD  Clr = pTmp[ Pixel ];\
			if( Clr != 0x0 )\
			{\
				if( (Clr & 0xf000)==0xf000 )\
				{\
					if( puretmp  == wRGB )\
					{\
						//puretmp = 0x0fff&Clr;
						{\
							pTmp[Pixel] = Clr&( wA |0x0fff) ;\
						}\
					}\
				}\
			}\
		}\
	TexSurf->UnlockRect();\
}\

#define FIND_EMPTYSLOT(Index){\
	for( int i=0; i< MAX_TEXTURE; i++ )\
	{\
		if( m_Texture[i].lpTexture == NULL && m_Texture[i].RefCnt == 0 )\
		{\
			Index = i;	break;\
		}\
	}\
}\


///////////////////////////////////////////////////////////////////////////
// 스트링을 텍스쳐로 만들어서 그 인덱스를 돌려줌  외곽선색깔과 글씨의 색깔  
///////////////////////////////////////////////////////////////////////////
int CD3D::Str2Texture(TCHAR* szStr,D3DCOLOR OutLineClr, D3DCOLOR FrontClr,D3DCOLOR BackGroundClr ) //SIZE sz, 
{
	#define CLRKEY 0xffff00ff 
	int Index = -1;
	FIND_EMPTYSLOT(Index);

	/*
	시스템 DC 서피스 1개  
	시스템 알파 서피스 2개 락걸어서 복사
	최종 텍스쳐 1개 락걸어서 복사
	
	*/
	HRESULT hr=~D3D_OK;
	LPDIRECT3DSURFACE9 sfWork=NULL, sfShadow=NULL, sfFont=NULL, sfResult=NULL;
	RECT rt={0,};
	int len = (int)strlen(szStr );
	hr = m_DxFont->DrawText( NULL,szStr, len, &rt, DT_CALCRECT, 0xffffffff );//DT_LEFT|

	int width  = XPOW2(rt.right);
	int height = XPOW2(rt.bottom);



	hr = m_D3DDevice->CreateOffscreenPlainSurface( width, height, D3DFMT_X8R8G8B8,  D3DPOOL_SYSTEMMEM, 	&sfWork,NULL);
	hr = m_D3DDevice->CreateOffscreenPlainSurface( width, height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, 	&sfShadow,NULL);
	hr = m_D3DDevice->CreateOffscreenPlainSurface( width, height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, 	&sfFont,NULL);
	hr = m_D3DDevice->CreateTexture( width, height, 1,  0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED,&m_Texture[ Index ].lpTexture ,NULL ); //D3DPOOL_DEFAULT	
	hr = m_Texture[Index].lpTexture->GetSurfaceLevel(0,&sfResult); 

	HDC hdc = NULL;

	if( GETA(OutLineClr) != 0x00 ) // 그림자를 안그린다면
	{
		sfWork->GetDC(&hdc);
		::SetBkMode(hdc,TRANSPARENT); //ARGB 순이므로.. 그러나 폰트 컬러는 반대일걸
		COLORREF cl = RGB(GETR(CLRKEY),GETG(CLRKEY),GETB(CLRKEY)); //0xff000000|
		HBRUSH hbrush = CreateSolidBrush( cl );// RGB(255,0,255)
		RECT fillrt={0,0,width,height};
		::FillRect( hdc, &fillrt,hbrush);
		::DeleteObject( hbrush); // 역상도 넣어야 하는디 
		SelectObject( hdc, m_hFont );
		BYTE R=GETR(OutLineClr),G=GETG(OutLineClr),B=GETB(OutLineClr);
		::SetTextColor(hdc,RGB( R,G,B  ) ); 
		///int rr = ::SetROP2(hdc,R2_NOP);
//		TextOut( hdc, 1,0,szStr,len);//상
		TextOut( hdc, 1,2,szStr,len);//하
//		TextOut( hdc, 0,1,szStr,len);//좌
		TextOut( hdc, 2,1,szStr,len);//우

		//TextOut( hdc, 0,0,szStr,len);//상좌
		//TextOut( hdc, 0,2,szStr,len);//하좌
		//TextOut( hdc, 2,0,szStr,len);//상우
		TextOut( hdc, 2,2,szStr,len);//하우

		sfWork->ReleaseDC(hdc); 
	}
	// 알파텍스쳐에 복사하기
	// 알파텍스쳐는 결과텍스쳐에 복사, 알파값이 0 아닌것만 복사함
	DWORD alpha = 0;
	alpha= OutLineClr & 0xff000000;
	// work to shadow
	if( alpha != 0)
	{
		//sfWork->ReleaseDC(hdc); // 망할놈에 릴리즈 디시 
		{
			D3DLOCKED_RECT DESTlrt,SRClrt;
			sfWork->LockRect( &SRClrt,NULL,0);
			sfShadow->LockRect( &DESTlrt,NULL,0);
			DWORD* pDESTBits =(DWORD*) DESTlrt.pBits;
			DWORD* pSRCBits =(DWORD*) SRClrt.pBits;
			DWORD SRCClr =0;
			int Pixel=0;
			for( int y = 0; y<rt.bottom+2; y++)
				for( int x=0; x<rt.right+2; x++)
				{
					Pixel= y*width+x ;
					SRCClr = pSRCBits[ Pixel ];
					if( (SRCClr&0x00ffffff) == (OutLineClr&0x00ffffff) )
					{
						pDESTBits[Pixel] = SRCClr | ( alpha  );
					}
				}
				sfWork->UnlockRect();
				sfShadow->UnlockRect();
		}
		{
			D3DLOCKED_RECT DESTlrt,SRClrt;
			sfShadow->LockRect( &SRClrt,NULL,0);
			sfResult->LockRect( &DESTlrt,NULL,0);
			WORD* pDESTBits =(WORD*) DESTlrt.pBits;
			DWORD* pSRCBits =(DWORD*) SRClrt.pBits;
			DWORD SRCClr =0;
			int Pixel=0;
			for( int y = 0; y<rt.bottom+2; y++)
				for( int x=0; x<rt.right+2; x++)
				{
					Pixel= y*width+x ;
					SRCClr = pSRCBits[ Pixel ];
					if( GETA(SRCClr) != 0 )
					{
						_32_to_4444( SRCClr, pDESTBits[Pixel] );
					}
					else
						pDESTBits[Pixel] = 0x0000;
				}
				sfShadow->UnlockRect();
				sfResult->UnlockRect();
		}
	}


	alpha= FrontClr&0xff000000;
	// work to front
	if( alpha != 0)
	{
		sfWork->GetDC(&hdc);
		::SetBkMode(hdc,TRANSPARENT); //ARGB 순이므로.. 그러나 폰트 컬러는 반대일걸
		COLORREF cl = RGB(GETR(CLRKEY),GETG(CLRKEY),GETB(CLRKEY)); //0xff000000|
		HBRUSH hbrush = CreateSolidBrush( cl );// RGB(255,0,255)
		RECT fillrt={0,0,width,height};
		::FillRect( hdc, &fillrt,hbrush);
		::DeleteObject( hbrush);
		SelectObject( hdc, m_hFont );
		DWORD cc = RGB( GETR(FrontClr),GETG(FrontClr),GETB(FrontClr) );
		::SetTextColor(hdc, cc ); //ARGB 순이므로.. 그러나 폰트 컬러는 반대일걸
		TextOut( hdc, 1,1,szStr,len);//우
		sfWork->ReleaseDC(hdc); // 망할놈에 릴리즈 디시 
		{
			D3DLOCKED_RECT DESTlrt,SRClrt;
			sfWork->LockRect( &SRClrt,NULL,0);
			sfFont->LockRect( &DESTlrt,NULL,0);
			DWORD* pDESTBits =(DWORD*) DESTlrt.pBits;
			DWORD* pSRCBits =(DWORD*) SRClrt.pBits;
			DWORD SRCClr =0;
			int Pixel=0;
			for( int y = 0; y<rt.bottom+2; y++)
				for( int x=0; x<rt.right+2; x++)
				{
					Pixel= y*width+x ;
					SRCClr = pSRCBits[ Pixel ];
					if( (SRCClr&0x00ffffff) == (FrontClr&0x00ffffff) )
					{
						pDESTBits[Pixel] = SRCClr | ( alpha  );
					}
				}
				sfWork->UnlockRect();
				sfFont->UnlockRect();
		}
		{
			D3DLOCKED_RECT DESTlrt,SRClrt;
			sfFont->LockRect( &SRClrt,NULL,0);
			sfResult->LockRect( &DESTlrt,NULL,0);
			WORD* pDESTBits =(WORD*) DESTlrt.pBits;
			DWORD* pSRCBits =(DWORD*) SRClrt.pBits;
			DWORD SRCClr =0,DESTClr=0;
			int Pixel=0;
			for( int y = 0; y<rt.bottom+2; y++)
				for( int x=0; x<rt.right+2; x++)
				{
					Pixel= y*width+x ;
					SRCClr = pSRCBits[ Pixel ];
					//DESTClr = pDESTBits[Pixel];
					if( GETA(SRCClr) != 0 ) // 알파값이 맞는 넘만 수정함 
					{
						_32_to_4444( SRCClr,pDESTBits[Pixel]);
					}
				}
				sfResult->UnlockRect();
				sfFont->UnlockRect();
		}
	}

	m_ActualImgSize[ Index ].cx = rt.right+2;
	m_ActualImgSize[ Index ].cy = rt.bottom+2;
	m_Texture[Index].RefCnt++;


	SAFE_RELEASE(sfWork);
	SAFE_RELEASE(sfShadow);
	SAFE_RELEASE(sfFont);
	SAFE_RELEASE(sfResult);
	assert( hr == D3D_OK);
	return Index;
}

int CD3D::Str2Texture_backup(TCHAR* szStr,D3DCOLOR OutLineClr, D3DCOLOR FrontClr,D3DCOLOR BackGroundClr ) 
{
	int Index = -1;
	for( int i=0; i< MAX_TEXTURE; i++ )
	{
		if( m_Texture[i].lpTexture == NULL && m_Texture[i].RefCnt == 0 )
		{
			Index = i;	break;
		}
	}
	
	// 일단 임시 서피스 "작업" 서피스를 만든다.
	// 작업 서피스에는 그림자가될 글씨를 뿌려놓고 결과 서피스에 복사함 565짜리 시스템 서피스
	// 결과 서피스는 그냥 락걸수 있도록 ARGB 만 가진 4444 짜리 시스템 서피스

	// 1. 그림자 셋팅
	// 알파가 0 이라면 그림자는 무시
	// dc로 그림자를 쓰기 
	// 최종 서피스에 복사
	// 만일 알파가 0xff 라면 이미 셋팅되어 있는 셈이므로 스킵
	// 락을 걸고 4444 서피스에 그림자의 알파값을 셋팅, 
	// 2. 글씨 셋팅
	// 만일 알파가 0 이라면 글씨쓰기 스킵
	// dc로 글자 쓰기 
	// 최종 서피스에 복사
	// 만일 알파가 0xff 라면 이미 셋팅되어 있는 셈이므로 스킵
	// 락을 걸고 4444 서피스에 글자의 알파값을 셋팅, 


	HRESULT hr=~D3D_OK;
	LPDIRECT3DSURFACE9 Work=NULL,TexSurf=NULL;//Result=NULL,
	RECT rt={0,};
	int len = (int)strlen(szStr );
	hr = m_DxFont->DrawText( NULL,szStr, len, &rt, DT_CALCRECT, 0xffffffff );//DT_LEFT|

	int width  = XPOW2(rt.right);
	int height = XPOW2(rt.bottom);

	#define CLRKEY 0xffff00ff 

	g_D3D.GetDevice()->CreateOffscreenPlainSurface( width, height, D3DFMT_R5G6B5, D3DPOOL_SYSTEMMEM, 	&Work,NULL);
	hr = m_D3DDevice->CreateTexture( width, height, 1,  0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED,&m_Texture[ Index ].lpTexture ,NULL ); //D3DPOOL_DEFAULT	
	// 최종적으로 텍스쳐 서피스에 Result 를 복사해줍니다.
	hr = m_Texture[Index].lpTexture->GetSurfaceLevel(0,&TexSurf); 
	assert( hr == D3D_OK);

	HDC hdc = NULL;
	Work->GetDC(&hdc);
	::SetBkMode(hdc,TRANSPARENT); //ARGB 순이므로.. 그러나 폰트 컬러는 반대일걸
	COLORREF cl = RGB(GETR(CLRKEY),GETG(CLRKEY),GETB(CLRKEY)); //0xff000000|
	HBRUSH hbrush = CreateSolidBrush( cl );// RGB(255,0,255)
	RECT fillrt={0,0,width,height};
	::FillRect( hdc, &fillrt,hbrush);
	::DeleteObject( hbrush);
	SelectObject( hdc, m_hFont );


		BYTE R=GETR(OutLineClr),G=GETG(OutLineClr),B=GETB(OutLineClr);
		if( GETRGB( OutLineClr ) == GETRGB(FrontClr) )
		{
			int _B = B+16;
			if(_B > 255 ) _B=255;
			B= _B;
		}
		OutLineClr = D3DCOLOR_ARGB( GETA(OutLineClr), R,G,B);
	if( GETA(OutLineClr) != 0x00 ) // 그림자를 안그린다면
	{
		::SetTextColor(hdc,RGB( R,G,B  ) ); 
		///int rr = ::SetROP2(hdc,R2_NOP);
		TextOut( hdc, 1,0,szStr,len);//상
		TextOut( hdc, 1,2,szStr,len);//하
		TextOut( hdc, 0,1,szStr,len);//좌
		TextOut( hdc, 2,1,szStr,len);//우

		TextOut( hdc, 0,0,szStr,len);//상좌
		TextOut( hdc, 0,2,szStr,len);//하좌
		TextOut( hdc, 2,0,szStr,len);//상우
		TextOut( hdc, 2,2,szStr,len);//하우
	}

	if( GETA(FrontClr) != 0x00 ) //안그린다면
	{
		DWORD cc = RGB( GETR(FrontClr),GETG(FrontClr),GETB(FrontClr) );
		::SetTextColor(hdc, cc ); //ARGB 순이므로.. 그러나 폰트 컬러는 반대일걸
		TextOut( hdc, 1,1,szStr,len);//우
	}

	Work->ReleaseDC(hdc); // 망할놈에 릴리즈 디시 

	hr = D3DXLoadSurfaceFromSurface(TexSurf,NULL,NULL,Work,NULL,NULL,D3DX_FILTER_NONE,CLRKEY );
	assert( hr == D3D_OK);

	D3DLOCKED_RECT lrt;
	TexSurf->LockRect( &lrt,NULL,0);
	int x=0,y=0;
	WORD* pTmp =(WORD*) lrt.pBits;
	WORD puretmp =0;
	WORD wOutClr =0, wOutRGB=0;
	WORD wFrontClr =0, wFrontRGB=0;
	_32_to_4444(OutLineClr,wOutClr);
	_32_to_4444(FrontClr,  wFrontClr);
	WORD wOA = wOutClr&0xf000;
	WORD wFA = wFrontClr&0xf000;
	wOutRGB= wOutClr&0x0fff;
	wFrontRGB = wFrontClr &0x0fff;
	int Pixel=0;
	for( y = 0; y<rt.bottom+2; y++)
		for( x=0; x<rt.right+2; x++)
		{
			Pixel= y*width+x ;
			WORD  Clr = pTmp[ Pixel ];
			if( (Clr & 0xf000)>=0x0100 )
			{
				puretmp = Clr&0x0fff;
				if( puretmp  == wOutRGB )
				{
					Clr &= ( wOA |0x0fff) ;
				}else if( puretmp  == wFrontRGB )
					Clr &= ( wFA |0x0fff) ;
				pTmp[Pixel] = Clr;
			}
		}
		TexSurf->UnlockRect();


	// 완전 빈픽셀이 아니라면
	/*알파값 제거뒤 순수한 컬러값만 비교*/
	// 알파값을 넣어줍니다.


	m_ActualImgSize[ Index ].cx = rt.right+2;
	m_ActualImgSize[ Index ].cy = rt.bottom+2;
	m_Texture[Index].RefCnt++;


	SAFE_RELEASE(Work);
	SAFE_RELEASE(TexSurf);
	assert( hr == D3D_OK);
	return Index;
}


void CD3D::BltText( BYTE Layer,int x, int y, D3DCOLOR rgbFontColour, const TCHAR* pText )
{
	if( _tcslen( pText ) == 0 ) return; //_tcsclen
	
	m_BltText[ CBltText::BltTextCnt ].BltText( x,y,rgbFontColour,pText);

	m_DrawObj[ Layer ][ m_DrawObjCnt[ Layer ] ] = &m_BltText[ CBltText::BltTextCnt ];
	CBltText::BltTextCnt++;
	m_DrawObjCnt[ Layer ]++;
}

void CD3D::BltText( BYTE Layer,RECT& rt, D3DCOLOR rgbFontColour, const TCHAR* pText )
{
	if( _tcslen( pText ) == 0 )
		return;
	
/*	TCHAR buff[ 256 ]={ _T("") };
	va_list args;
    
	va_start(args, pText );
	_vstprintf(buff, pText, args );
	va_end(args);*/

	m_BltText[ CBltText::BltTextCnt ].BltText( rt, rgbFontColour,pText );
	m_DrawObj[ Layer ][ m_DrawObjCnt[ Layer ] ] = &m_BltText[ CBltText::BltTextCnt ];
	CBltText::BltTextCnt++;
	m_DrawObjCnt[ Layer ]++;
}


void CD3D::ReleaseANI( int TexID )
{
	assert( TexID >= 0 );
	if( TexID < 0 ) return ;
	
	ReleaseTexture(TexID);
	if( m_Texture[ TexID ].RefCnt <= 0 ) // 참조 카운트가 0 이하라면 맵에서도 지워 버립니다. 
	{
		if(TextureMap.empty() == false )
		{
			TEXTUREMAPITOR it = TextureMap.begin() ;
			TEXTUREMAPITOR endit =TextureMap.end();
			while( it!= endit )
			{
				RES_ANI& Ani =(RES_ANI)(it->second);
				//if( pAni != NULL )
				{
					if( Ani.TexID == TexID )
					{
						//SAFE_RELEASE( m_Texture[ Ani.TexID ].lpTexture );
						//SAFE_FREE( pAni );
						TextureMap.erase( it );
						break;
					}
				}
				it++;
			}
		}
	}
}

////-----------------------------------------------------------------------------
//// Builds a transformation matrix from the arguments.
////
//// pOut     - The result of this operation.
//// pCenter  - The center of scaling and rotation.
//// pScaling - The scaling vector.
//// angle    - The angle of rotation in radians.
////-----------------------------------------------------------------------------
//
//D3DMATRIX* CD3D::BuildMatrix( D3DMATRIX* pOut, D3DXVECTOR2* pCenter, D3DXVECTOR2* scaling, float angle )
//{
//	D3DXMATRIX matrices[5];
//
//	D3DXMatrixTranslation(&matrices[0], -pCenter->x, -pCenter->y, 0 );
//	D3DXMatrixScaling( &matrices[1], scaling->x, scaling->y, 1 );
//
//	D3DXMatrixIdentity( &matrices[2] );
//	D3DXMatrixIdentity( &matrices[3] );
//
//	D3DXMatrixRotationZ( &matrices[2], angle );
//	D3DXMatrixTranslation( &matrices[3], pCenter->x, pCenter->y, 0 );
//
//	matrices[4] = matrices[0] * matrices[1] * matrices[2] * matrices[3];
//	*pOut = matrices[4];
//
//	return pOut;
//}
//
//
//void CD3D::TransformVertices(D3DTLVERTEX vertices[], D3DXVECTOR2* pCenter, D3DXVECTOR2* pScaling,float angle)
//{
//	D3DXMATRIX matTransf, matVerts, matNew;
//	BuildMatrix(&matTransf, pCenter, pScaling, angle);
//
//	DWORD cr;
//	for (cr = 0; cr < 4; cr++)
//	{
//		// 4 vertices fit nicely into a 4x4 matrix --
//		// Put each vertex point into a matrix row.
//		matVerts(cr, 0) = vertices[cr].x;
//		matVerts(cr, 1) = vertices[cr].y;
//		matVerts(cr, 2) = vertices[cr].z;
//		matVerts(cr, 3) = 1.0f;  // the scaling factor, w
//	}
//
//	// Reuse D3D matrix multiplication code to transform our 
//	// vertices.
//	matNew = matVerts * matTransf;
//
//	for (cr = 0; cr < 4; cr++)
//	{
//		// Retrieve the newly transformed points.
//		vertices[cr].x = matNew(cr, 0);
//		vertices[cr].y = matNew(cr, 1);
//		vertices[cr].z = matNew(cr, 2);
//	}
//}
//
//void CD3D::Draw(	int Index, RECT* pSrc, D3DXVECTOR2* pScaling,
//                    D3DXVECTOR2* pCenter,  float Rotate,D3DXVECTOR2* Trans,  D3DCOLOR colour )
//{
//	D3DSURFACE_DESC surfDesc;
//	m_Texture[Index].lpTexture->GetLevelDesc(0, &surfDesc);
//	
//	float left   = (float)Trans->x;
//	float top    = (float)Trans->y;
//	float right  = left + m_ActualImgSize[Index].cx;//surfDesc.Width; // - 1;
//	float bottom = top + m_ActualImgSize[Index].cy;//surfDesc.Height; // - 1;
//
//	const float z = 0.0f, rhw = 1.0f;
//	float texelX =	(float)pSrc->left / (float)surfDesc.Width;
//	float texelY =  (float)pSrc->top / (float)surfDesc.Height;
//	float texelWidth  = (float)pSrc->right /  (float)surfDesc.Width;
//	float texelHeight = (float)pSrc->bottom /(float)surfDesc.Height;
//
//	D3DTLVERTEX vertices[4] = // 순서 상관 있음 x, y, z, rhw, colour, tu, tv
//	{
//		{ left,  top,    z, rhw, colour, texelX, texelY },			// 좌상
//		{ right, top,    z, rhw, colour, texelWidth, texelY },		// 우상
//		{ right, bottom, z, rhw, colour, texelWidth, texelHeight }, // 우하
//		{ left,  bottom, z, rhw, colour, texelX, texelHeight }		// 좌하
//	};
//
//	D3DXVECTOR2 center, scaling;
//	if (pScaling == NULL) 
//	{
//		scaling.x = scaling.y = 1;
//		pScaling = &scaling;
//	}
//	center.x = (float)Trans->x + pCenter->x;
//	center.y = (float)Trans->y + pCenter->y;
//	pCenter = &center;
//	
//	TransformVertices( vertices, pCenter, pScaling, Rotate );
//	
//	m_D3DDevice	->SetTexture(0, m_Texture[Index].lpTexture );
//	m_D3DDevice	->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(D3DTLVERTEX) );
//	m_D3DDevice	->SetTexture(0, NULL );
//}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




int CD3D::RegistWindow( TCHAR* lpSz, HICON hIcon,  WNDPROC WndProc )
{
    WNDCLASSEX wcex = 
	{
        sizeof(WNDCLASSEX),         // wcex.cbSize
        CS_OWNDC|CS_DBLCLKS ,//|CS_CLASSDC,      // wcex.style
        (WNDPROC)WndProc,           // wcex.lpfnWndProc
        0L,                         // wcex.cbClsExtra
        0L,                         // wcex.cbWndExtra
        GetModuleHandle(NULL),      // wcex.hInstance
        hIcon,                      // wcex.hIcon
        LoadCursor(NULL,IDC_ARROW), // wcex.hCursor
        (HBRUSH)GetStockObject(BLACK_BRUSH),                       // wcex.hbrBackground
        NULL,                       // wcex.lpszMenuName
        lpSz,						// wcex.lpszClassName
        hIcon                       // wcex.hIconSm
	};

    if(!RegisterClassEx(&wcex))
        return E_FAIL;  

	int winx,winy,winwidth,winheight;

	if( m_bFull )
	{
		winx = 0;
		winy = 0;
		winwidth = GetSystemMetrics(SM_CXSCREEN);
		winheight = GetSystemMetrics(SM_CYSCREEN);
	}
	else 
	{
		winx = GetSystemMetrics(SM_CXSCREEN)/2 - (m_WinWidth/2);
		winy = GetSystemMetrics(SM_CYSCREEN)/2 - (m_WinHeight/2);
		winwidth =m_WinWidth ;
		winheight =m_WinHeight;
	}

    m_hWnd = CreateWindow(
        wcex.lpszClassName, wcex.lpszClassName,                    
        WS_POPUP|WS_CLIPCHILDREN,//WS_OVERLAPPED | WS_SYSMENU |WS_MINIMIZEBOX | WS_VISIBLE,
        winx,
        winy,
        winwidth,
        winheight,
        GetDesktopWindow(),
        NULL,						
        wcex.hInstance,
        NULL						
		);

	assert( m_hWnd != NULL );

	if( m_hWnd != NULL )
	{
		if( m_bFull == true )
        ShowWindow( m_hWnd, SW_SHOWMAXIMIZED); 
		else
        ShowWindow( m_hWnd, SW_SHOWDEFAULT); 

        UpdateWindow( m_hWnd );
	}

    if( !m_hWnd )
        return -1;

    return 1;
}




//-----------------------------------------------------------------------------
// Creates a surface from the specified graphics file.
//
//  ppSurface  - The created surface object.
//  szFileName - The bitmap file name.
//  colourKey  - The transparent colour. Opaque black by default.
//-----------------------------------------------------------------------------
void CD3D::CreateSurfaceFromFile(LPDIRECT3DSURFACE9* ppSurface,SIZE* pSize,
							const TCHAR* szFileName, D3DCOLOR colourKey )
{
	assert( 0&&"do not use this");
	D3DFORMAT SURFACE_FORMAT = D3DFMT_A1R5G5B5;
//	const D3DFORMAT SURFACE_FORMAT = D3DFMT_R5G6B5;
//	const D3DFORMAT SURFACE_FORMAT = D3DFMT_X1R5G5B5;
//	const D3DFORMAT SURFACE_FORMAT = D3DFMT_A8R8G8B8;

	LPDIRECT3DSURFACE9 pSurface;
	D3DXIMAGE_INFO srcInfo;    

	HRESULT hr;
	SURFACE_FORMAT  = D3DFMT_R5G6B5;
	if( FAILED( m_D3DDevice->CreateOffscreenPlainSurface( 1, 1, D3DFMT_R5G6B5, D3DPOOL_MANAGED , &pSurface,NULL) ) )
	{
		SURFACE_FORMAT  = D3DFMT_A8R8G8B8;
		if( FAILED( m_D3DDevice->CreateOffscreenPlainSurface(1, 1, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED , &pSurface,NULL) ) )
		{
			SURFACE_FORMAT  = D3DFMT_X1R5G5B5;
			if( FAILED( m_D3DDevice->CreateOffscreenPlainSurface(1, 1, D3DFMT_X1R5G5B5,D3DPOOL_MANAGED , &pSurface,NULL) ) )
			{
				SURFACE_FORMAT  = D3DFMT_A1R5G5B5;
				if( FAILED( m_D3DDevice->CreateOffscreenPlainSurface(1, 1, D3DFMT_A1R5G5B5,D3DPOOL_MANAGED , &pSurface,NULL) ) )
				{
#ifdef _HTMLLOG
					g_HtmlLog.LogError( "그래픽카드가 너무 구형이라 텍스쳐가 생성안됨" );
#endif
					assert( 0 && "Don't Support Surface" );
				}
			}
		}
	}

	hr = D3DXLoadSurfaceFromFile(pSurface, NULL, NULL, szFileName, 
		NULL, D3DX_FILTER_NONE, 0, &srcInfo);
	assert( hr == D3D_OK && "D3DXLoadSurfaceFromFile");
	pSurface->Release();

	// Create a surface to hold the entire file
	hr = m_D3DDevice->CreateOffscreenPlainSurface(srcInfo.Width, srcInfo.Height, SURFACE_FORMAT, D3DPOOL_MANAGED ,ppSurface,NULL);
	pSurface = *ppSurface;
	assert( hr == D3D_OK && "CreateImageSurface");
	// The default colour key is 0xFF000000 (opaque black). Magenta 
	// (0xFFFF00FF) is another common colour used for transparency.
	hr = D3DXLoadSurfaceFromFile( pSurface, NULL, NULL, szFileName, NULL, 
								D3DX_FILTER_NONE, colourKey, &srcInfo );
	assert( hr == D3D_OK && "D3DXLoadSurfaceFromFile");
	pSize->cx = srcInfo.Width;
	pSize->cy = srcInfo.Height;
}

//-----------------------------------------------------------------------------
// Creates a texture from a region of the surface.
//
// pSurface  - the source surface.
// pSrcRect  - portion of the source surface to create the texture from.
// ppTexture - the created texture object.
//-----------------------------------------------------------------------------
void CD3D::CreateTextureFromSurface(LPDIRECT3DSURFACE9 pSurface, RECT* pSrcRect,
                              LPDIRECT3DTEXTURE9* ppTexture)
{
	assert( 0&&"don't use this" );
	assert( pSurface != NULL );
	int width =0;
	int height =0;
	D3DSURFACE_DESC surfDesc;
#ifdef _HTMLLOG
	D3DSURFACE_DESC surfDesc1;
#endif

	if( pSrcRect == NULL ) // 서피스 전부를 텍스쳐로 만들어 버림 
	{

		pSurface->GetDesc(&surfDesc) ;
		width  = surfDesc.Width  ;
		height = surfDesc.Height ; 	
	}
	else
	{
		width  = pSrcRect->right - pSrcRect->left; // + 1;
		height = pSrcRect->bottom - pSrcRect->top; // + 1;
	}
	HRESULT hr;
	
//	hr = m_D3DDevice -> CreateTexture( width, height, 0, 0, surfDesc.Format, D3DPOOL_MANAGED, ppTexture);
	hr = D3DXCreateTexture(m_D3DDevice, width, height, 1, 0, surfDesc.Format, D3DPOOL_MANAGED, ppTexture);
	TCHAR * ch = (TCHAR * )g_D3D.GetD3DErr(hr);
	assert( hr == D3D_OK );
	assert( ppTexture !=NULL && "Don't Create Texture" ); 

	// Retrieve the surface image of the texture.
	LPDIRECT3DSURFACE9 pTexSurface;
	LPDIRECT3DTEXTURE9 pTexture = *ppTexture;
	pTexture->GetLevelDesc(0, &surfDesc);
	hr = pTexture->GetSurfaceLevel(0, &pTexSurface);
	assert( hr == D3D_OK && "GetSurfaceLevel" );

	RECT rt ={0,};
	if( pSrcRect != NULL )
	rt = *pSrcRect;
	else 
	{
		rt.left = 0;
		rt.right =  width;
		rt.top	 = 0;
		rt.bottom = height ;
	}

	// Copy the image to the texture.
//	hr = m_D3DDevice->CopyRects(pSurface, &rt, 1, 	pTexSurface, NULL );
#ifdef	_HTMLLOG
	if( hr != D3D_OK)
	{
		ch = (TCHAR * )g_D3D.GetD3DErr(hr);	
		pSurface->GetDesc(&surfDesc);
		g_HtmlLog.LogError( "원본 %d, %d,%d ",surfDesc.Format, surfDesc.Width, surfDesc.Height);
		
		pTexSurface->GetDesc(&surfDesc1);
		g_HtmlLog.LogError( "대상 %d, %d,%d ",surfDesc1.Format, surfDesc1.Width, surfDesc1.Height);
	}
#endif
	assert( hr == D3D_OK );

	D3DSURFACE_DESC dd,ad;
	hr = pSurface->GetDesc( &dd );
	assert( hr == D3D_OK );
	hr = pTexSurface->GetDesc( &ad );
	assert( hr == D3D_OK  );
	assert( dd.Format == ad.Format );
	assert( pSurface != NULL && "pSurface is NULL" );
	assert( pTexSurface != NULL && "pTexSurface is NULL" );
	if( FAILED( hr  ) )
	{ 
		MessageBox( 0, g_D3D.GetD3DErr( hr ),_T("err"),MB_OK);
		assert( 0 && "Don't Copy Rect"  ); 
	}
	pTexSurface->Release();
}

inline void CD3D::FillRect(RECT *pRect, D3DCOLOR fillColor)
{
	//RECT rt;
	//if( pRect == NULL )
	//	SetRect(&rt,0,0,m_WinWidth,m_WinHeight);
	//else
	//	rt = *pRect;
    // (D3DRECT*)&rt
//	HRESULT hr = m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, fillColor, 1.0f, 0);
//	assert( hr == D3D_OK);
}


int	CD3D::LoadTGA( const TCHAR* fName, RES_ANI& Ani )
{
	assert( m_D3DDevice != NULL );

	int TextureID = -1;
	for( int i = 0 ; i < MAX_TEXTURE; i++ )
	{
		if( m_Texture[ i ].lpTexture == NULL)
		{
			TextureID = i;
			break;
		}
	}

	if( TextureID == -1 ) 
	{
#ifdef _HTMLLOG
		g_HtmlLog.LogError( _T(" %d CD3D::LoadIMG, %s슬롯이부족하여 로딩불가"),__LINE__, fName );
#endif
		assert( TextureID != -1 ); 
		return -1;
	}

	HRESULT hr ;
	D3DXIMAGE_INFO imageInfo; //
	hr = D3DXCreateTextureFromFileEx( m_D3DDevice,fName,0,0,1,0,D3DFMT_UNKNOWN,
						D3DPOOL_MANAGED,D3DX_DEFAULT,D3DPOOL_MANAGED,0,&imageInfo,NULL,&m_Texture[ TextureID ].lpTexture);
	
	SIZE sz;
	sz.cx=imageInfo.Width;
	sz.cy=imageInfo.Height;

//    HRESULT hr = D3DXCreateTextureFromFile(   m_D3DDevice, fName,   &m_Texture[ TextureID ].lpTexture );
	char* ch = this->GetD3DErr( hr );

	
	m_Texture[ TextureID ].RefCnt++;
	
	Ani.sprInfo.nWidth  = sz.cx ;
	Ani.sprInfo.nHeight = sz.cy ;
	Ani.TexID = TextureID;
//	RES_ANI* pTmpAni =(RES_ANI*) malloc( sizeof( RES_ANI ));
//	*pTmpAni = Ani;
	TextureMap[ fName ] =Ani;// pTmpAni;
#ifdef _HTMLLOG
//	g_HtmlLog.LogInfo( _T("CD3D::LoadAni, %s , %d"), fName, TextureID );	
#endif
	return TextureID;
}



int CD3D::LoadIMG(const TCHAR* fName, RES_ANI& Ani )
{
	assert( m_D3DDevice != NULL );

	int TextureID = -1;
	for( int i = 0 ; i < MAX_TEXTURE; i++ )
	{
		if( m_Texture[ i ].lpTexture == NULL)
		{
			TextureID = i;
			break;
		}
	}

	if( TextureID == -1 ) 
	{
#ifdef _HTMLLOG
		g_HtmlLog.LogError( _T(" %d CD3D::LoadIMG, %s슬롯이부족하여 로딩불가"),__LINE__, fName );
#endif
		assert( TextureID != -1 ); 
		return -1;
	}

	//SIZE sz;
	//LPDIRECT3DSURFACE9	surface;
	//CreateSurfaceFromFile( &surface, &sz, fName, D3DCOLOR_ARGB(0xff,0xff,0x00,0xff) );
	//CreateTextureFromSurface( surface,NULL, &m_Texture[ TextureID ].lpTexture );
	D3DXIMAGE_INFO imageInfo; //
	HRESULT hr = D3DXCreateTextureFromFileEx( this->m_D3DDevice,fName,
		D3DX_DEFAULT,//UINT                      Width,
		D3DX_DEFAULT,//UINT                      Height,
		1,//UINT                      MipLevels,
		0,//DWORD                     Usage,
		D3DFMT_UNKNOWN,//D3DFORMAT                 Format,
		D3DPOOL_MANAGED, //D3DPOOL                   Pool,
		D3DX_FILTER_NONE,//DWORD                     Filter, D3DX_FILTER_POINT
		D3DX_FILTER_NONE,//DWORD                     MipFilter,
		0,//D3DCOLOR                  ColorKey,
		&imageInfo,//D3DXIMAGE_INFO*           pSrcInfo,
		NULL,//PALETTEENTRY*             pPalette,
		&m_Texture[ TextureID ].lpTexture);	//LPDIRECT3DTEXTURE9*       ppTexture);
	if( hr !=D3D_OK)
	{
		MessageBox(0,fName,"D3D Error",MB_OK);
	}

//	assert( hr == D3D_OK && "이미지로딩중에러남");		
	m_Texture[ TextureID ].RefCnt++;
	
	Ani.sprInfo.nWidth = imageInfo.Width;
	Ani.sprInfo.nHeight = imageInfo.Height;
	Ani.TexID = TextureID;
	//RES_ANI* pTmpAni =(RES_ANI*) malloc( sizeof( RES_ANI ));
	//*pTmpAni = *pAni;
	TextureMap[ fName ] = Ani;
#ifdef _HTMLLOG
//	g_HtmlLog.LogInfo( _T("CD3D::LoadAni, %s , %d"), fName, TextureID );	
#endif
	return TextureID;
}


int CD3D::LoadAni( const TCHAR* fName, RES_ANI& Ani )
{

	TCHAR pTmpName[MAX_PATH];
	_tcscpy( pTmpName, fName );	
	_tcsupr( pTmpName);

	RES_ANI Tmp ;
	Tmp = (TextureMap[ pTmpName ]);
	if( Tmp.TexID != -1 )
	{
		m_Texture[ Tmp.TexID ].RefCnt++;
		Ani = Tmp;

#ifdef _DEBUGCONSOLE
//	ConsoleOut( "%s is Already \n", fName );
#endif

		return Tmp.TexID;
	}

#ifdef _DEBUGCONSOLE
//	ConsoleOut( "%s load begin\n", fName );
#endif


	TCHAR ext[4]={ 0, };
	_tcscpy( ext, &pTmpName[ _tcslen(pTmpName)- 3 ] );
	
	// 일반 포멧인지, GUI_ANI 포멧인지 구분한다.
	// 일반 포멧이면 이미지와 에니메이션 파일 까지 읽어 들인뒤 셋팅해준다.
	// GUI_ANI 포멧이면 에니메이션 정보가 들어있으므로 셋팅해준다.
	if( _tcscmp( ext,_T("ANI") ) == 0 )
	goto ANIFORMAT;
	else //if( _tcscmp( ext,_T("TGA") ) == 0 )
//	goto TGAFORMAT;
//	else 
	goto NORMALFORMAT;
	
NORMALFORMAT:	
	{
		ZeroMemory( &Ani,sizeof( Ani ) );
		DWORD start = timeGetTime();

		int ret = LoadIMG( pTmpName, Ani );
		m_ActualImgSize[Ani.TexID].cx = Ani.sprInfo.nWidth;
		m_ActualImgSize[Ani.TexID].cy = Ani.sprInfo.nHeight;
		return ret;
	}


ANIFORMAT:
	{
		// 텍스쳐 슬롯의 빈공간 찾기
		int TextureID = -1;
		for( int i = 0 ; i < MAX_TEXTURE; i++ )
		{
			if( m_Texture[ i ].lpTexture == NULL )
			{
				TextureID = i;
				break;
			}
		}
		

		if( TextureID == -1 )
		{
#ifdef _HTMLLOG
//			g_HtmlLog.LogError( _T("CD3D::LoadAni, "),fName );
#endif
			Ani.TexID = -1;
			return -1;
		}
		

		FILE *fp;
		fp = _tfopen( pTmpName, _T("rb") );
		DWORD ret = GetLastError();
		if(fp == NULL)
		{
#ifdef _DEBUGCONSOLE
//		g_HtmlLog.LogError( _T("CD3D::LoadAni, %s 파일이 없습니다"), pTmpName );
		ConsoleOut( "CD3D::LoadAni, %s 파일이 없습니다\n", pTmpName );
#endif
			Ani.TexID = -1;
			return -1;
		}
		
		fread(&(Ani.sprInfo), 1, sizeof(SPRITEINFOHEADER32), fp);
		
		int Size = Ani.sprInfo.nFrame * Ani.sprInfo.nHeight * Ani.sprInfo.nWidth*3;//24비트 짜리원본을 읽어들임
		BYTE* pBuff = (BYTE*)malloc(Size);
		fread(pBuff, Size,1, fp); // 오우 베이베 
		
//#define _4444_
#define _1555_

#ifdef _4444_
		D3DFORMAT fmt = D3DFMT_A4R4G4B4;
#else if _1555_
		D3DFORMAT fmt = D3DFMT_A1R5G5B5;
#endif

//		D3DFORMAT fmt = D3DFMT_A8R8G8B8;
//#endif

		int TexWidth  = XPOW2( Ani.sprInfo.nWidth * Ani.sprInfo.nFrame );
		int TexHeight = XPOW2( Ani.sprInfo.nHeight );
		HRESULT hr ;

		hr = D3DXCreateTexture( m_D3DDevice, TexWidth,TexHeight,1, 0,fmt,D3DPOOL_MANAGED, &m_Texture[ TextureID ].lpTexture );

		m_Texture[ TextureID ].RefCnt++;
		assert( m_Texture[ TextureID ].lpTexture != NULL && "Don't Create Texture" );
		if( hr != D3D_OK )
		{
			TCHAR * aa = GetD3DErr( hr );
			fclose(fp);
			return -1;
		}

		D3DSURFACE_DESC d3dsd;
		m_Texture[ TextureID ].lpTexture->GetLevelDesc( 0, &d3dsd );
		// Lock the texture
		D3DLOCKED_RECT  d3dlr;
		hr = m_Texture[ TextureID ].lpTexture->LockRect( 0, &d3dlr, 0, 0 );//D3DLOCK_NOSYSLOCK D3DLOCK_DISCARD  
		if( hr != D3D_OK )
		{
			TCHAR* aa = GetD3DErr(hr);
			Ani.TexID = -1;
			fclose(fp);
			return -1;
		}

int ByteSize = d3dlr.Pitch* d3dsd.Height; //d3dsd.Width 
#ifdef  _4444_
		WORD* pBits = (WORD*)d3dlr.pBits;
		WORD* Img = (WORD*)malloc(  ByteSize ); 
		WORD  ClrKey =0xf000 + (( (255>>4)<<8) | (0<<4) | (255>>4)) ;
#else if _1555_
		WORD* pBits = (WORD*)d3dlr.pBits;
		WORD* Img = (WORD*)malloc(  ByteSize ); 
		WORD  ClrKey =0x8000 + (( (0xff>>3)<<10) | (0<<5) | (255>>3)) ;		

//#else if _8888_
//		DWORD* pBits = (DWORD*)d3dlr.pBits;
////		ByteSize *=4;
//		DWORD* Img = (DWORD*)malloc( ByteSize ); 
//		DWORD  ClrKey =D3DCOLOR_XRGB(255,0,255);
//		
#endif
		m_Texture[ TextureID ].lpTexture->UnlockRect( 0 );
		for( int f = 0; f< ( Ani.sprInfo.nFrame ); f++ )
		{
			int offset = Ani.sprInfo.nWidth * Ani.sprInfo.nHeight * 3 * f;
			
			for( DWORD y=0; y<(DWORD)Ani.sprInfo.nHeight; y++ )
			{
				for( DWORD x = 0; x <(DWORD) Ani.sprInfo.nWidth; x++ )
				{
					int Index = ( y * ( Ani.sprInfo.nWidth * 3 ) ) +( x * 3 );

#ifdef _4444_
					BYTE R = pBuff[ offset + Index + 0 ]>>4; //4비트씩 감쇠 시킨뒤 다시 재조합 시킴 
					BYTE G = pBuff[ offset + Index + 1 ]>>4; 
					BYTE B = pBuff[ offset + Index + 2 ]>>4; 
					WORD Clr =0xf000 + ((R<<8)|(G<<4)|(B));
					if( ClrKey != Clr ) 
					{
						Img[ (d3dsd.Width * y) + (f * Ani.sprInfo.nWidth)+ x ] = Clr;
					}
					else
					{
						Img[ (d3dsd.Width * y) + (f * Ani.sprInfo.nWidth)+ x ] = 0x0000;
					}					
#else if _1555_
					BYTE R = pBuff[ offset + Index + 0 ]>>3; //4비트씩 감쇠 시킨뒤 다시 재조합 시킴 
					BYTE G = pBuff[ offset + Index + 1 ]>>3; 
					BYTE B = pBuff[ offset + Index + 2 ]>>3; 
					WORD Clr =0x8000 + ((R<<10)|(G<<5)|(B));
					if( ClrKey != Clr ) 
					{
						Img[ (d3dsd.Width * y) + (f * Ani.sprInfo.nWidth)+ x ] = Clr;
					}
					else
					{
						Img[ (d3dsd.Width * y) + (f * Ani.sprInfo.nWidth)+ x ] = 0x0000;
					}					
//#else if _8888_
//					BYTE R = pBuff[ offset + Index + 0 ]; 
//					BYTE G = pBuff[ offset + Index + 1 ]; 
//					BYTE B = pBuff[ offset + Index + 2 ]; 
//
//					if( ClrKey != D3DCOLOR_XRGB( R,G,B) ) 
//					{
//						Img[ (d3dsd.Width * y) + (f * Ani.sprInfo.nWidth)+ x ] = D3DCOLOR_ARGB(0xff,R,G,B);
//					}
//					else
//					{
//						Img[ (d3dsd.Width * y) + (f * Ani.sprInfo.nWidth)+ x ] = 0x00000000;
//					}					
#endif

				}
			}
		}	// 메모리에서 가공을 완료함

		memcpy( pBits, Img,  ByteSize );
		free( Img );

		fclose(fp);
		SAFE_FREE( pBuff );

		Ani.TexID = TextureID;
		m_ActualImgSize[TextureID].cx = Ani.sprInfo.nWidth*Ani.sprInfo.nFrame;
		m_ActualImgSize[TextureID].cy = Ani.sprInfo.nHeight;

//		RES_ANI* pTmpAni =(RES_ANI*) malloc( sizeof( RES_ANI ));
//		*pTmpAni = Ani;
		TextureMap[ pTmpName ] = Ani;

#ifdef _HTMLLOG
//g_HtmlLog.LogInfo( _T("CD3D::LoadAni, %s , %d"), pTmpName, TextureID );	
#endif

#ifdef _DEBUGCONSOLE
//ConsoleOut( "%s load end \n", fName );
#endif

		return 1;
	}

	return -1;
}

void CD3D::FillScreen(D3DCOLOR Clr)
{
    m_D3DDevice->Clear(0,NULL,D3DCLEAR_TARGET ,Clr,1.0f, 0 ); // 배경을 모두 지운다 
}

bool	CD3D::IsDeviceLost(void )
{
	return m_bDeviceLost;
}
HRESULT CD3D::Resize3DEnvironment()
{
	ClearAllParam();
	DeleteAllStateBlock();
    HRESULT hr;
//	SAFE_RELEASE( m_DxFont );

	hr = m_DxFont->OnLostDevice();
    if( FAILED(hr) )
	{
		TCHAR* cc = g_D3D.GetD3DErr( hr );
        return hr;        
	}

	//hr = m_Sprite->OnLostDevice();
 //   if( FAILED(hr) )
	//{
	//	TCHAR* cc = g_D3D.GetD3DErr( hr );
	//	return hr;
	//}
//---------------------------------------------
	hr = m_D3DDevice->Reset( &m_d3dpp );
	if( hr != D3D_OK)
	{
		TCHAR* cc = g_D3D.GetD3DErr( hr );
		if(  hr == D3DERR_DEVICELOST )
		{
			m_bDeviceLost = true;
		}
       return hr;
	}
//---------------------------------------------

	hr = m_DxFont->OnResetDevice();
    if( FAILED(hr) )
	{
		TCHAR* cc = g_D3D.GetD3DErr( hr );
		return hr;
	}

	//hr = m_Sprite->OnResetDevice();
 //   if( FAILED( hr  ) )
	//{
	//	TCHAR* cc = g_D3D.GetD3DErr( hr );
	//	return hr;
	//}
    //----------------------------------------------------- 절취선 ㅋㅋㅋ


	MakeStateBlock();

    return S_OK;
}


HRESULT CD3D::ToggleScreen(bool bFull)
{
    m_bFull = bFull;

/*
    if( m_bFull )
        m_d3dpp.MultiSampleType    = //pDeviceInfo->MultiSampleTypeFullscreen;
    else
        m_d3dpp.MultiSampleType    = pDeviceInfo->MultiSampleTypeWindowed;
*/
    m_d3dpp.hDeviceWindow          = m_hWnd;

    D3DDISPLAYMODE d3ddm;
    if( FAILED( m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm) ) )
		return E_FAIL;

		m_d3dpp.BackBufferWidth		= m_WinWidth;
		m_d3dpp.BackBufferHeight	= m_WinHeight;
	    m_d3dpp.Windowed          = m_bFull ? false : true; 
//		m_d3dpp.SwapEffect        = m_d3dpp.Windowed ? D3DSWAPEFFECT_FLIP:D3DSWAPEFFECT_COPY ;//;D3DSWAPEFFECT_DISCARD//  
		m_d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.hDeviceWindow     = m_hWnd;
		m_d3dpp.BackBufferFormat  = m_bFull ? D3DFMT_R5G6B5 : m_D3DDisplayMode.Format;
		m_d3dpp.BackBufferCount   = 1;
		m_d3dpp.EnableAutoDepthStencil = FALSE;
		m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;// DEF_FIXEDFRAME;
		m_d3dpp.PresentationInterval =D3DPRESENT_INTERVAL_IMMEDIATE;//  D3DPRESENT_INTERVAL_DEFAULT:  ;//   ;//  D3DPRESENT_INTERVAL_ONE

	if( m_bFull)
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_WinWidth, m_WinHeight, SWP_SHOWWINDOW);// 	| SWP_NOSIZE | SWP_NOMOVE

    // Resize the 3D device
    if( FAILED( Resize3DEnvironment() ) )
    {
        if( m_bFull ) return E_FAIL;
    }
//	if( m_bFull)
//	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_WinWidth, m_WinHeight, SWP_SHOWWINDOW| SWP_NOSIZE | SWP_NOMOVE );// 	


		if( m_bFull == false )
		{
			int	winx = GetSystemMetrics(SM_CXSCREEN)/2 - (m_WinWidth/2);
			int	winy = GetSystemMetrics(SM_CYSCREEN)/2 - (m_WinHeight/2);
		//	RECT rt={0,0,m_WinWidth,m_WinHeight};
		//	OffsetRect(&rt,winx,winy);
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST, winx,winy,800,600, SWP_SHOWWINDOW);
		}

 	
    return S_OK;
}

void CD3D::DrawText_CalcRECT( int x, int y, RECT&rt, const TCHAR* pText )
{
	m_DxFont->DrawText( NULL, pText, -1, &rt,DT_LEFT | DT_CALCRECT, 0);
	OffsetRect( &rt, x,y );
}

void CD3D::DrawText(RECT& rt, D3DCOLOR rgbFontColour,DWORD Flag , const TCHAR* pText )
{
//	TCHAR tmp [256]={0,};
//	_tcscpy( tmp, pText);
	////DT_NOPREFIX |DT_PATH_ELLIPSIS | DT_MODIFYSTRING|DT_END_ELLIPSIS 
	
	HRESULT hr;
	int len = (int)_tcslen( pText );
	hr = m_DxFont->DrawText( NULL,pText, len, &rt,Flag, rgbFontColour);	//DT_WORD_ELLIPSIS  
	
//	char * ttt = GetD3DErr(hr);
//	assert( hr == D3D_OK);
}

/*
inline HRESULT CD3D::Draw(int Index,  const RECT* pSrcRect,  const D3DXVECTOR2* pScaling,  
					const D3DXVECTOR2* pRotationCenter,	float Rotation,	const D3DXVECTOR2* pTranslation,  D3DCOLOR Color )
{
	assert( Index!= -1 );
	assert( m_Texture[Index].lpTexture != NULL );
	HRESULT hr = m_Sprite->Draw( m_Texture[Index].lpTexture, pSrcRect, pScaling, pRotationCenter,	Rotation, pTranslation, Color );
	D3DSURFACE_DESC Desc;

	return hr;
}*/
#ifdef rhw
inline HRESULT CD3D::Draw(int Index,  const RECT& srcRt, const RECT& destRt, D3DCOLOR Color )
{
	assert( Index!= -1 );
	assert( m_Texture[Index].lpTexture != NULL );
		HRESULT hr=~D3D_OK;
		D3DSURFACE_DESC Desc;
		m_Texture[Index].lpTexture->GetLevelDesc(0, &Desc);
		float texWidth = (float)Desc.Width;
		float texHeight= (float)Desc.Height; 

		m_vtx[0].Pos = D3DXVECTOR4( (float)destRt.left-0.5f, (float)destRt.top-0.5f,0.0f,1.0f); //좌상에서부터 시계 방향으로 
		m_vtx[0].Clr = Color;
		m_vtx[0].tu  = srcRt.left / texWidth;
		m_vtx[0].tv  = srcRt.top  / texHeight;
		m_vtx[1].Pos = D3DXVECTOR4((float)destRt.right,(float)destRt.top-0.5f,0.0f,1.0f);
		m_vtx[1].Clr = Color;
		m_vtx[1].tu  = srcRt.right / texWidth;
		m_vtx[1].tv  = srcRt.top  / texHeight;
		m_vtx[2].Pos = D3DXVECTOR4((float)destRt.right,(float)destRt.bottom,0.0f,1.0f);
		m_vtx[2].Clr = Color;
		m_vtx[2].tu  = srcRt.right / texWidth;
		m_vtx[2].tv  = srcRt.bottom  / texHeight;
		m_vtx[3].Pos = D3DXVECTOR4((float)destRt.left-0.5f,(float)destRt.bottom,0.0f,1.0f);
		m_vtx[3].Clr = Color;
		m_vtx[3].tu  = srcRt.left / texWidth;
		m_vtx[3].tv  = srcRt.bottom  / texHeight;
		hr = m_D3DDevice->SetTexture(0, m_Texture[Index].lpTexture );
		assert( hr == D3D_OK);

		hr = m_D3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2,m_vtx, sizeof(SpriteVertex) );	//D3DPT_TRIANGLESTRIP
		assert( hr == D3D_OK);
		m_D3DDevice->SetTexture(0, NULL );
	return hr;
}
#else
inline HRESULT CD3D::Draw(int Index,  const RECT& srcRt, const RECT& destRt, D3DCOLOR Color )
{
	HRESULT hr;
		D3DSURFACE_DESC Desc;
		m_Texture[Index].lpTexture->GetLevelDesc(0, &Desc);
		float texWidth = (float)Desc.Width;
		float texHeight= (float)Desc.Height; 
		long x = destRt.left;
		long y = destRt.top;
		long Width= destRt.right - destRt.left;
		long Height = destRt.bottom - destRt.top;
		m_matMoveScale._11 = (float)Width;
		m_matMoveScale._22 = (float)Height;
		m_matMoveScale._41 = (float)x;
		m_matMoveScale._42 = (float)y;

		m_matTex._11 = ((srcRt.right-srcRt.left) /texWidth )  / Width;
		m_matTex._22 = ((srcRt.bottom-srcRt.top)/texHeight ) / Height;
		m_matTex._41 = (srcRt.left/texWidth)  - m_matTex._11* x;
		m_matTex._42 = (srcRt.top/texHeight )  - m_matTex._22* y; 

		m_D3DDevice->SetTransform( D3DTS_VIEW, &m_matMoveScale);
		m_D3DDevice->SetTransform( D3DTS_TEXTURE0, &m_matTex);

		m_D3DDevice->SetTexture(0, m_Texture[Index].lpTexture );
		m_D3DDevice->SetRenderState( D3DRS_TEXTUREFACTOR, Color );
		hr = m_D3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, m_vtx, sizeof(SpriteVertex) );	//D3DPT_TRIANGLESTRIP
		assert( hr == D3D_OK );
		m_D3DDevice->SetTexture(0, NULL );
		return hr;
}
#endif

// 특정좌표에 이미지를 올림 
void CD3D::BltSprite(BYTE Layer, int TxtNum, float x,float y,D3DCONST RenderMode )
{
	assert( TxtNum != -1 );
	assert( m_Texture[TxtNum].lpTexture != NULL );
	
//	m_BltSprite[ CBltSprite::BltSpriteCnt ].BltSprite( TxtNum,x,y,RenderMode );
	RECT srcRt,destRt;
	SetRect( &srcRt, 0,0,m_ActualImgSize[TxtNum].cx, m_ActualImgSize[TxtNum].cy);
	SetRect( &destRt, (long)x,(long)y,(long)(x+m_ActualImgSize[TxtNum].cx),(long)(y+m_ActualImgSize[TxtNum].cy) );
	m_BltSprite[ CBltSprite::BltSpriteCnt ].BltSprite( TxtNum, srcRt, destRt,0xffffffff, RenderMode );
	m_DrawObj[ Layer ][ m_DrawObjCnt[Layer] ] = &m_BltSprite[ CBltSprite::BltSpriteCnt ];
	CBltSprite::BltSpriteCnt++;
	m_DrawObjCnt[Layer]++;
}



//-------------------------------------------------------------------
void CD3D::BltSprite( BYTE Layer, int TextureNum, RECT *pSrcRect,const RECT& DestRect ) // 매개 변수를 큐로 집어넣는다.
{
	assert( TextureNum != -1 );
	assert( m_Texture[ TextureNum ].lpTexture != NULL );
	assert( m_DrawParamCnt[ Layer ] < MAX_DRAWPARAM );
	RECT srcRect;
	if( pSrcRect == NULL)
	{
		SetRect(&srcRect, 0,0,m_ActualImgSize[TextureNum].cx,m_ActualImgSize[TextureNum].cy);
	}
	else
	{
		srcRect = *pSrcRect;
	}
//     D3DXVECTOR2 scaling(1,1), rcenter(0,0), trans(0,0);
     // 찍힐 위치 
  /* if (pDestRect) 
	 {
          trans.x = (float) pDestRect->left;
          trans.y = (float) pDestRect->top;
     } 

     // Scaling - 소스와 대상 사각형의 비율을 1.0 기준으로 변환 
     if (
		( pDestRect->left-trans.x   == pSrcRect->left   ) &&
		( pDestRect->top- trans.y   == pSrcRect->top    ) &&
		( pDestRect->right-trans.x  == pSrcRect->right  ) &&
		( pDestRect->bottom -trans.y== pSrcRect->bottom ) 
		) // 모든게 같으면 스케일링을 1:1 로 한다 
	 {
			scaling.x = 1.0f;
			scaling.y = 1.0f;
     }
	 else
	 {
          scaling.x = ((float) (pDestRect->right - pDestRect->left)) / ((float) (pSrcRect->right - pSrcRect->left));
          scaling.y = ((float) (pDestRect->bottom - pDestRect->top)) / ((float) (pSrcRect->bottom - pSrcRect->top));
	 }*/

	m_BltSprite[ CBltSprite::BltSpriteCnt ].BltSprite( TextureNum,srcRect ,DestRect,0xffffffff, D3D_NORMALSTATE ); 
	m_DrawObj[ Layer ][ m_DrawObjCnt[Layer] ] = &m_BltSprite[ CBltSprite::BltSpriteCnt ];
	CBltSprite::BltSpriteCnt++;
	m_DrawObjCnt[Layer]++;
}


//-------------------------------------------------------------------
void CD3D::BltSprite( BYTE Layer, int TextureNum, RECT *pSrcRect, const RECT& DestRect, D3DCOLOR Clr,D3DCONST RenderMode ) // 매개 변수를 큐로 집어넣는다.
{
	assert( TextureNum != -1 );
	assert( m_Texture[ TextureNum ].lpTexture != NULL );
	assert( m_DrawParamCnt[ Layer ] < MAX_DRAWPARAM );

	RECT srcRt;
    if( pSrcRect == NULL )
	{
		SetRect(&srcRt, 0,0, m_ActualImgSize[ TextureNum].cx, m_ActualImgSize[ TextureNum].cy);
		
	}
	else
	{
		srcRt = *pSrcRect;
	}

	m_BltSprite[ CBltSprite::BltSpriteCnt ].BltSprite( TextureNum, srcRt, DestRect, Clr,RenderMode ); 
	m_DrawObj[ Layer ][ m_DrawObjCnt[Layer] ] = &m_BltSprite[ CBltSprite::BltSpriteCnt ];
	CBltSprite::BltSpriteCnt++;
	m_DrawObjCnt[Layer]++;
}

void CD3D::quick_sort( CBltSprite** Arr, int Size ) // 구조체 버젼 
{
	// Arr 은 포인터를 가진 배열이고 
	// 그 포인터들을 소트하여 준다
    int v;
	int i, j; 
	static CBltSprite* tmp = NULL;

    if( Size > 1)    // terminal condition 
	{
		v = (int)(Arr[ Size - 1 ])->GetSortY();
		assert( v > 0 );
		i = -1;
		j = Size - 1;
		
		while ( 1 )      // partition 
		{
			while ( (Arr[ ++i ])->GetSortY()  < v ){ if( i>=j) break;};
			while ( (Arr[ --j ])->GetSortY()  > v ){ if( i>=j) break; };

			if (i >= j) break;
			
			tmp = Arr[i];
			Arr[i] = Arr[j];
			Arr[j] = tmp;
		}

		tmp = Arr[i];
		Arr[i] = Arr[Size - 1] ;
		Arr[Size - 1] = tmp;		

		quick_sort( Arr,     i);
		quick_sort( Arr+i+1, Size - i - 1 );
	}
}






using namespace std ;

class cmp
{

public:
	cmp(){};
	~cmp(){};
	bool inline operator()( CBltSprite& x,CBltSprite& y )
	{
	return x.GetSortY() < y.GetSortY();	
	}

};


void CD3D::Present()
{
	HRESULT hr = D3D_OK;
	int Index = 0;
	if( m_bDeviceLost == true )
	{
		if( FAILED( hr = m_D3DDevice->TestCooperativeLevel() ) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				m_bDeviceLost = false;
				ClearAllParam();
#ifdef _DEBUGCONSOLE
				ConsoleOut("D3DERR_DEVICELOST \n" );
#endif			
				return;
			}
			
			if( D3DERR_DEVICENOTRESET == hr )
			{
				if( FAILED( hr = Resize3DEnvironment() ) )
				{
					Sleep( 100 );
					if( D3DERR_DEVICENOTRESET == hr )
					m_bDeviceLost = true;
					else 
					m_bDeviceLost = false;
#ifdef _DEBUGCONSOLE
					ConsoleOut("D3DERR_DEVICENOTRESET \n" );
//		g_HtmlLog.LogError( _T("Resize3DEnvironment(),%s"), GetD3DErr( hr ) );
#endif

					return;
				}

			}
		}
	}
//	FillScreen();
	//this->FillRect( NULL, 0xff0000ff);
    if( m_D3DDevice->BeginScene() == D3D_OK ) // 그리기 상태 시작 
	{
		quick_sort( (CBltSprite**)m_DrawObj[ D3D_Y_SORT_LAYER0 ], 
			m_DrawObjCnt[ D3D_Y_SORT_LAYER0 ] );
		//		if( m_DrawObjCnt[ D3D_Y_SORT_LAYER0 ] > 0 )
		{
			//			CBltSprite** pBlt = (CBltSprite**)m_DrawObj[D3D_Y_SORT_LAYER0];
			//			sort( (CBltSprite*)(*pBlt), (*pBlt)+m_DrawObjCnt[ D3D_Y_SORT_LAYER0 ] , cmp() );
		}

		//		if( m_Sprite->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK ) return;
		//		if( m_DxFont->Begin() != D3D_OK ) return;

		for( int Layer = 0; Layer < MAX_LAYERCNT; Layer++)
		{
			for( Index = 0 ; Index < m_DrawObjCnt[ Layer ]; Index++ )
			{
				if( hr == D3D_OK)
				{
					assert( m_DrawObj[ Layer ][ Index ] != NULL );
					m_DrawObj[ Layer ][ Index ]->Draw();
				}
			}
			m_DrawObjCnt[ Layer ] = 0;
			CBltSprite::BltSpriteCnt = 0;
			CBltText::BltTextCnt = 0;
		}
		//		m_DxFont->End();	

		m_D3DDevice->EndScene();		// 그리기 상태끝
	}
		hr = m_D3DDevice->Present(NULL,NULL,NULL,NULL); // 화면에 모조리 전송하기
		if( hr != D3D_OK ) //D3DERR_DEVICELOST 
		{
			if( hr == D3DERR_DEVICELOST )
				m_bDeviceLost = true;
#ifdef _HTMLLOG
			g_HtmlLog.LogError( _T("Present Error %s"),GetD3DErr( hr ) );
#endif
		}
}

// saves an array of pixels as a TGA image
int CD3D::WriteTGAFile(const TCHAR* fName, short int width, short int height, unsigned char* imageData) 
{
	unsigned char byteSkip;			// used to fill in the data fields that we don't care about
	short int     shortSkip;
	unsigned char imageType;		// type of image we're writing to file
	int           colorMode;
	unsigned char colorSwap = 0;
	int			  imageIdx = 0;
	unsigned char bitDepth;
	long		  imageSize;
	FILE *filePtr;

	// create file for writing binary mode
	filePtr = _tfopen(fName, _T("wb") );
	if (!filePtr)
	{
		fclose(filePtr);
		return 0;
	}

	imageType = 2;		// RGB, uncompressed
	bitDepth = 32;		// 24-bitdepth
	colorMode = 4;		// RGB color mode

	byteSkip = 0;
	shortSkip = 0;

	// write 2 bytes of blank data
	fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);
	fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);

	// write imageType
	fwrite(&imageType, sizeof(unsigned char), 1, filePtr);

	fwrite(&byteSkip, sizeof(unsigned char), 5, filePtr);
//	fwrite(&shortSkip, sizeof(short int), 1, filePtr);
//	fwrite(&shortSkip, sizeof(short int), 1, filePtr);
//	fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);
//	fwrite(&shortSkip, sizeof(short int), 1, filePtr);
//	fwrite(&shortSkip, sizeof(short int), 1, filePtr);

	// write image dimensions
	fwrite(&width, sizeof(short int), 1, filePtr);
	fwrite(&height, sizeof(short int), 1, filePtr);
	fwrite(&bitDepth, sizeof(unsigned char), 1, filePtr);

	// write 1 byte of blank data
	byteSkip = 8;
	fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);

	// calculate the image size
	imageSize = width * height * colorMode;

	// change image data from RGB to BGR
	for (imageIdx = 0; imageIdx < imageSize ; imageIdx += colorMode) 
	{
		BYTE A = imageData[imageIdx+0];
		BYTE R = imageData[imageIdx+1];
		BYTE G = imageData[imageIdx+2];
		BYTE B = imageData[imageIdx+3];

		imageData[imageIdx+0] = G ;//G
		imageData[imageIdx+1] = R ;//R
		imageData[imageIdx+2] = A ;//A
		imageData[imageIdx+3] = B ;//B
	}

	// write the image data
	fwrite(imageData, sizeof(unsigned char)*4, imageSize/4, filePtr );

	// close the file
	fclose(filePtr);

	return 1;
}

int CD3D::ScreenShot(const TCHAR* fName )
{

#define __TEST
		HRESULT hr = 0 ;
#ifndef __TEST
		{
			
			// 여기서 시간을 뿌려줍니다.
			// 년 월 일 시 초 
			time_t tt;
			time( &tt );
			TCHAR ampm[] ={ _T("오전") };
			TCHAR szTime[32]={0,};
			struct tm* today;
			today = localtime( &tt );
			if( today->tm_hour > 12 )
			{
				strcpy( ampm, "오후" );
				today->tm_hour -= 12;
			}
			if( today->tm_hour == 0 )  
				today->tm_hour = 12;
			
			_stprintf(szTime, "%4d년 %s %.8s", today->tm_year+1900, ampm, asctime( today ) + 11 );
			RECT rt={0,};

			DrawText_CalcRECT( 20, 580, rt, szTime );

			OffsetRect( &rt, -2,-2);
			DrawText( rt,0xff000000, szTime );
			OffsetRect( &rt, 1, 0 );
			DrawText( rt,0xff000000, szTime );
			OffsetRect( &rt, 1, 0 );
			DrawText( rt,0xff000000, szTime );

			OffsetRect( &rt, -2, 1 );
			DrawText( rt,0xff000000, szTime );

			OffsetRect( &rt, 2, 0 );
			DrawText( rt,0xff000000, szTime );

			OffsetRect( &rt, -2, 1 );
			DrawText( rt,0xff000000, szTime );
			OffsetRect( &rt, 1, 0 );
			DrawText( rt,0xff000000, szTime );
			OffsetRect( &rt, 1, 0 );
			DrawText( rt,0xff000000, szTime );

			OffsetRect( &rt, -1, -1 );			
			DrawText( rt,0xffffffff, szTime );


			m_D3DDevice->Present(NULL,NULL,NULL,NULL); // 화면에 모조리 전송하기

			LPDIRECT3DSURFACE9 frontbuf,syssurf = NULL;
			hr = m_D3DDevice->GetRenderTarget(0, &frontbuf );
			m_D3DDevice->GetRenderTargetData( frontbuf, syssurf);
			if(hr != D3D_OK)
			{
				TCHAR* aa = GetD3DErr( hr );
				frontbuf->Release(); 
				return -1;
			}
			hr = D3DXSaveSurfaceToFile( fName, D3DXIFF_BMP, frontbuf, NULL, NULL); // D3DXIFF_BMP
			SAFE_RELEASE( frontbuf );
			SAFE_RELEASE( syssurf );
		}
#else
	{
		LPDIRECT3DSURFACE9 testsf,syssurf;
		hr = m_D3DDevice->GetRenderTarget(0, &testsf);
		m_D3DDevice->CreateOffscreenPlainSurface( 800,600,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,&syssurf,NULL);
		m_D3DDevice->GetRenderTargetData( testsf, syssurf);
		TCHAR* tttttt = GetD3DErr( hr );
		int testx=800 ,testy=600;
		D3DLOCKED_RECT rt;
		hr = syssurf->LockRect( &rt, 0, D3DLOCK_READONLY    );	//  
		WORD* pTmp = (WORD*)rt.pBits;
		WriteTGAFile( fName,testx,testy, (unsigned char*)pTmp );
		syssurf->UnlockRect();
		SAFE_RELEASE( testsf );
		SAFE_RELEASE( syssurf );
	}
#endif	
	return 0;
}

void CD3D::ClearAllParam()
{
	m_DrawParamCnt[ 0 ] = 0;
	m_DrawParamCnt[ 1 ] = 0;
	m_DrawParamCnt[ 2 ] = 0;

	m_DrawObjCnt[ 0 ] = 0;
	m_DrawObjCnt[ 1 ] = 0;
	m_DrawObjCnt[ 2 ] = 0;
	m_DrawObjCnt[ 3 ] = 0;
	m_DrawObjCnt[ 4 ] = 0;
	m_DrawObjCnt[ 5 ] = 0;
	m_DrawObjCnt[ 6 ] = 0;
	CBltSprite::BltSpriteCnt = 0;
	CBltText::BltTextCnt = 0;
}

HRESULT CD3D::MakeStateBlock()
{
	HRESULT hr = m_D3DDevice->BeginStateBlock();
	assert( hr == D3D_OK ) ;
	m_D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE		);
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE );
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO  );
	m_D3DDevice->EndStateBlock( &m_hStateBlock[ D3D_NORMALSTATE ]	);
	
	hr = m_D3DDevice->BeginStateBlock();
	assert( hr == D3D_OK ) ;
	m_D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE		);
	m_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE		);
	m_D3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE		);
	m_D3DDevice->EndStateBlock( &m_hStateBlock[ D3D_GASANSTATE ]	);

	hr = m_D3DDevice->BeginStateBlock();
	assert( hr == D3D_OK ) ;
	m_D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE			);
	m_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA		);
	m_D3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA	);
	m_D3DDevice->EndStateBlock( &m_hStateBlock[ D3D_ALPHASTATE ]		);

//	m_D3DDevice->ApplyStateBlock( m_hStateBlock[D3D_ALPHASTATE ]);

	m_D3DDevice->SetFVF( SpriteVertex::D3DFVF_SPRITEVERTEX );
	m_D3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE			);
	m_D3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA		);
	m_D3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA	);

    m_D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// 투명색 적용을 위한 알파테스트. (알파값이 0이면 투명색처리)
	m_D3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE);
	m_D3DDevice->SetRenderState( D3DRS_ALPHAREF , 0x0 );
	m_D3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL ); 

#ifdef rhw
	m_D3DDevice->SetTextureStageState( 0,D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_D3DDevice->SetTextureStageState( 0,D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	m_D3DDevice->SetTextureStageState( 0,D3DTSS_ALPHAARG2, D3DTA_TEXTURE );//DIFFUSE 
    m_D3DDevice->SetTextureStageState( 0,D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_D3DDevice->SetTextureStageState( 0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    m_D3DDevice->SetTextureStageState( 0,D3DTSS_COLORARG2, D3DTA_TEXTURE  );
#else
	m_D3DDevice->SetTextureStageState( 0,D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_D3DDevice->SetTextureStageState( 0,D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_D3DDevice->SetTextureStageState( 0,D3DTSS_ALPHAARG2, D3DTA_TEXTURE );//DIFFUSE 
    m_D3DDevice->SetTextureStageState( 0,D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_D3DDevice->SetTextureStageState( 0,D3DTSS_COLORARG1, D3DTA_TFACTOR);
    m_D3DDevice->SetTextureStageState( 0,D3DTSS_COLORARG2, D3DTA_TEXTURE  );
#endif
	// D3DCMPFUNC에 대한 정의 디폴트: D3DCMP_ALWAYS
	// 알파테스트 사용 유/무 디폴트: FALSE
	//	D3DRS_ALPHAREF // 시험 픽셀값 0x00 ~ 0xff
	//	D3DRS_ALPHAFUNC // D3DCMPFUNC에 대한 정의 디폴트: D3DCMP_ALWAYS
	// 알파테스트에 사용될 알파값 = TextureAlpha * ShadeAlpha
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);//D3DTEXF_LINEAR
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);//D3DTEXF_LINEAR
	m_D3DDevice->SetSamplerState(0,	D3DSAMP_MIPFILTER, D3DTEXF_NONE);

/*
	m_D3DDevice->SetTextureStageState(	0,	D3DTSS_ADDRESSU,D3DTADDRESS_CLAMP);
	m_D3DDevice->SetTextureStageState(	0,	D3DTSS_ADDRESSV,D3DTADDRESS_CLAMP);*/


	// 폴리곤 그릴때 뒷면은 안그린다.
    m_D3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    m_D3DDevice->SetRenderState( D3DRS_DITHERENABLE, FALSE );
    m_D3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
    m_D3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );


//----------------------------------------------------------------------------
#ifdef rhw

#else
	m_D3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
	m_D3DDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
	m_vtx[0].Pos = D3DXVECTOR3( 0,0,0);
	m_vtx[1].Pos = D3DXVECTOR3( 1,0,0);
	m_vtx[2].Pos = D3DXVECTOR3( 1,1,0);
	m_vtx[3].Pos = D3DXVECTOR3( 0,1,0);
#endif

	D3DXMATRIX projection;
	D3DXMATRIX identity;
	D3DXMatrixIdentity( &identity );
	m_matMoveScale = m_matTex = identity;

	m_D3DDevice->SetTransform( D3DTS_VIEW, &identity );
	m_D3DDevice->SetTransform( D3DTS_WORLD, &identity );
	D3DXMatrixOrthoOffCenterLH( &projection , 0,(float)m_WinWidth,(float)m_WinHeight, 0, 0, 1 );
	D3DXMATRIX offset;
	D3DXMatrixTranslation( &offset, -0.5, -0.5, 0 );
	projection = offset * projection ;
	m_D3DDevice->SetTransform( D3DTS_PROJECTION, &projection );//projection 


/*
    D3DXMATRIX matWorld;
    D3DXMATRIX matView;
    D3DXMATRIX matProj;
	D3DXMATRIX matMove;
    D3DXMatrixIdentity(&matWorld);

	D3DXMatrixTranslation( &matMove, 3.0f, 10.0f, 0.0f);
	D3DXMatrixMultiply( &matWorld, &matWorld, &matMove);

    m_D3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
    
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f, -3.5f ),
                                 &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ) ); // 머리 방향 
    m_D3DDevice->SetTransform(D3DTS_VIEW, &matView);


    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 0.5f, 1.0f, 100.0f );
    m_D3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
*/
	
	return hr;	
}

TCHAR* CD3D::GetD3DErr(HRESULT hr)
{	
	switch( hr)
	{
	case D3DXERR_INVALIDDATA:
		return _T("데이타가 잘못됐어요");

	case D3DERR_WRONGTEXTUREFORMAT:
		return _T("텍스쳐포멧이 잘못됐음돠.");
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		return _T("지원되지 않는 색조합연산임돠.");
	case D3DERR_UNSUPPORTEDCOLORARG:
		return _T("지원되지 않는 색인자임돠.");
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		return _T("지원되지 않는 알파연산임돠.");
	case D3DERR_UNSUPPORTEDALPHAARG:
		return _T("지원되지 않는 알파 인자임돠.");
	case D3DERR_TOOMANYOPERATIONS:
		return _T("연산이 너무 많습니다.");
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		return _T("텍스쳐필터조합이 엉망임돠.");
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		return _T("UNSUPPORTEDFACTORVALUE");
	case D3DERR_CONFLICTINGRENDERSTATE:
		return _T("렌더링상태조합이 엉망임돠.");
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		return _T("지원하지 않는 텍스쳐 필더임돠.");
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		return _T("텍스쳐 팔레트가 엉망임돠");
	case D3DERR_DRIVERINTERNALERROR:
		return _T("드라이버 내부 에러임돠");
	case D3DERR_NOTFOUND:
		return _T("찾을수 없음돠.");
	case D3DERR_MOREDATA:
		return _T("MOREDATA");
	case D3DERR_DEVICELOST:
		return _T("DEVICELOST");
	case D3DERR_DEVICENOTRESET:
		return _T("DEVICENOTRESET");
	case D3DERR_NOTAVAILABLE:
		return _T("NOTAVAILABLE");
	case D3DERR_OUTOFVIDEOMEMORY:
		return _T("OUTOFVIDEOMEMORY");
	case D3DERR_INVALIDDEVICE:
		return _T("INVALIDDEVICE");
	case D3DERR_INVALIDCALL:
		return _T("INVALIDCALL");
	case D3DERR_DRIVERINVALIDCALL:
		return _T("DRIVERINVALIDCALL");
	}
	return _T("해당하는 에러가 없슴돠.");
}


bool CD3D::IsTransparentColor(int TextureID, int x, int y, DWORD ClrKey )
{	// 현재 8888 만 지원된다
	// 4444 나 1555 등도 지원하게 해야 함.
	// 4444 enabled 

	HRESULT hr;
	bool ret = false;
	D3DSURFACE_DESC d3dsd;
	assert( m_Texture[ TextureID ].lpTexture != NULL );
	hr = m_Texture[ TextureID ].lpTexture->GetLevelDesc( 0, &d3dsd );
	DWORD w = m_ActualImgSize[ TextureID ].cx;
	DWORD h = m_ActualImgSize[ TextureID ].cy;
	if( x <= 0 ) x = 0;
	if( y <= 0 ) y = 0;
	x %= w ;
	y %= h ;
	
	D3DLOCKED_RECT  d3dlr={0,};
	const RECT region= {0,0,w,h };
	hr = m_Texture[ TextureID ].lpTexture->LockRect( 0, &d3dlr, &region, D3DLOCK_READONLY  );

	m_Texture[ TextureID ].lpTexture->UnlockRect( 0 );

    BYTE r =BYTE( ((ClrKey&0x00ff0000)>>16) );
    BYTE g =BYTE( ((ClrKey&0x0000ff00)>>8)  );
    BYTE b =BYTE( ((ClrKey&0x000000ff)>>0)  );

	switch( d3dsd.Format )
	{
		case D3DFMT_A8R8G8B8:
			{
				D3DCOLOR Clr = ((DWORD*)d3dlr.pBits)[ (d3dsd.Width * y) + (x) ]; // Alpha 성분만 가져오기
				D3DCOLOR key = D3DCOLOR_ARGB(0x00,r, g, b );
				if( Clr  == key )
					ret = true;
			}
			break;

		case D3DFMT_A4R4G4B4:
			{
				WORD	Clr = ((WORD*)d3dlr.pBits)[ (d3dsd.Width * y) + x ]; // Alpha 성분만 가져오기
				WORD key = (r<<8| g<<4| b) ;
				if( Clr  == key )
					ret = true;				
			}
			break;

		case D3DFMT_A1R5G5B5:
			{
			
			}
			break;
	}

	return ret;
}
/*
void CD3D::SetGamma( float gamma, int overbright )
{
    D3DGAMMARAMP ramp;
    float f;
	m_D3DDevice->GetGammaRamp( &ramp );
	
    for ( int i = 0; i < 256; i++ )
	{
        f = (float)(255 * pow((float)i/256, 1/gamma));
        f = f + overbright;
        if (f < 0)
            f = 0;
        if (f > 255)
            f = 255;
		ramp.red[i] = ramp.green[i] = ramp.blue[i] = ( (WORD)f<< 8 );
    }
	m_D3DDevice->SetGammaRamp( D3DSGR_CALIBRATE , &ramp );
}*/


void CD3D::SetGamma( long gamma )
{
    D3DGAMMARAMP ramp;
	m_D3DDevice->GetGammaRamp( 0, &ramp );
	ZeroMemory( &ramp, sizeof(ramp) );

    WORD tmpGamma = 0;

    for ( int i = 0; i < 256; i++ )
	{
		ramp.red[i] = ramp.green[i] = ramp.blue[i] = tmpGamma;
	tmpGamma = tmpGamma + (WORD)gamma;
    }
    m_D3DDevice->SetGammaRamp( 0, D3DSGR_CALIBRATE , &ramp );
}

RES_ANI* CD3D::GetResAni( const TCHAR* fName )
{
	TCHAR pTmpName[MAX_PATH];
	_tcscpy( pTmpName, fName );	
	_tcsupr( pTmpName);
	return	&TextureMap[ pTmpName ];
}

#pragma comment(lib, "ddraw.lib" )
void GetVideMem(DWORD & Total, DWORD & Free)
{
   LPDIRECTDRAW7 lpDD = NULL;
   DDSCAPS2      ddsCaps2; 

   HRESULT       hr; 
   hr = DirectDrawCreateEx(NULL, (VOID**)&lpDD, IID_IDirectDraw7, NULL );
   if (FAILED(hr))
       return ; 

   // Initialize the structure.
   ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));

   ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM; 
   hr = lpDD->GetAvailableVidMem(&ddsCaps2, &Total, &Free); 
   if (FAILED(hr))
       return ;
	   if(lpDD)	   lpDD->Release();

}
#ifndef _OUT_
#define _OUT_
#endif
#ifndef _IN_
#define _IN_
#endif

bool CD3D::CreateBlankTexture( _OUT_ int&paramIndex, int Width,int Height )
{
	paramIndex = -1;
	FIND_EMPTYSLOT( paramIndex );
	assert( paramIndex != -1 );
	int powx = XPOW2(Width);
	int powy = XPOW2(Height);
	HRESULT hr = ~D3D_OK;//D3DPOOL_SYSTEMMEM D3DFMT_A8R8G8B8
	hr = m_D3DDevice->CreateTexture(	powx, powy, 1,  0,D3DFMT_A8R8G8B8, 
										D3DPOOL_MANAGED,&m_Texture[ paramIndex ].lpTexture,	NULL ); 
	assert( m_Texture[ paramIndex ].lpTexture != NULL );
	
	m_ActualImgSize[paramIndex].cx = Width;
	m_ActualImgSize[paramIndex].cy = Height;
	return true;
}

void	CD3D::LoadStream(int & Index,const string& imgstream )
{
	Index=-1;
	FIND_EMPTYSLOT( Index );
	HRESULT hr = ~D3D_OK;
	D3DXIMAGE_INFO Info;	
	LPDIRECT3DTEXTURE9 Tex=NULL;
	D3DXCreateTextureFromFileInMemoryEx( m_D3DDevice,imgstream.c_str(), (UINT)imgstream.size(),D3DX_DEFAULT,D3DX_DEFAULT,1,
		0,D3DFMT_R5G6B5, D3DPOOL_MANAGED, D3DX_FILTER_NONE,D3DX_FILTER_NONE,0,&Info,NULL,&m_Texture[Index].lpTexture );

	assert( m_Texture[ Index ].lpTexture != NULL );
	m_ActualImgSize[Index].cx = Info.Width;
	m_ActualImgSize[Index].cy = Info.Height;
}


void CD3D::ReleaseTexture(int Index)
{
	if( Index < 0 || Index > MAX_TEXTURE )
	{
		return;
	}
		
	if( m_Texture[Index].lpTexture !=NULL)
	{
		m_Texture[Index].RefCnt --;
		if(  m_Texture[Index].RefCnt <=0 )
		{
			SAFE_RELEASE( m_Texture[Index].lpTexture );
			m_Texture[Index].RefCnt = 0;
			m_ActualImgSize[Index].cx = 0;
			m_ActualImgSize[Index].cy = 0;
		}
	}
	
}

