#ifndef MENU_HPP
#define MENU_HPP
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <cmath>

#include "../texture/texturehandler.hpp"
#include "../event/eventhandler.hpp"
#include "../constants/blockids.hpp"
#include "../ui/button.hpp"
#include "../math/animation.hpp"

class Menu {
public:
    Menu(SDL_Renderer* renderer, 
        TextureHandler &textures,
        EventHandler &events,
        int* WINDOW_W,
        int* WINDOW_H,
        std::vector<std::string> option_str);
    ~Menu();

    void update();
    void render();
    void render_background();
    int get_pressed();
private:
    std::vector<std::string> option_str;
    std::vector<int> option_state; 
    Button button;
    std::vector<Button> button_group;
    const int BLOCK_S;
    const int BUTTON_W;
    int* WINDOW_W;
    int* WINDOW_H;

    SDL_Renderer* renderer;
    TextureHandler* textures;
    EventHandler* events;
    Animation shift;
};


#endif // MENU_HPP