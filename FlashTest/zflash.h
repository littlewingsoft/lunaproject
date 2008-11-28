//code by zZz/KCN aka Anatoliy Samara. feb2003
#include <stdlib.h>
#include <string>

namespace ZFLASH{

int Init(int version_needed);
void Exit();
unsigned long* LoadMovie(const std::wstring& movie_name, int w,int h);

//void* CreateBuffer(int width,int height);
int DrawFrame();

void Loop( bool bLoop );
bool Loop();

int GetWidth();
int GetHeight();


}//namespace