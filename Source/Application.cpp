#include "Application.h"

#include <iostream>

namespace
{
    sf::Color deadColour   = {100, 100, 100};
    sf::Color aliveColour  = sf::Color::Black;
}

sf::Color& getCellColour(Cell cell)
{
    return cell == Cell::Alive ?
                    aliveColour :
                    deadColour;
}

Application::Application()
:   m_window    ({1024, 768}, "Conway's Game of Life")
,   QUAD_SIZE   (8)
,   WIDTH       (m_window.getSize().x / QUAD_SIZE)
,   HEIGHT      (m_window.getSize().y / QUAD_SIZE)
,   m_cells     (WIDTH * HEIGHT)
,   m_view      ({0, 0}, {(float)m_window.getSize().x, (float)m_window.getSize().y})
,   m_drawBoard (WIDTH, HEIGHT, QUAD_SIZE)
{
    m_font.loadFromFile         ("font/arial.ttf");
    m_text.setFont              (m_font);
    m_text.setFillColor         (sf::Color::White);
    m_text.setOutlineColor      (sf::Color::Black);
    m_text.setOutlineThickness  (3);
    m_text.setCharacterSize     (15);
    m_text.setPosition          (5, 5);
    m_text.setString            ("Click cell to change it to alive/ dead. \nPress \"Space\" when you are ready.");

    m_window.setFramerateLimit(30);

    cellForEach([&](unsigned x, unsigned y)
    {
        auto& cell = m_cells[getCellIndex(x, y)];
        cell = ((Cell)m_rand.getIntInRange(0, 1));
        m_drawBoard.addQuad(x, y, getCellColour(cell));
    });

    m_view.setCenter(m_window.getSize().x / 2,
                   m_window.getSize().y / 2);
}

void Application::run()
{
    int generations = 0;
    sf::Clock clock;
    while (m_window.isOpen())
    {
        m_window.clear();
        m_window.setView(m_view);

        handleInput(clock.restart().asSeconds());
        //update
        switch (m_state)
        {
            case State::Creating:
                mouseInput();
                break;

            case State::Sim:
                m_text.setString("Generation: " + std::to_string(generations++));
                updateWorld();
                break;
        }

        m_drawBoard.draw(m_window, m_state == State::Sim ? false : true);
        m_window.setView(m_window.getDefaultView());
        m_window.draw(m_text);

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

        m_drawBoard.setQuadColour(x, y, getCellColour(updateCell));
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

void Application::handleInput(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_state = State::Sim;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_view.move(0, -100 * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_view.move(0, 100 * dt);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_view.move(-100 * dt, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_view.move(100 * dt, 0);
    }
    /*
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_view.zoom(1.1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_view.zoom(0.9);
    }*/
}

void Application::mouseInput()
{
    static sf::Clock delay;
    if (delay.getElapsedTime().asSeconds() > 0.2)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            delay.restart();
            auto mousePosition = sf::Mouse::getPosition(m_window);
            auto adjustedMousePosition = m_window.mapCoordsToPixel({(float)mousePosition.x,
                                                                    (float)mousePosition.y});
            auto x = adjustedMousePosition.x;
            auto y = adjustedMousePosition.y;

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
            m_drawBoard.setQuadColour(newX, newY, getCellColour(cell));
        }
    }
}
