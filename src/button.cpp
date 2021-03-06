#include "button.hpp"

Button::Button(int id,
			   int x,
			   int y, 
			   int width, 
			   int height) 
	: id{static_cast<unsigned int>(id)}, width{width}, height{height}, hovered{false}, button_text{nullptr} {
	this->x = x;
	this->y = y;
}

Button::~Button() {}

void Button::update(EventWrapper*& events, int offset_x, int offset_y) {
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
	std::array<int, 2> mouse_pos = events->get_vmouse_pos();
	if (AABB(mouse_pos[0], mouse_pos[1], 1, 1, offset_x+x, offset_y+y, calc_width, dest.h)) {
		hovered = true;
		if (events->get_vmouse_down() == 1) {
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

void Button::render(GraphicsWrapper* renderer, int offset_x, int offset_y) {
	Rect begin{offset_x+x, offset_y+dest.y, src.w*2, dest.h},
		end{offset_x+dest.x+dest.w, offset_y+dest.y, src.w*2, dest.h};
	Rect mod_dest{offset_x+dest.x, offset_y+dest.y, dest.w, dest.h};
	renderer->render(src, begin, 6);
	renderer->render(src, mod_dest, 5);
	renderer->render(src, end, 7);

	if (button_text != nullptr)
		button_text->render(renderer, x+(width/2)-(button_text->get_width()/2)-2,
							mod_dest.y+(height/2)-(button_text->get_height()/2)-2, 20);
}

void Button::set_text(std::string text) {
	this->text = text;

    delete button_text;
	button_text = GENERATE_TEXT(text);
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
