#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include <iostream>
#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>
#include "texturehandler.hpp"
#include "eventhandler.hpp"
#include "blockids.hpp"
#include "position.hpp"


class Inventory {
public:
    Inventory() = default;
    Inventory(SDL_Renderer* renderer,
        TextureHandler &textures,
        EventHandler &events,
        int* WINDOW_W,
        int* WINDOW_H);
    ~Inventory();

    void draw_hotbar();
    void draw_inventory_menu();
    bool get_inventory_visibility();
    void update();
private:
    unsigned hotbar_slots;
    unsigned max_slots;
    bool visible;
    bool show_inventory_menu;
    int hotbar_pos;
    std::vector<int> get_hovered_pos(int x, int y, int corner_x, int corner_y, bool draw);
    bool collision(int, int, int, int, int, int, int, int);

    std::vector<int> slots;

    int* WINDOW_W;
    int* WINDOW_H;

    SDL_Renderer* renderer;
    TextureHandler* textures;
    EventHandler* events;
};

#endif // INVENTORY_HPP