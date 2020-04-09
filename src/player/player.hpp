#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include "../chunkgroup.hpp"
#include "../chunk.hpp"
#include "../block.hpp"
#include "../gameobj.hpp"

class Player: public Game_Object {
public:
    Player() = default;
    Player(TextureHandler &textures, SDL_Renderer* renderer, Camera &camera);
    ~Player();

    void update(Chunk_Group &chunks);
    bool check_block_collision(Chunk_Group &chunks);

    // Movement
    void jump(Chunk_Group &chunks);
    void direct_player(int direction, Chunk_Group chunks);
private:
    double vel_x;
    double vel_y;
    double vel_x_speed;
};

#endif // PLAYER_HPP
