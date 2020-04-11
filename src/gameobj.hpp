#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "texturehandler.hpp"
#include "constants/blockids.hpp"
#include "position.hpp"
#include "SDL2/SDL.h"
#include <string>
#include "camera.hpp"

class Game_Object {

public:
    Game_Object();
    Game_Object(int texture_id, TextureHandler &textures, SDL_Renderer* renderer, Camera &camera, double x, double y, double w, double h);
    virtual ~Game_Object();

    virtual void update();
    virtual void render();

    virtual bool out_of_view();

    // Return position
    virtual double get_default_x() const;
    virtual double get_default_y() const;
    virtual double get_x() const;
    virtual double get_y() const;
    virtual double get_width() const;
    virtual double get_height() const;

    virtual const Position* get_obj() const;

    bool is_colliding(const Game_Object &obj2);
protected:
    int texture_id;

    Position obj;
    SDL_Renderer* renderer;
    SDL_Rect src, dest;
    TextureHandler* textures;
    Camera* camera;

};


#endif // GAMEOBJ_HPP
