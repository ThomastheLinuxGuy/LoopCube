#include "inventory.hpp"

Inventory::Inventory(GraphicsWrapper* renderer,
					 EventWrapper* events) : animation{false}, hotbar_slots{10}, max_slots{hotbar_slots*5}, visible{true}, show_inventory_menu{false},
											 hotbar_pos{0}, item_held{} {
	this->renderer = renderer;
	this->events = events;
	
	items.resize(max_slots, Item{});

	inventory_slide = Transition{static_cast<double>(renderer->screen_size()[1]), 0.30};
	hotbar_slide = Transition{0, 0.30};
}

Inventory::~Inventory() {}

Item& Inventory::get_selected_item() {
	return items[hotbar_pos];
}

void Inventory::add_item(int id) {
	constexpr int max_count = 99;

	auto found = std::find_if(items.begin(), items.end(), [&](Item item) {
		if (!item.enabled) return false; 
		return item.get_block().get_id() == id ? item.get_count() < max_count : false;
	});

	if (found != items.end()) {
		found->add_count();
	} else {
		for (Item& i: items) {
			if (!i.enabled) {
				i = Item{id};
				break;
			}
		}
	}
}

void Inventory::update() {
	// hotbar_slot keys
	for (int i = 5; i < 15; ++i) {
		if (events->get_key_state()[i]) {
			hotbar_pos = i-5;
		}
	}

	if (events->get_key_state()[4] || events->get_button_state()[8]) {
		show_inventory_menu = !show_inventory_menu;
		animation = show_inventory_menu;
	}

	inventory_slide.update();
	hotbar_slide.update();
}

void Inventory::draw_inventory_menu() {
	auto win = renderer->screen_size();


	// Sliding Animation
	if (animation) {
		inventory_slide.value = 0;
	} else {
	    inventory_slide.value = renderer->screen_size()[1];
	}

	int slide = inventory_slide.get();

	constexpr int offset = 1;
	if (slide < renderer->screen_size()[1] - offset) {
		int scale = 3; // TODO Fix with hovered position

		constexpr int menu_width = 170;
		constexpr int menu_height = 90;
		
		const int MAX_X = (win[0] - (menu_width*scale))/2;
		const int MAX_Y = (win[1] - (menu_height*scale))/2;

		// First draw BG shadow
		Rect shadow{0, static_cast<int>(slide), win[0], win[1]};
		renderer->render_rect(shadow, Color{0, 0, 0, 180});

		// Render inventory menu
		Rect src{0, 0, menu_width, menu_height};
		Rect dest{MAX_X, MAX_Y+static_cast<int>(slide), src.w*scale, src.h*scale};
		renderer->render(src, dest, 11);

		std::vector<int> pos = get_hovered_pos(events->get_vmouse_pos()[0], events->get_vmouse_pos()[1], MAX_X, MAX_Y+slide, true);
		
		if (events->get_vmouse_clicked() == 1 && pos[0] != -1 && pos[1] != -1) {
			auto& it = items[pos[0]+(pos[1]*hotbar_slots)];
			if (!item_held.enabled) {
				// move item; null out inventory slot
				item_held = it;
				it = Item{};
			} else {
				// Swap items
				auto tmp = it;
				it = item_held;
				item_held = tmp;
			}
		}
		
		auto mouse_pos = events->get_vmouse_pos();
		if (item_held.enabled) item_held.draw(renderer, mouse_pos[0]-17, mouse_pos[1]-17, 35, 35);
	}
}

// returns std::vector<int>{-1, -1} if nothing is being hovered
std::vector<int> Inventory::get_hovered_pos(int x, int y, int corner_x, int corner_y, bool draw = false) {
	constexpr int tile_size = 42;

	// Start gap
	constexpr int sx_gap = 18;
	constexpr int sy_gap = 18;

	// Splitter gap
	constexpr int sp_gap = 6;
	int total = max_slots/10;

	std::vector<int> returner;
	
	// Loop through slots
	for (int i = 0; i < hotbar_slots; ++i) {
		for (int j = 0; j < total; ++j) {
			// Check collision

			int new_x = corner_x+(i*(tile_size+sp_gap)+sx_gap);
			int new_y = corner_y+(j*(tile_size+sp_gap)+sy_gap);

			// Draw item
			if (items[i+(j*hotbar_slots)].enabled) {
				items[i+(j*hotbar_slots)].draw(renderer, new_x+3, new_y+3, tile_size-7, tile_size-7);
			}

			if (collision(x, y, 1, 1,
						  new_x, new_y, tile_size, tile_size)) {

				if (draw) {
				    Rect rect{new_x, new_y, tile_size, tile_size};
					renderer->render_rect(rect, Color{255, 255, 255, 40});
				}

				returner = {i, j};
			}
		}
	}

	if (returner.size() == 2) {
		return returner;
	}

	return std::vector<int>{-1, -1};
}

bool Inventory::collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	Rect r1{x1, y1, w1, h1}, r2{x2, y2, w2, h2};

	if (r1.x < r2.x + r2.w &&
		r1.x + r1.w > r2.x &&
		r1.y < r2.y + r2.h &&
		r1.y + r1.h > r2.y) {
		return true;
	}
	return false;
}

bool Inventory::get_inventory_visibility() {
	return show_inventory_menu;
}

void Inventory::draw_hotbar() {
	constexpr int BLOCK_S = 40;
    const int MAX_X = (renderer->screen_size()[0] - ((hotbar_slots+1)*BLOCK_S+3))/2;
	constexpr int offset = -1;
	
	// Animation
	if (animation) {
		hotbar_slide.value = (BLOCK_S*-1) + offset;
	} else {
		hotbar_slide.value = 0;
	}

	int slide = hotbar_slide.get();
	
	for (int i = 0; i < hotbar_slots; ++i) {
	    Rect src{0, 0, 16, 16};
		if (hotbar_pos == i) {
			src.y = 16;
		}
	    Rect block{i*(BLOCK_S+3)+MAX_X, slide, BLOCK_S, BLOCK_S};
		renderer->render(src, block, 10);

		if (items[i].enabled) {
			items[i].draw(renderer, block.x+5, slide+block.y+5, 30, 30);
		}
	}
}
