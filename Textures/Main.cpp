#include"TextureApp.h"

int main()
{
	Application * c = new TextureApp();
	int appStatus = c->Run(1600, 900, "test", false);
	delete c;
	return appStatus;
}
