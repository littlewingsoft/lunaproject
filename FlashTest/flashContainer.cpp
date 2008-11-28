#include "flashContainer.h"

FlashContainer::FlashContainer(IDirect3DDevice9 * m_pDevice,char * Path,int Width,int Height)
{	
	HRESULT hr = S_OK;

	_width = Width;
	_height = Height;

	//create the system memory texture to hold the pixel buffer
	hr = m_pDevice->CreateTexture(Width,					// width
									Height,					// height
									1,						// levels
									D3DUSAGE_DYNAMIC,		// usage
									D3DFMT_X8R8G8B8,        // format
									D3DPOOL_SYSTEMMEM, 
									&memtex,
									NULL);

	//convert Path to WideChar	
	MultiByteToWideChar(CP_ACP,0,Path,-1,(LPWSTR)real_name,256);

	//change threading model
	DWORD dwID;
	hThread = CreateThread(NULL,0,LoadSWF,(LPVOID)this,0,&dwID);
}

DWORD WINAPI LoadSWF (LPVOID data) throw()
{
	FlashContainer * cont = (FlashContainer *)data;

	HRESULT hr = S_OK;

	//create Window and Attach Flash control
	cont->windowless = CreateWindowEx(0,CAxWindow::GetWndClassName(),0,WS_POPUP,0,0,cont->_width,cont->_height,0,0,0,0);
	hr = CoCreateInstance(__uuidof(ShockwaveFlash),0,CLSCTX_ALL, __uuidof(IShockwaveFlash), (void **)&cont->iflash);

	//set up event notifications
	hr = cont->iflash->QueryInterface(IID_IConnectionPointContainer, (void**)&cont->m_lpConCont);
	if (FAILED(hr))
		return -1;
	hr = cont->m_lpConCont->FindConnectionPoint(DIID__IShockwaveFlashEvents, &cont->m_lpConPoint);
	if (FAILED(hr))
		return -1;
	hr = cont->m_lpConPoint->Advise((_IShockwaveFlashEvents *)cont, &cont->m_dwConPointID);
	if (FAILED(hr))
		return -1;

	//We went through all this trouble, make the little prick give us the Alpha Channel
	hr = cont->iflash->put_WMode(L"transparent");
	hr = AtlAxAttachControl(cont->iflash,cont->windowless,0);

	//get the view object
	hr = cont->iflash->QueryInterface(__uuidof(IViewObject),(void **)&cont->viewobject);

	//create stream to Marshal view object into render thread
	cont->pStream = NULL;
	hr = CoMarshalInterThreadInterfaceInStream(__uuidof(IViewObject), cont->viewobject, &cont->pStream);

	//sanity check
	cont->RTviewobject = NULL;

	//we want it to always loop
	hr = cont->iflash->put_Loop(true);

	//load the movie
	hr = cont->iflash->put_Movie((BSTR)cont->real_name);

	//since these are always going to be local media,  force wait until ready state is loaded
	for(cont->state = -1; (!hr) && (cont->state != 4);)
	{
		hr = cont->iflash->get_ReadyState(&cont->state);
		if(cont->state == 4)
		{
			//get the total frames of the SWF file
			hr = cont->iflash->get_TotalFrames(&cont->totalframes);
			break;
		}
		Sleep(0); //snooze
	}

	hr = cont->iflash->Play();
	cont->flashThreadStatus = eRunning;

	//our own little message loop.  The ActiveX container needs this to run outside of the main proc thread.
	MSG msg;
	while (GetMessage (&msg, 0, 0, 0))
	{	
		//TO-Do mutex this!
		if(eWaitingToStop == cont->flashThreadStatus)
		{
			break;
		}
		else
		{
			DispatchMessage (&msg);
		}
	}

	cont->flashThreadStatus = eFinished;
	return 0;
}

FlashContainer::~FlashContainer(void)
{
	//kill the memtexture
	SAFE_RELEASE(memtex);

	//release the viewobject and it's evil twin RTviewobject
	SAFE_RELEASE(RTviewobject);
	SAFE_RELEASE(viewobject);

	//unadvise the event sink
	m_lpConPoint->Unadvise(m_dwConPointID);

	//stop the flash thread
	flashThreadStatus = eWaitingToStop;
	while(flashThreadStatus == eWaitingToStop)
	{
		Sleep(10);
	}
	CloseHandle(hThread);
	SAFE_RELEASE(iflash);
}

//Draw the contents of the SFW hiden window onto a Render Target in the GPU (m_pOutTexture)
HRESULT FlashContainer::GetFrame(IDirect3DDevice9 * m_pDevice,IDirect3DTexture9 * m_pOutTexture)
{
	HRESULT hr = S_OK;

	//Don't bother if it's not fully loaded
	if(state != 4) return hr;

	//do we need to un-marshall our version of the view object?
	if(!RTviewobject)
	{
		CoGetInterfaceAndReleaseStream (pStream, __uuidof(IViewObject), (void**) &RTviewobject);
	}

	RECT rc={0,0,0,0};
	rc.right = _width;
	rc.bottom = _height;
	
	VARIANT_BOOL isplay;
	long fn = -1;

	iflash->IsPlaying(&isplay);
	iflash->CurrentFrame(&fn);

	//clear out the memtexture
	D3DLOCKED_RECT d3dlr;
	hr = memtex->LockRect(0,&d3dlr,0,D3DLOCK_DONOTWAIT);
	_int32 * buffer = (_int32 *)d3dlr.pBits;
	DWORD buffersize = _width * _height;
	for(int i = 1; i < buffersize; i++)
	{
		buffer[i] = (_int32)0x00000000;
	}
	memtex->UnlockRect(0);

	//draw frame onto surface of memtexture
	IDirect3DSurface9 * memsurface = NULL;
	memtex->GetSurfaceLevel(0,&memsurface);
	
	//Draw contents of hidden SWF window directly onto surface of in-memory texture
	HDC hdcCompatible;
	hr = memsurface->GetDC(&hdcCompatible);
	//Sanity check
	SetMapMode(hdcCompatible,MM_TEXT);

	if(!hr)
	{
		//draw the frame
		try
		{			
			hr = OleDraw(RTviewobject,DVASPECT_CONTENT,hdcCompatible,&rc);
		}
		catch(HRESULT hr1)
		{
			hr = hr1;
		}
		memsurface->ReleaseDC(hdcCompatible);
	}
	SAFE_RELEASE(memsurface);

	//upload mem-texture to render target on GPU
	memtex->AddDirtyRect(NULL);
	m_pDevice->UpdateTexture(memtex,m_pOutTexture);
	return hr;
}

//implement these if we want event feedback from Flash Scripting
//DShockwaveFlashEvents
HRESULT STDMETHODCALLTYPE FlashContainer::OnReadyStateChange(long newState)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE FlashContainer::OnProgress(long percentDone)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE FlashContainer::FSCommand(BSTR command, BSTR args)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE FlashContainer::FlashCall(BSTR request)
{
	return S_OK;
}

//IDispatch Impl
HRESULT STDMETHODCALLTYPE FlashContainer::GetTypeInfoCount(UINT __RPC_FAR *pctinfo)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE FlashContainer::GetTypeInfo( 
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE FlashContainer::GetIDsOfNames( 
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
	/* [in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	NOTIMPLEMENTED;
}
HRESULT STDMETHODCALLTYPE FlashContainer::Invoke( 
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
	/* [out] */ VARIANT __RPC_FAR *pVarResult,
	/* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
	/* [out] */ UINT __RPC_FAR *puArgErr)
{
	return S_OK;
}

//IUnknown Impl
HRESULT STDMETHODCALLTYPE FlashContainer::QueryInterface(REFIID riid, void ** ppvObject)
{
	if (IsEqualGUID(riid, DIID__IShockwaveFlashEvents))
		*ppvObject = (void*)dynamic_cast<_IShockwaveFlashEvents *>(this);
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
	if (!(*ppvObject))
		return E_NOINTERFACE; //if dynamic_cast returned 0
	m_iRef++;
	return S_OK;
}

ULONG STDMETHODCALLTYPE FlashContainer::AddRef()
{
	m_iRef++;
	return m_iRef;
}

ULONG STDMETHODCALLTYPE FlashContainer::Release()
{
	m_iRef--;
	return m_iRef;
}