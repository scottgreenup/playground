#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace GE {

class Window {
public:
    Window(unsigned int width, unsigned int height);
    ~Window();

    static void ErrorCallback(int error, const char* desc);

    explicit operator bool() const
    {
        return m_constructed && !glfwWindowShouldClose(m_window);
    }

    inline GLFWwindow* window()
    {
        return m_window;
    }

    bool update();

private:
    unsigned int m_width;
    unsigned int m_height;

    GLFWwindow* m_window;

    bool m_constructed;
};

}


#endif
