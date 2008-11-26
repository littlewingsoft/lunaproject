//code by zZz/KCN aka Anatoliy Samara. feb2003

#include "zflash.h"
#include "zflash_flash.h"

#include <stdio.h>

#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>

extern void __Error(char *message,const char *file=__FILE__,int line=__LINE__);
IShockwaveFlash *iflash=NULL;
IUnknown *unk=NULL;
CAxWindow cw;
IViewObjectEx *viewobject;

int bm_width=0,bm_height=0;
HDC hdcCompatible = NULL;
HBITMAP hBitmap = NULL;
void *frame_buffer;
RECTL rectl;

namespace ZFLASH{

int Init(int version_needed){
	int player_ok=1;
	IShockwaveFlash *try_flash;
	OleInitialize(NULL);
	CoCreateInstance(CLSID_ShockwaveFlash,NULL,CLSCTX_INPROC_SERVER,IID_IShockwaveFlash,(void **)&try_flash);
	if (!try_flash){
		player_ok=0;
	}
	else{
		long version;
		try_flash->FlashVersion(&version);
		if (version<version_needed)
			player_ok=0;
		try_flash->Release();
	}

	if (!player_ok){
		char temp[1024];
		sprintf(temp,"Flash Player v%d.%02d is not registered on this system!\nFlash Player is required to run this programm.\n\nRegister it now?",version_needed>>16,version_needed&0xffff);
		int res=MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);
		if (res==IDYES){
			HMODULE hlib=LoadLibrary("swflash.ocx");
			if (!hlib)
				__Error("File swflash.ocx not found");
			void (__stdcall *DllRegisterServer)(void);
			DllRegisterServer=(void (__stdcall *)(void))GetProcAddress(hlib,"DllRegisterServer");
			DllRegisterServer();
		}
		else{
			return 0;
		}
	}
	return 1;
}

void ReleaseInterfaces(){
	if (unk) {
		unk->Release();
		unk=NULL;
	}
	if (iflash){
		iflash->Release();
		iflash=NULL;
	}
}

void ReleaseGDI(){
	if (hBitmap){
		DeleteObject(hBitmap);
	}

	if (hdcCompatible){
		DeleteDC(hdcCompatible);
	}
}

void Exit(){
	ReleaseInterfaces();
	ReleaseGDI();
}
			
void* CreateBuffer(int width,int height){
	ReleaseGDI();

	HDC desktop_dc=GetDC(GetDesktopWindow());
	hdcCompatible = CreateCompatibleDC(desktop_dc);	

	BITMAPINFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.bmiHeader.biSize=sizeof(binfo);
	binfo.bmiHeader.biPlanes=1;
	binfo.bmiHeader.biBitCount=32;
	binfo.bmiHeader.biCompression=BI_RGB;
	binfo.bmiHeader.biHeight=height;
	binfo.bmiHeader.biWidth=width;
	
	hBitmap=CreateDIBSection(hdcCompatible,&binfo,DIB_RGB_COLORS,&frame_buffer,NULL,0);
	SelectObject(hdcCompatible, hBitmap);
	bm_width=width;
	bm_height=height;

	RECT rc={0,0,width,height};
	AdjustWindowRect(&rc,cw.GetWindowLong(GWL_STYLE),FALSE);
	cw.SetWindowPos(NULL,0,0,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOREDRAW|SWP_DEFERERASE|
	                                      SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING|
										  SWP_NOZORDER);
	rectl.left=0;
	rectl.right=bm_width;
	rectl.top=0;
	rectl.bottom=bm_height;

	return frame_buffer;
}

void LoadMovie(char *movie_name){
	
	ReleaseInterfaces();
	ReleaseGDI();

	AtlAxWinInit();
	if (cw.IsWindow())
		cw.DestroyWindow();

	RECT rc={0,0,400,400};
	AdjustWindowRect(&rc,0,FALSE);
	cw.Create(NULL,&rc,flash_clsid_c,0);

	cw.QueryControl(&unk);
	if (!unk) __Error("NULL IUnknown");

	HRESULT hr=unk->QueryInterface(IID_IShockwaveFlash,(void **)&iflash);
	if (!iflash) __Error("Unable to query IFlash");

	char buf[256];
	if (movie_name[0]=='.'){
		GetCurrentDirectory(256,buf);
		char last_ch=buf[strlen(buf)-1];
		if (last_ch!='\\'&&last_ch!='/'){
			strcat(buf,"/");
		}
		strcat(buf,movie_name);
	}
	else{
		strcpy(buf,movie_name);
	}

	LPWSTR real_name;//unsigned short
	MultiByteToWideChar( CP_ACP,0,buf,-1, real_name,256);
	
	//iflash->put_Playing(FALSE);
	iflash->LoadMovie(0,real_name);
	//iflash->put_Quality(5);

	iflash->QueryInterface(IID_IViewObjectEx,(void **)&viewobject);

	//iflash->SetZoomRect(0,0,256,256);

}

int DrawFrame(){
	static long lastframe=-1;
	long frame;
	iflash->CurrentFrame(&frame);
	
	if (frame!=lastframe){
		lastframe=frame;
		viewobject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hdcCompatible, &rectl, &rectl, NULL, NULL); 
		return 1;
	}
	return 0;
}

}//namespace