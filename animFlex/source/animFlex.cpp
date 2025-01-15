#include "AFWindow.h"
#include <chrono>

int main()
{
    AFWindow& window = AFWindow::GetInstance();
    while(!window.ShouldShutdown())
    {
        window.Tick();
    }
} 
