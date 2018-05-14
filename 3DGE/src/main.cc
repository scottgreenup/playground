
#include <iostream>

//#include "Entity.h"
//#include "Components/Renderer.h"

#include "Game.h"

using namespace GE;

int main()
{
    //Entity entity;
    //entity.addComponent(new Renderer());

    Game game;

    while (game.update()) {
        game.render();
    }

    //std::cout << entity.hasComponent<Renderer>() << std::endl;

    std::cout << "Exiting." << std::endl;
    return 0;
}
