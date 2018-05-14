
#include "Game.h"

#include <iostream>

using namespace GE;

Game::Game()
: m_window(1920, 1080)
, m_keyboard(m_window)
{

}

Game::~Game()
{

}

bool Game::update()
{
    if (!m_window || !m_window.update()) {
        return false;
    }

    if (m_keyboard.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return false;
    }

    return true;
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(m_window.window());
}


