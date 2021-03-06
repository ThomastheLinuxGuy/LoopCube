#include "box.hpp"

Box::Box(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h},
									   src{0, 0, 40, 40} {
	update_pair();
}

Box::~Box() {}

void Box::update_pair() {
	pos = std::array<std::pair<int, Rect>, 9>{
		std::make_pair(22, Rect{x, y, src.w, src.h}), // Top left
		std::make_pair(23, Rect{x+src.w, y, w-(src.w*2), src.h}), // Top middle
		std::make_pair(24, Rect{x+w-src.w, y, src.w, src.h}), // Top right
		std::make_pair(25, Rect{x, y+src.h, src.w, h-(src.h*2)}), // Middle left
		std::make_pair(26, Rect{x+src.w, y+src.h, w-(src.w*2), h-(src.h*2)}), // Center
		std::make_pair(27, Rect{x+w-src.w, y+src.h, src.w, h-(src.h*2)}), // Middle right
		std::make_pair(28, Rect{x, y+h-src.h, src.w, src.h}), // Bottom left
		std::make_pair(29, Rect{x+src.w, y+h-src.h, w-(src.w*2), src.h}), // Bottom middle
		std::make_pair(30, Rect{x+w-src.w, y+h-src.h, src.w, src.h}) // Bottom right
	};
}

void Box::set_x(int x) {
	this->x = x;
}
void Box::set_y(int y) {
	this->y = y;
}
void Box::set_width(int w) {
	this->w = w;
}
void Box::set_height(int h) {
	this->h = h;
}

void Box::render(GraphicsWrapper* renderer, int offset_x, int offset_y) {
	for (auto& p: pos) {
		Rect mod = p.second;
		mod.x += offset_x;
		mod.y += offset_y;
		renderer->render(src, mod, p.first);
	}
}
