#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>  //uint8_t

#include "Random.h"
#include "DrawBoard.h"

enum class Cell : uint8_t
{
    Dead,
    Alive
};

enum class State  : uint8_t
{
    Sim,
    Creating
};

class Application
{
    public:
        Application();

        void run();

    private:
        void handleEvents       ();
        void updateWorld        ();
        void handleInput        (float dt);
        void mouseInput         ();
        unsigned getCellIndex   (unsigned x, unsigned y);

        template<typename F>
        void cellForEach(F f);

        sf::RenderWindow m_window;

        const unsigned QUAD_SIZE;
        const unsigned WIDTH;
        const unsigned HEIGHT;

        std::vector<Cell>           m_cells;

        Random m_rand;

        State m_state = State::Creating;

        sf::Font m_font;
        sf::Text m_text;

        sf::View m_view;

        Drawboard m_drawBoard;
};

template<typename F>
void Application::cellForEach(F f)
{
    for (unsigned y = 0; y < HEIGHT; y++)
    {
        for (unsigned x = 0; x < WIDTH; x++)
        {
            f(x, y);
        }
    }
}

#endif // APPLICATION_H_INCLUDED
