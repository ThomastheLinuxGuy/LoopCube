#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include "chunkgroup.hpp"
#include "chunk.hpp"
#include "block.hpp"
#include "gameobj.hpp"

class Player: public Game_Object {
public:
    Player() = default;
    Player(TextureHandler &textures);
    ~Player();

    void update(Chunk_Group &chunks, Camera& camera);
    bool check_block_collision(Chunk_Group &chunks);

    // Movement
    void jump(Chunk_Group &chunks);
    void direct_player(int direction, Chunk_Group &chunks);

    double get_vel_x() const;
    double get_vel_y() const;
private:
    double vel_x;
    double vel_y;
    double vel_x_speed;
    int last_pos;

	// For jumping
	bool on_ground;
	bool jumping;
	bool can_jump;
	bool jump_enabled;
};

#endif // PLAYER_HPP
