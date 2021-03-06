#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>

#include <SDL2/SDL.h>

#include "chunkgroup.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "block.hpp"
#include "gameobj.hpp"
#include "collisioninfo.hpp"

class Entity: public GameObject {
public:
	Entity() = default;
	Entity(int texture_id,
		   double x, double y, double width, double height);
	virtual ~Entity();

	virtual void update(ChunkGroup& chunks);

	double get_vel_x() const;
	double get_vel_y() const;

	// Collision functions
	virtual void collision_left();
	virtual void collision_right();
	virtual void collision_bottom();
	virtual void collision_top();
protected:
	void update_basic_physics(ChunkGroup& chunks);
	CollisionInfo check_block_collision(ChunkGroup &chunks);

	// Physics
	double vel_x;
	double vel_y;
	double vel_x_speed;
	bool on_ground;
	int last_pos;
};

#endif
