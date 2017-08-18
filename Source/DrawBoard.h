#ifndef DRAWBOARD_H_INCLUDED
#define DRAWBOARD_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "Config.h"

class Drawboard
{
    public:
        Drawboard(const Config& config);

        void addQuad        (unsigned x, unsigned y, sf::Color& colour);
        void setQuadColour  (unsigned x, unsigned y, sf::Color& colour);

        void draw(sf::RenderWindow& window);

    private:
        unsigned getQuadIndex(unsigned x, unsigned y) const;
        void makeGrid();

        std::vector<sf::Vertex>     m_grid;
        std::vector<sf::Vertex>     m_pixels;

        const Config CONFIG;
};

#endif // DRAWBOARD_H_INCLUDED
