#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GE {

class Window;

class Keyboard {
public:
    Keyboard(Window& window);
    ~Keyboard();

    int getKey(int key);
private:

    Window& m_window;
};

}

#endif
