#pragma once

//#include "stdafx.h"

//things needed for flash
#include "flash.h"
#include <atlbase.h>
#include <atlwin.h>

//DirectX9 goodies
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")
#include <dxerr9.h>
#pragma comment(lib,"dxerr9.lib")

#include <windows.h>

//ShatterStar thingz
//#include "wrapperutils.h"	
//#include "shatterstarutils.h"

#define NOTIMPLEMENTED return E_NOTIMPL

//Our own little thread.  This will push the Flash AcitveX Control
//to the least used Core on a multi-core system
DWORD WINAPI LoadSWF (LPVOID filename);

//yes, this needs to be mutexed
typedef enum FlashThreadStatus
{
    eNotStarted = 0x0,
    eRunning,
    eWaitingToStop,
    eFinished,
} FlashThreadStatus;

class FlashContainer : public _IShockwaveFlashEvents
{
public:
	//hidden window that will hold the flash ActiveX Control
	HWND windowless;

	//interfaces to shockwave object
	IShockwaveFlash *iflash;
	IUnknown *unk;
	IViewObject *viewobject;

	//the stream interface to marshal the viewobject into the Rendering Thread
	IStream *pStream;

	//the RenderThread's version of the view object
	IViewObject *RTviewobject;

	//the ready state of the Flash Document
	long state;

	//handle to Flash's thread
	HANDLE hThread;

	//event thinga-ma-bobs
	IConnectionPointContainer *m_lpConCont;
	IConnectionPoint *m_lpConPoint;

	//Event Advise cookie (mmmmmmm cookies)
	DWORD m_dwConPointID;

	//System memory texture
	IDirect3DTexture9 * memtex;

	//Size of ActiveX Container
	int _width,_height;

	//number of frames in the Flash Document
	long totalframes;

	//path to flash file in Unicode
	unsigned short real_name[256];

	//status of the flash thread
	FlashThreadStatus flashThreadStatus;

	FlashContainer(IDirect3DDevice9 * m_pDevice,char * Path, int Width,int Height);
	~FlashContainer(void);

	HRESULT GetFrame(IDirect3DDevice9 * m_pDevice,IDirect3DTexture9 * m_pOutTexture);

	//DShockwaveFlashEvents
    HRESULT STDMETHODCALLTYPE OnReadyStateChange (
        long newState );
    HRESULT STDMETHODCALLTYPE OnProgress (
        long percentDone );
    HRESULT STDMETHODCALLTYPE FSCommand (
        BSTR command,
        BSTR args );
	HRESULT STDMETHODCALLTYPE FlashCall(
		BSTR request);

	//IDispatch proto
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
        /* [out] */ UINT __RPC_FAR *pctinfo);
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

	//IUnknown proto
	int m_iRef;
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
};