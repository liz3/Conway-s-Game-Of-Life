#ifndef DRAWBOARD_H_INCLUDED
#define DRAWBOARD_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

class Drawboard
{
    public:
        Drawboard(unsigned width, unsigned height, unsigned quadSize);

        void addQuad        (unsigned x, unsigned y, sf::Color& colour);
        void setQuadColour  (unsigned x, unsigned y, sf::Color& colour);

        void draw(sf::RenderWindow& window, bool drawGrid);

    private:
        unsigned getQuadIndex(unsigned x, unsigned y);
        void makeGrid();

        std::vector<sf::Vertex>     m_grid;
        std::vector<sf::Vertex>     m_pixels;

        const unsigned WIDTH, HEIGHT, QUAD_SIZE;
};

#endif // DRAWBOARD_H_INCLUDED
