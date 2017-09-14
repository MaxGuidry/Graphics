#include "LightingApp.h"

int main()
{
	Application * c = new LightingApp();
	int appStatus = c->Run(1600, 900, "test", false);
	delete c;
	return appStatus;
}
