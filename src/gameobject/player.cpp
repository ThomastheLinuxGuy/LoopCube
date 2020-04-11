#include "player.hpp"

Player::Player(TextureHandler &textures, SDL_Renderer* renderer, Camera &camera)
    : Game_Object{0xFA0, textures, renderer, camera, 0, 0, 30, 58}, vel_x{0}, vel_y{0}, vel_x_speed{2} {

}

Player::~Player() {

}

bool Player::check_block_collision(Chunk_Group &chunks) {
    std::vector<Chunk> chunkgroup = *chunks.get_chunks();

    // Loop through all chunks
    for (auto &chunk_it: chunkgroup) {

        // Then grab the chunk we want
        std::vector<Block> chunk = *chunk_it.get_chunk();

        // Loop through blocks in the chunk
        for (auto &block: chunk) {
            while (is_colliding(block)) {
                return true;
            }
        }
    }
    return false;
}

void Player::jump(Chunk_Group &chunks) {
    obj.y += 3;
    if (check_block_collision(chunks)) {
        vel_y = -14;
    }
    obj.y -= 3;
}

void Player::direct_player(int direction, Chunk_Group chunks) {
    switch (direction) {
        case 0: // UP
            jump(chunks);
            break;
        case 1: // RIGHT
            vel_x += vel_x_speed;
            break;
        case 2: // DOWN
            break;
        case 3: // LEFT
            vel_x -= vel_x_speed;
            break;
        default:
            std::cerr << "[Warning] Invalid direction" << std::endl;
    }
}

double Player::get_vel_x() const {
    return vel_x;
}

double Player::get_vel_y() const {
    return vel_y;
}

void Player::update(Chunk_Group &chunks) {
    // TODO possibly change engine code, player sometimes gets stuck in a wall
    vel_x *= 0.75;
    obj.x += vel_x;


    if (check_block_collision(chunks)) {
        obj.x += vel_x * -1;
        vel_x = 0;
    }

    vel_y += .7;
    obj.y += vel_y;

    if (check_block_collision(chunks)) {
        obj.y += vel_y * -1;
        vel_y = 0;
    }


    // Update draw position
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x();
    dest.y = get_y();
}
