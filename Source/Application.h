#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

enum class Cell
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

        unsigned getCellIndex(unsigned x, unsigned y);

        template<typename F>
        void cellForEach(F f);

        sf::RenderWindow m_window;

        const unsigned QUAD_SIZE;
        const unsigned WIDTH;
        const unsigned HEIGHT;

        std::vector<sf::Vertex>     m_pixels;
        std::vector<Cell>           m_cells;
};

template<typename F>
void Application::cellForEach(F f)
{
    for (unsigned x = 0; x < WIDTH; x++)
    {
        for (unsigned y = 0; y < WIDTH; y++)
        {
            f(x, y);
        }
    }
}

#endif // APPLICATION_H_INCLUDED
