
#include "Window.h"

#include <iostream>

using namespace GE;

Window::Window(unsigned int width, unsigned int height)
: m_width(width)
, m_height(height)
, m_window(nullptr)
, m_constructed(false)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    m_window = glfwCreateWindow(
        m_width,
        m_height,
        "window_name",
        glfwGetPrimaryMonitor(),
        NULL
    );

    if (!m_window) {
        // TODO change this into googlelog, fail hard
        std::cerr << "Failed to initialise the GLFW window." << std::endl;
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glfwSetErrorCallback(Window::ErrorCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::cout << glGetString(GL_VERSION) << std::endl;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialise GLEW." << std::endl;
        return;
    }

    glClearColor(0.8, 0.1, 0.1, 0.1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LESS);

    m_constructed = true;
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

bool Window::update()
{
    glfwPollEvents();
    return true;
}

void Window::ErrorCallback(int error, const char* desc)
{
    std::cerr << "error occured (" << error << "): " << desc << std::endl;
}

