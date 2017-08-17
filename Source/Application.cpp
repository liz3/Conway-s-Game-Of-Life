#include "Application.h"

#include <ctime>    //time(int)
#include <cstdlib>  //rand, srand (TEMP)
#include <cstdint>  //uint8_t
#include <iostream>



Application::Application()
:   m_window    ({1280, 720}, "Conway's Game of Life")
,   QUAD_SIZE   (8)
,   WIDTH       (m_window.getSize().x / QUAD_SIZE)
,   HEIGHT      (m_window.getSize().y / QUAD_SIZE)
,   m_cells     (WIDTH * HEIGHT)
{

    m_pixels.reserve(WIDTH *
                     HEIGHT * 4);

    m_window.setFramerateLimit(60);
    std::srand(std::time(nullptr)); //temp for testing purposes

    auto addQuad = [&](int x, int y)
    {
        sf::Vertex topLeft;
        sf::Vertex topRight;
        sf::Vertex bottomLeft;
        sf::Vertex bottomRight;

        float pixelX = x * QUAD_SIZE;
        float pixelY = y * QUAD_SIZE;

        topLeft     .position = {pixelX,            pixelY};
        topRight    .position = {pixelX + WIDTH,    pixelY};
        bottomLeft  .position = {pixelX,            pixelY + WIDTH};
        bottomRight .position = {pixelX + WIDTH,    pixelY + WIDTH};

        //temp for testing
        uint8_t c = std::rand() % 255;
        topLeft     .color = {c, c, c};
        topRight    .color = {c, c, c};
        bottomLeft  .color = {c, c, c};
        bottomRight .color = {c, c, c};

        m_pixels.push_back(topLeft);
        m_pixels.push_back(bottomLeft);
        m_pixels.push_back(bottomRight);
        m_pixels.push_back(topRight);
    };

    for (unsigned x = 0; x < m_window.getSize().x / QUAD_SIZE; ++x)
    for (unsigned y = 0; y < m_window.getSize().y / QUAD_SIZE; ++y)
    {
        addQuad(x, y);
    }

    cellForEach([&](int x, int y)
    {
        auto& cell = m_cells[getCellIndex(x, y)];
    });

}

void Application::run()
{
    while (m_window.isOpen())
    {
        m_window.clear();

        m_window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
        m_window.display();
        handleEvents();
    }
}

void Application::updateWorld()
{
    std::vector<Cell> newCells(WIDTH * HEIGHT);


}


void Application::handleEvents()
{
    sf::Event e;

    while (m_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

unsigned Application::getCellIndex(unsigned x, unsigned y)
{
    return y * WIDTH + x;
}






























