// ResMgr.cpp: implementation of the ResMgr class.
//
//////////////////////////////////////////////////////////////////////
#include <stdafx.h>
#include "baseobj.h"
#include "resmgr.h"
//#include "block.h"
//#include "map.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ResMgr			g_ResMgr;

ResMgr::ResMgr()
{

}

ResMgr::~ResMgr()
{
	ReleaseAll();
}


void ResMgr::ReleaseAll()
{
	// 유닛, 블럭, 아템  모두 지운다 
	ReleaseAllUnitAni(); 
	ReleaseAllBlockAni(); 
	ReleaseAllItemAni();
}

void ResMgr::ReleaseAllItemAni()
{
	RES_ITOR itor = m_ItemAniList.begin();
	while( itor != m_ItemAniList.end() )
	{
		RES_ANI* pAni =(RES_ANI*)(itor->second);
		g_D3D.ReleaseANI( pAni->TexID );
		SAFE_FREE( pAni );
		itor++;
	}
	m_ItemAniList.clear();
}

void ResMgr::ReleaseAllBlockAni()
{
	size_t cnt = m_BlockAniList.size();
#ifdef _HTMLLOG
	g_HtmlLog.LogInfo( _T("현재 블럭텍스쳐수  %d 개 "), cnt );
	g_HtmlLog.LogInfo( _T("전 텍스쳐 [ %d  개 ]"), g_D3D.GetTextureCNT() );
#endif
	RES_ITOR itor = m_BlockAniList.begin();

	while( itor != m_BlockAniList.end() )
	{
		RES_ANI* pAni = ( RES_ANI* )( itor->second );
		tstring sz = (itor->first);
		for( int i=0; i< 2/*8*/; i++ )
		{
			if( pAni != NULL )
			{
				if( pAni[i].TexID != -1 )
				g_D3D.ReleaseANI( pAni[i].TexID );
			}
		}
		SAFE_FREE( pAni );
		itor++;
	}
	m_BlockAniList.clear();
#ifdef _HTMLLOG
	g_HtmlLog.LogInfo( _T("후 텍스쳐 [ %d 개 ]"),g_D3D.GetTextureCNT() );
#endif
}

void ResMgr::ReleaseAllUnitAni()
{	
	RES_ITOR itor = m_UnitAniList.begin();
	while( itor != m_UnitAniList.end() )
	{
		RES_ANI* pAni =(RES_ANI*)(itor->second);
//		g_D3D.ReleaseANI( pAni->TexID );
		SAFE_FREE( pAni );
		itor++;
	}
	m_UnitAniList.clear();
}


RES_ANI* ResMgr::LoadUnitAni(const TCHAR* FileName  )
{
	static TCHAR Nametemplate[][64]={
		{_T("Hold.ani") },
		{_T("Move.ani") },
		{_T("OkMark.ani") },
		{_T("Success.ani") },
		{_T("Fail.ani") },

		
	};
	RES_ANI* pRes = (RES_ANI*)m_UnitAniList[FileName];

	if( pRes != NULL ) // 이미 로딩했다면 리턴~ 
	return pRes;
#define MAXMOTION 5

	pRes = ( RES_ANI* ) malloc( sizeof( RES_ANI ) * MAXMOTION ); // 총 15모션 이므로 
	// 아직 로딩한게 아니면 새로 로딩하기 
	TCHAR fName[256]={0,};  
//	_tsplitpath( FileName, NULL,NULL,fName,NULL  ); // 화일명만 분리하기 
	int tmpa=0;
//	int UnitDefine = _ttoi( FileName );
	for( int i = 0; i < MAXMOTION; i++ )
	{
		_stprintf( fName, _T("Data/IMG/CHARACTER/%s/%s"),FileName, Nametemplate[i] );
		int ret = g_D3D.LoadAni( fName, pRes[ i ] );
		if( ret == -1 )
		{
#ifdef _HTMLLOG
			g_HtmlLog.LogError( _T("ResMgr::LoadUnitAni(), %s 없는파일로딩하려함. "), fName );
#endif
			SAFE_FREE( pRes );
			assert( ! _T("Where is *.ani File ???~") );
			return NULL;
		}

	pRes[ i ].CenterX = 0;
	pRes[ i ].CenterY = 0;
					

	}
	
	m_UnitAniList[FileName] = pRes;
	return pRes;
}

//RES_ANI* ResMgr::LoadBlockAni(const TCHAR* fName )
//{
////	static TCHAR _tmptr[] = { 'N', 'B', 'U', 'D', 
////		                      'L', 'R', 'F', 'S' };
//	static TCHAR _tmptr[] = { 'N', 'B' };
//	TCHAR str[256]={0,};
//	RES_ANI* pRes = (RES_ANI*)m_BlockAniList[ fName ];
//	if(  pRes != NULL )
//		return pRes;
//
//	TCHAR tMapName[128]={0,};
//#ifdef _UNICODE
//	MultiByteToWideChar( CP_ACP, 0, g_Map.GetMapFileName(), strlen(g_Map.GetMapFileName()) , tMapName,128 );
//#else
//	strcpy( tMapName, g_Map.GetMapFileName() );
//#endif
//
//	pRes =(RES_ANI*) malloc( sizeof( RES_ANI ) * 2 /*8*/ ); // 총 8모션 이므로
//
//	int tmpa=0;
//	for(int n=0; n < 2/*8*/ ;n++ ) 
//	{
//		_stprintf(str, _T(".\\IMG\\map\\%s\\%s%c.ani"), tMapName, fName, _tmptr[n] );
//		if( g_D3D.LoadAni( str, pRes[ n ] ) == -1 )
//		{
//			memset( &pRes[ n ].sprInfo,0, sizeof( SPRITEINFOHEADER32 ));
//			pRes[ n ].TexID = -1;
//			tmpa++;
//		}
//	
//	pRes[n].CenterX = pRes[n].sprInfo.nWidth >> 1;
//	pRes[n].CenterY = pRes[n].sprInfo.nHeight <= 21 ? 
//					pRes[n].sprInfo.nHeight : pRes[n].sprInfo.nHeight - 21; 
//	// 만일 21 이하면  그냥 맨 바닥으로 설정 아니라면 바닥에서 21 띄워서 설정함
//	}
//
//
//	if( tmpa == 2 ) // 모두 로딩 안된건 날림 
//	{
//		SAFE_FREE(pRes); // 여기서 의심이 갑니당. 말롱으로 배열을 잡아? 후움...
//		m_BlockAniList[fName] = NULL; 
//		return NULL;
//	}
//
//	m_BlockAniList[fName] = pRes; 
//	return pRes; 
//}

RES_ANI* ResMgr::LoadItemAni(const TCHAR* fName )
{
	RES_ANI* pRes = (RES_ANI*)m_ItemAniList[ fName ];
	
	if( pRes != NULL ) // 이미 로딩했다면 리턴~ 
		return pRes;
	
	pRes =(RES_ANI*) malloc( sizeof( RES_ANI )  );
	RES_ANI Res;
	// 아직 로딩한게 아니면 새로 로딩하기 
	
	if( g_D3D.LoadAni( fName, Res ) == -1 )
	{
		assert( !_T("Where is *.ani File ???~") );
#ifdef _HTMLLOG
		g_HtmlLog.LogError( _T("ResMgr::LoadUnitAni(),  %s 없는파일로딩하려함. "), fName );	
#endif
		SAFE_FREE( pRes );
		return NULL;
	}

	pRes->CenterX = pRes->sprInfo.nWidth >> 1;
	pRes->CenterY = pRes->sprInfo.nHeight <= 21 ? 
					pRes->sprInfo.nHeight : pRes->sprInfo.nHeight - 21; 
	*pRes = Res;	
	m_ItemAniList[ fName ] = pRes; 
	return pRes; 
}