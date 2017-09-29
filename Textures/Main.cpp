#include"TextureApp.h"
#include <ctime>

int main()
{
	srand(time(NULL));
	Application * c = new TextureApp();
	int appStatus = c->Run(1600, 900, "test", false);
	delete c;
	return appStatus;
}
