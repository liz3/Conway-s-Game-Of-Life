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
:   m_window    ({800, 600}, "Conway's Game of Life")
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
    m_text.setString            ("Click cell to change it to alive/ dead. \nPress \"Space\" when you are ready.");


    m_pixels.reserve(WIDTH *
                     HEIGHT * 4);

    m_window.setFramerateLimit(10);


    auto addQuad = [&](unsigned x, unsigned y)
    {
        sf::Vertex topLeft;
        sf::Vertex topRight;
        sf::Vertex bottomLeft;
        sf::Vertex bottomRight;

        float pixelX = x * QUAD_SIZE;
        float pixelY = y * QUAD_SIZE;

        topLeft     .position = {pixelX,                pixelY};
        topRight    .position = {pixelX + QUAD_SIZE,    pixelY};
        bottomLeft  .position = {pixelX,                pixelY + QUAD_SIZE};
        bottomRight .position = {pixelX + QUAD_SIZE,    pixelY + QUAD_SIZE};

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

    cellForEach([&](unsigned x, unsigned y)
    {
        m_cells[getCellIndex(x, y)] = Cell::Dead;
        addQuad(x, y);
    });
/*
    cellForEach([&](unsigned x, unsigned y)
    {
        addQuad(x, y);
    });
*/
    makeGrid();
}

void Application::run()
{
    while (m_window.isOpen())
    {
        m_window.clear();

        //update
        switch (m_state)
        {
            case State::Creating:
                handleInput();
                break;

            case State::Sim:
                updateWorld();
                break;
        }

        //draw
        switch (m_state)
        {
            case State::Creating:
                m_window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
                m_window.draw(m_grid.data(), m_grid.size(), sf::Lines);
                m_window.draw(m_text);
                break;

            case State::Sim:
                m_window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
                m_window.draw(m_grid.data(), m_grid.size(), sf::Lines);
                break;
        }
        m_window.display();
        handleEvents();
    }
}

void Application::updateWorld()
{
    std::vector<Cell> newCells(WIDTH * HEIGHT);

    cellForEach([&](unsigned x, unsigned y)
    {
        unsigned count = 0;
        for (int nX = -1; nX <= 1; nX++)    //check neighbours
        for (int nY = -1; nY <= 1; nY++)
        {
            int newX = nX + x;
            int newY = nY + y;

            if (newX == -1 || newX == (int)WIDTH ||
                newY == -1 || newY == (int)WIDTH || //out of bounds
                (nX == 0 && nY == 0)) //Cell itself
            {
                continue;
            }
            auto cell = m_cells[getCellIndex(newX, newY)];
            if (cell == Cell::Alive)
                count++;
        }

        auto cell           = m_cells[getCellIndex(x, y)];
        auto& updateCell    = newCells[getCellIndex(x, y)];
        updateCell = cell;
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

void Application::handleInput()
{
    static sf::Clock delay;
    if (delay.getElapsedTime().asSeconds() > 0.2)
    {
        mouseInput();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_state = State::Sim;
    }
}

void Application::mouseInput()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        delay.restart();
        auto mousePosition = sf::Mouse::getPosition(m_window);
        auto x = mousePosition.x;
        auto y = mousePosition.y;

        if (x < 0 || x > (int)m_window.getSize().x ||
            y < 0 || y > (int)m_window.getSize().y)
        {
            return;
        }

        //Convert mouse/ screen coordinates to cell coordinates
        int newX = x / QUAD_SIZE;
        int newY = y / QUAD_SIZE;

        //Switch cell type
        auto& cell = m_cells[getCellIndex(newX, newY)];
        cell =  cell == Cell::Alive ?
                    Cell::Dead :
                    Cell::Alive;

        //Set new colour
        setQuadColour(newX, newY, cell);
    }
}

void Application::makeGrid()
{
    for (unsigned x = 0; x < WIDTH; x++)
    {
        sf::Vertex top;
        sf::Vertex bottom;

        top     .position = {float(x * QUAD_SIZE), 0};
        bottom  .position = {float(x * QUAD_SIZE), float(HEIGHT * QUAD_SIZE)};

        m_grid.push_back(top);
        m_grid.push_back(bottom);
    }

    for (unsigned y = 0; y < WIDTH; y++)
    {
        sf::Vertex left;
        sf::Vertex right;

        left    .position = {0,                         float(y * QUAD_SIZE)};
        right   .position = {float(WIDTH * QUAD_SIZE),  float(y * QUAD_SIZE)};

        m_grid.push_back(left);
        m_grid.push_back(right);
    }
}
