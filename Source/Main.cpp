#include <iostream>

#include "Application.h"
#include "Config.h"

int main()
{
    Config config;

    std::cout << "Enter window width: ";
    std::cin >> config.windowWidth;

    std::cout << "Enter window height: ";
    std::cin >> config.windowHeight;

    std::cout << "Enter cell size: ";
    std::cin >> config.quadSize;

    //Make it so that the cells fit in the window
    config.windowWidth -= config.windowWidth   % config.quadSize;
    config.windowHeight -= config.windowHeight % config.quadSize;

    config.simWidth  =  config.windowWidth  / config.quadSize;
    config.simHeight =  config.windowHeight / config.quadSize;

    Application app(config);
    app.run();
}
