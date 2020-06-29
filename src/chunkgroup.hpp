#ifndef CHUNKGROUP_HPP
#define CHUNKGROUP_HPP
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "chunk.hpp"
#include "blockids.hpp"
#include "texturehandler.hpp"

// TODO non-important Rename this properly
class Chunk_Group {
public:
    Chunk_Group() = default;
    Chunk_Group(unsigned long int seed, SDL_Renderer* renderer, Camera &camera, TextureHandler &textures);
    ~Chunk_Group();

    void generate_chunk(int id);
    void check_area(int x, int y);
    void render_all();
    void update_all();

    int get_id(int surrounding = 8);

    Chunk* get_chunk_at(int x);

    std::vector<Chunk>* get_chunks();
    bool chunk_already_generated(int id);
private:
    void sort_all();
    std::vector<Chunk> group; // Chunks the game works with in place
    std::vector<int> loaded_chunks; // group ids
    std::vector<Chunk> group_past; // Unloaded chunks, gets moved into group when loaded again
    std::vector<int> past_chunks; // group_past ids
    unsigned long seed;

    SDL_Renderer* renderer;
    Camera* camera;
    TextureHandler* textures;
};


#endif // CHUNKGROUP_HPP
