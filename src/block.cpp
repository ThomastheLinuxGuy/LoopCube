#include "block.hpp"

Block::Block() {}

Block::Block(int id, TextureHandler &textures, SDL_Renderer* renderer, Camera &camera, int x, int y)
    : Game_Object{id, textures, renderer, camera, (double)x, (double)y, block_w, block_h} {
    this->id = id;
}

double Block::get_x() const {
    return obj.x*obj.w + (camera->get_x());
}

double Block::get_y() const {
    return obj.y*obj.h + (camera->get_y());

}

void Block::update() {
    src.h = obj.h;
    src.w = obj.w;
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x();
    dest.y = get_y();
}
