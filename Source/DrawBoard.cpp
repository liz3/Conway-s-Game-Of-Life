#include "DrawBoard.h"

Drawboard::Drawboard(unsigned width, unsigned height, unsigned quadSize)
:   WIDTH       (width)
,   HEIGHT      (height)
,   QUAD_SIZE   (quadSize)
{
    m_pixels.reserve(WIDTH * HEIGHT * 4);
    makeGrid();
}

void Drawboard::addQuad(unsigned x, unsigned y, sf::Color& colour)
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

    topLeft     .color = colour;
    topRight    .color = colour;
    bottomLeft  .color = colour;
    bottomRight .color = colour;

    m_pixels.push_back(topLeft);
    m_pixels.push_back(bottomLeft);
    m_pixels.push_back(bottomRight);
    m_pixels.push_back(topRight);
}

void Drawboard::setQuadColour(unsigned x, unsigned y, sf::Color& colour)
{
    auto index = getQuadIndex(x, y);

    for (int i = 0; i < 4; i++)
    {
        m_pixels[index + i].color = colour;
    }
}

void Drawboard::draw(sf::RenderWindow& window, bool drawGrid)
{
    window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
    if (drawGrid)
    {
        window.draw(m_grid.data(), m_grid.size(), sf::Lines);
    }
}


void Drawboard::makeGrid()
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

unsigned Drawboard::getQuadIndex(unsigned x, unsigned y)
{
    return (y * WIDTH + x) * 4;
}

