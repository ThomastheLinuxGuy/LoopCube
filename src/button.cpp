#include "button.hpp"

Button::Button(int id,
			   TextureHandler& textures,
			   int x,
			   int y, 
			   int width, 
			   int height) 
    : textures{textures}, id{static_cast<unsigned int>(id)}, width{width}, height{height}, hovered{false}, button_text{nullptr} {
    this->x = x;
    this->y = y;
}

Button::~Button() {}

void Button::update(EventHandler& events, int x_offset, int y_offset) {
	src.h = 16;
    src.w = 16;
    src.x = 0;

    // Change sprite to hovered version if hovered is true
    src.y = (static_cast<int>(hovered))*16;

    dest.h = height;
    dest.w = width-(32*2);
    dest.x = x+32;
    dest.y = y;

    if (clicked == true) {
        clicked = false;
		being_clicked = false;
    }

    int calc_width = dest.w + (src.w*2)*2;
	std::array<int, 2> mouse_pos = events.get_mouse_pos();
    if (AABB(mouse_pos[0]+x_offset, mouse_pos[1]+y_offset, 1, 1, x, y, calc_width, dest.h)) {
        hovered = true;
        if (events.get_mouse_down() == 1) {
            dest.y += 5;
            being_clicked = true;
        } else {
            if (being_clicked == true) {
                clicked = true;
                being_clicked = false;
            }
        }

    } else {
        hovered = false;
        being_clicked = false;
        clicked = false;
    }
}

bool Button::get_pressed() {
    return clicked;
}

int Button::get_id() {
    return id;
}

void Button::render(SDL_Renderer* renderer, int x_offset, int y_offset) {
    SDL_Rect begin{x+x_offset, dest.y+y_offset, src.w*2, dest.h},
		end{dest.x+x_offset+dest.w, dest.y, src.w*2, dest.h};
    SDL_RenderCopy(renderer, textures.get_texture(6), &src, &begin);
    SDL_RenderCopy(renderer, textures.get_texture(5), &src, &dest);
    SDL_RenderCopy(renderer, textures.get_texture(7), &src, &end);
    if (button_text != nullptr) {
        button_text->draw(dest.x+x_offset, dest.y+y_offset);
    }
}

void Button::set_text(SDL_Renderer* renderer, std::string text) {
    this->text = text;
    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    
    button_text = std::shared_ptr<Text>(new Text(renderer, this->text, color, constants::button_font));
}

void Button::set_x(int x) {
    this->x = x;
}

void Button::set_y(int y) {
    this->y = y;
}

std::string Button::get_text() {
    return text;
}
