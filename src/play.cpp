#include "play.hpp"

Play::Play(GraphicsWrapper* renderer, EventWrapper*& events)
	: window_w{renderer->screen_size()[0]}, window_h{renderer->screen_size()[1]}, show_particles{false}, chunks{0}, 
	  camera{&window_w, &window_h}, player{}, entities{}, fade{60}, particles{}, time{8600, 28500, 8600, 22000, 1700, 1700},
	  background{} {
	this->renderer = renderer;
	this->events = events;

	camera.set_pos(0, 0);

	inv = std::unique_ptr<Inventory>(new Inventory(renderer, events));
	update_config();
}

Play::~Play() {}

void Play::update() {
	// Update screen size for camera
	window_w = renderer->screen_size()[0];
	window_h = renderer->screen_size()[1];
	
	mouse_events();
	
	// Update all chunks
	chunks.update_all_viewport(camera);
	chunks.check_area(player.get_default_x(), structures);
	
	inv->update();

	// Update all entities
	for (Entity*& entity: entities) {
		if (!entity->out_of_view(camera)) {
			entity->update(chunks);
		}
	}
	
	// Update player
	player.update(chunks, entities);

	for (int i = 0; i < 4; ++i) {
		if (events->get_key_state()[i]) {
			player.direct_player(i, chunks);
		}
	}

	// Create entity
	if (events->get_key_state()[16] || events->get_button_state()[9]) {
		entities.push_back(new TestEntity(player.get_default_x(), player.get_default_y()-30));
	}

	// Jump (A)
	if (events->get_button_state()[4]) {
		player.direct_player(0, chunks);
	}

	// Down
	if (events->get_button_state()[0]) {
		player.direct_player(2, chunks);
	}

	// Right
	if (events->get_button_state()[1]) {
		player.direct_player(1, chunks);
	}

	// Left
	if (events->get_button_state()[2]) {
		player.direct_player(3, chunks);
	}

	// Up
	if (events->get_button_state()[3]) {
		player.direct_player(0, chunks);
	}


	// Update animation(s)
	fade.tick();
	
	// Structures Queue Handling
	for (size_t k = 0; k < structures.size(); ++k) {
		auto& data = structures[k]->get_data();
		
		// Make sure we delete old structures to prevent memleaks
		if (data.size() == 0) {
			delete structures[k];
			structures.erase(structures.begin() + k);
			continue;
		}
		
		for (size_t i = 0; i < data.size(); ++i) {
			// See if the chunk actually exists
			auto c1 = chunks.get_chunk_at((structures[k]->get_x()+data[i].x)*constants::block_w, false);
			
			if (c1 != nullptr) {
				int chunk_pos = std::abs(((structures[k]->get_x()+data[i].x))-(c1->get_slot()*constants::chunk_width));

				// Place block in proper chunk
				c1->place_block(data[i].id, chunk_pos, structures[k]->get_y() + data[i].y);
				
				// Erase the data now that we are done using it
				data.erase(data.begin() + i);
			}
		}
	}
	
	// Particles
	dead_particles();

	if (show_particles) {
		for (auto& particle: particles) {
			particle.update(chunks);
		}
	}

	handle_camera();

	// Update background (after we handle camera or things get a tiny bit off sync)
	background.update(camera, time);

	time.tick();
}

void Play::update_config() {
	show_particles = constants::config.get_int(CONFIG_SHOW_PARTICLES);
}

void Play::render() {
	// Render background elements
	background.render(renderer);
	
	if (show_particles) {
		for (auto& particle: particles) {
			particle.render(renderer, camera);
		}
	}
	
	chunks.render_all_viewport(renderer, camera);
	
	player.render(renderer, camera);

	for (Entity*& entity: entities) {
		entity->render(renderer, camera);
	}

	background.render_light(renderer, camera);

	if (!inv->get_inventory_visibility()) draw_selection(nullptr, nullptr);

	inv->draw_hotbar();

	inv->draw_inventory_menu();
}

void Play::mouse_events() {
	int p1, p2;
	if (!inv->get_inventory_visibility()) draw_selection(&p1, &p2);

	
	// Get cursor over chunk
	Chunk* chunk = chunks.get_chunk_at(p1*constants::block_w, true);
	if (chunk != nullptr) {
		// Do some math to get the chunk position
		int chunk_pos = std::abs(p1-(chunk->get_slot()*constants::chunk_width));

		switch(events->get_vmouse_down()) {
		case 1:
			{
				const BlockInfo* block = chunk->destroy_block(chunk_pos, p2, inv.get());

				// Check if block found
				if (block != nullptr) {
					// Generate particles
					if (show_particles) {
						GravityParticle temp{block->get_texture_id(),50, rand() % 2 == 1 ? -2.0 : 2.0, -3.0,
						    p1*constants::block_w+(constants::block_w/2), p2*constants::block_h, 8, 6};
						particles.push_back(temp);
					}
				}
			}
			break;
		case 3:
			{
				Item& item = inv->get_selected_item();
				if (item.enabled) {
					BlockInfo b_info = item.get_block();
					if (chunk->place_block(b_info.get_id(), chunk_pos, p2)) {
						item.add_count(-1);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

// Draw a selection box and set p1 and p2 to the position
// Perhaps we should rename this to get_selection and seperate rendering functions?
void Play::draw_selection(int* p1, int* p2) {
	int b_w = static_cast<int>(constants::block_w);
	int b_h = static_cast<int>(constants::block_h);

	auto mpos = events->get_vmouse_pos();

	const int sel_x = floor((mpos[0] - camera.get_x()) / b_w) * b_w;
	const int sel_y = floor((mpos[1] - camera.get_y()) / b_h) * b_h;

    Rect selection{sel_x + static_cast<int>(camera.get_x()), sel_y + static_cast<int>(camera.get_y()), b_w, b_h};

	int fade_amount = std::abs(std::sin(static_cast<double>(fade.get_frame())/20))*30+50;

	renderer->render_rect(selection, Color{255, 255, 255, fade_amount});

	if (p1 != nullptr) *p1 = sel_x/b_w;
	if (p2 != nullptr) *p2 = sel_y/b_h;
}

// Sets camera to player position
void Play::handle_camera() {
	double x = std::floor(player.get_default_x()) * -1 + (window_w/2) - player.get_width()/2;
	double y = std::floor(player.get_default_y()) * -1 + (window_h/2) - player.get_height()/2;
	
	static double move_x = x;
	static double move_y = y;

	float amount = 0.25;
	move_x += (x - move_x)*amount;
	move_y += (y - move_y)*amount;
	
	camera.set_pos(move_x, move_y);
}

void Play::dead_particles() {
	for (size_t i = 0; i < particles.size(); ++i) {
		if (particles[i].is_dead()) particles.erase(particles.begin() + i);
	}
}

// TODO I left this here because I wanted to do something with it...
// Don't uncomment right now, this code is bad, horribly ugly, and VERY slow
/*
  void Play::draw_debug_menu() {
  SDL_version compiled;
  SDL_GetVersion(&compiled);

  std::vector<std::string> dbg_text = {
  "[[ DEBUG ]]",
  // Player
  "Built: At "+ std::string(__TIME__)+" "+std::string(__DATE__)+" C++"+std::to_string(__cplusplus)
  +" SDL"+std::to_string(compiled.major)+"."+ std::to_string(compiled.minor)+"."+ std::to_string(compiled.patch)
  #ifdef __WIIU__
  +" For Wii U",
  #else
  +" for PC",
  #endif
  "= Player =",
  "X: " + std::to_string(player.get_default_x() / constants::block_w)
  + " / Y: " + std::to_string(player.get_default_y() / constants::block_h),
  // Chunk
  "= Chunk =",
  "Pos: " + std::to_string(chunks.get_id()),
  "Size: " + std::to_string(chunks.get_chunks()->size())

		
  };
  SDL_Color color;
  // For some reason it must be setup like this...
  color.r = 255; color.g = 255; color.b = 255;
  int size = 14;
  for (size_t i = 0; i < dbg_text.size(); ++i) {
  Text dtext{renderer, dbg_text[i], color, };
  dtext.draw(0,i*size);
  }
  }
*/
