#include "AFWindow.h"

int main()
{
    AFWindow& window = AFWindow::GetInstance();
	window.StartLoop();
} 
