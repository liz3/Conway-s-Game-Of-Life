#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>  //uint8_t

#include "DrawBoard.h"
#include "Config.h"

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
        Application(const Config& config);

        void run();

    private:
        void handleEvents       ();
        void updateWorld        ();
        void mouseInput         ();
        unsigned getCellIndex   (unsigned x, unsigned y) const;

        template<typename F>
        void cellForEach(F f);

        const Config CONFIG;

        Drawboard m_drawBoard;
        State m_state = State::Creating;

        sf::RenderWindow m_window;
        sf::View m_view;
        sf::Font m_font;
        sf::Text m_text;

        std::vector<Cell> m_cells;
};

template<typename F>
void Application::cellForEach(F f)
{
    for (unsigned y = 0; y < CONFIG.simHeight; y++)
    {
        for (unsigned x = 0; x < CONFIG.simWidth; x++)
        {
            f(x, y);
        }
    }
}

#endif // APPLICATION_H_INCLUDED
