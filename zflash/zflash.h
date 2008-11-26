//code by zZz/KCN aka Anatoliy Samara. feb2003
#include <stdlib.h>

namespace ZFLASH{

int Init(int version_needed);
void Exit();
void LoadMovie(char *movie_name);

void* CreateBuffer(int width,int height);
int DrawFrame();


}//namespace