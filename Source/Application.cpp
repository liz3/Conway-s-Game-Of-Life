#include "Application.h"

#include <iostream>

namespace
{
    sf::Color deadColour   = {100, 100, 100};
    sf::Color aliveColour  = sf::Color::Black;
}

sf::Color getCellColour(Cell cell)
{
    return cell == Cell::Alive ?
                    aliveColour :
                    deadColour;
}

Application::Application()
:   m_window    ({1280, 720}, "Conway's Game of Life")
,   QUAD_SIZE   (8)
,   WIDTH       (m_window.getSize().x / QUAD_SIZE)
,   HEIGHT      (m_window.getSize().y / QUAD_SIZE)
,   m_cells     (WIDTH * HEIGHT)
{
    m_font.loadFromFile         ("font/arial.ttf");
    m_text.setFont              (m_font);
    m_text.setFillColor         (sf::Color::White);
    m_text.setOutlineColor      (sf::Color::Black);
    m_text.setOutlineThickness  (3);
    m_text.setCharacterSize     (15);
    m_text.setPosition          (5, 5);
    m_text.setString            ("Click cell to change it to alive/ dead. \n\
                                Press \"Space\" when you are ready.");


    m_pixels.reserve(WIDTH *
                     HEIGHT * 4);

    m_window.setFramerateLimit(30);


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

        auto cell = m_cells[getCellIndex(x, y)];
        auto colour = getCellColour(cell);


        topLeft     .color = colour;
        topRight    .color = colour;
        bottomLeft  .color = colour;
        bottomRight .color = colour;

        m_pixels.push_back(topLeft);
        m_pixels.push_back(bottomLeft);
        m_pixels.push_back(bottomRight);
        m_pixels.push_back(topRight);
    };

    cellForEach([&](int x, int y)
    {
        m_cells[getCellIndex(x, y)] = Cell::Dead;
    });

    cellForEach([&](int x, int y)
    {
        addQuad(x, y);
    });
}

void Application::run()
{
    while (m_window.isOpen())
    {
        m_window.clear();

        switch (m_state)
        {
            case State::Creating:
                handleCreateInput();
                break;

            case State::Simulating:
                updateWorld();
                break;
        }

        m_window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
        m_window.draw(m_text);
        m_window.display();
        handleEvents();
    }
}

void Application::updateWorld()
{
    std::vector<Cell> newCells(WIDTH * HEIGHT);

    cellForEach([&](int x, int y)
    {
        unsigned count = 0;
        for (int nX = -1; nX <= 1; nX++)    //check neighbours
        for (int nY = -1; nY <= 1; nY++)
        {
            int newX = nX + x;
            int newY = nY + y;

            if (newX == -1 || newX == (int)WIDTH ||
                newY == -1 || newY == (int)WIDTH)
            {
                continue;   //out of bounds
            }
            auto cell = m_cells[getCellIndex(newX, newY)];
            if (cell == Cell::Alive)
                count++;
        }

        auto cell           = m_cells[getCellIndex(x, y)];
        auto& updateCell    = newCells[getCellIndex(x, y)];
        switch (cell)
        {
            case Cell::Alive:
                if (count < 2 || count > 3)
                {
                    updateCell = Cell::Dead;
                }
                break;

            case Cell::Dead:
                if (count == 3)
                {
                    updateCell = Cell::Alive;
                }
                break;
        }

        setQuadColour(x, y, updateCell);
    });
    m_cells = std::move(newCells);
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

void Application::setQuadColour(unsigned x, unsigned y, Cell cell)
{
    auto index =  getCellIndex(x, y) * 4;
    auto colour = getCellColour(cell);

    m_pixels[index    ].color = colour;
    m_pixels[index + 1].color = colour;
    m_pixels[index + 2].color = colour;
    m_pixels[index + 3].color = colour;
}

void Application::handleCreateInput()
{
    static sf::Clock delay;
    if (delay.getElapsedTime().asSeconds() > 0.1)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            delay.restart();
            auto mousePosition = sf::Mouse::getPosition(m_window);

            auto newX = mousePosition.x / QUAD_SIZE;
            auto newY = mousePosition.y / QUAD_SIZE;

            auto& cell = m_cells[getCellIndex(newX, newY)];

            cell =  cell == Cell::Alive ?
                        Cell::Dead :
                        Cell::Alive;

            setQuadColour(newX, newY, cell);
        }
    }
}




