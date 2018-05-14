
#ifndef _GAME_H_
#define _GAME_H_

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "Window.h"

namespace GE {

class Game {
public:
    Game();
    ~Game();

    bool update();
    void render();

private:
    Window m_window;
    Keyboard m_keyboard;
};

}

#endif
