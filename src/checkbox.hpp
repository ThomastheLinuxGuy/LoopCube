#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP
#include <iostream>

#include "backendincludes.hpp"

#include "rect.hpp"
#include "color.hpp"
#include "uielement.hpp"
#include "text.hpp"
#include "aabb.hpp"
#include "constants.hpp"

class Checkbox: public UiElement {
public:
	Checkbox() = default;
	Checkbox(int id, std::string text, int x, int y, int size, bool checked=false);
	~Checkbox();

	void on_change(void (*function)(int, int));
	int get_id();
	void get_value(bool& here);
	bool is_changed();
	bool toggle();
	bool get_checked();
	void check();
	void uncheck();

	void set_x(int x);
	void set_y(int y);
	void update(EventWrapper*& events, int offset_x = 0, int offset_y = 0);
	void render(GraphicsWrapper* renderer, int offset_x = 0, int offset_y = 0);
	
private:
	bool changed;
	int id;
	std::string text;
	int x;
	int y;
	int size;
	bool checked;

	Text text_render;

    Rect dest;
};

#endif
