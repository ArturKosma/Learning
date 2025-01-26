#include "AFApp.h"

int main()
{
	AFApp& app = AFApp::GetInstance();
	app.StartLoop();
} 
