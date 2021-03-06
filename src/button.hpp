#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <iostream>
#include <string>
#include <memory>

#include "backendincludes.hpp"

#include "text_full.hpp"
#include "rect.hpp"
#include "color.hpp"
#include "uielement.hpp"
#include "constants.hpp"
#include "text.hpp"
#include "aabb.hpp"

class Button: public UiElement {
public:
	Button() = default;
	Button(int id, int x, int y, int width, int height = 32);
	~Button();

	void set_x(int x);
	void set_y(int y);
	void update(EventWrapper*& events, int offset_x = 0, int offset_y = 0);
	void render(GraphicsWrapper* renderer, int offset_x = 0, int offset_y = 0);

	// Text
	void set_text(std::string text);
	std::string get_text();
	bool get_pressed();
	int get_id();
private:	
	std::string text;
	unsigned int id;
	int x;
	int y;
	int width;
	int height;

	// Mouse state
	bool hovered;
	bool being_clicked;
	bool clicked;

	Rect src, dest;
	Text* button_text;

};

#endif // BUTTON_HPP
