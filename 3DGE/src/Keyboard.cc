#include "Keyboard.h"
#include "Window.h"

using namespace GE;

Keyboard::Keyboard(Window& window)
: m_window(window)
{
}

Keyboard::~Keyboard()
{
}

int Keyboard::getKey(int key)
{
    return glfwGetKey(m_window.window(), key);
}
