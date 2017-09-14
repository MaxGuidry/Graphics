#include "RenderingApp.h"

int main()
{
	Application * c = new RenderingApp();
	int appStatus = c->Run(1600, 900, "test", false);
	delete c;
	return appStatus;
}
