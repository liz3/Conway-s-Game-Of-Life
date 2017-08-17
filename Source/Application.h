#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>  //uint8_t

#include "Random.h"

enum class Cell : uint8_t
{
    Dead,
    Alive
};

class Application
{
    public:
        Application();

        void run();

    private:
        void handleEvents();
        void updateWorld();

        unsigned getCellIndex   (unsigned x, unsigned y);
        void setQuadColour      (unsigned x, unsigned y, Cell cell);

        template<typename F>
        void cellForEach(F f);

        sf::RenderWindow m_window;

        const unsigned QUAD_SIZE;
        const unsigned WIDTH;
        const unsigned HEIGHT;

        std::vector<sf::Vertex>     m_pixels;
        std::vector<Cell>           m_cells;

        Random m_rand;
};

template<typename F>
void Application::cellForEach(F f)
{
    for (unsigned x = 0; x < WIDTH; x++)
    {
        for (unsigned y = 0; y < HEIGHT; y++)
        {
            f(x, y);
        }
    }
}

#endif // APPLICATION_H_INCLUDED
