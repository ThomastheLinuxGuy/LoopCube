// Only include if needed
#ifdef GRAPHIC_BACKEND_SDL2

#ifndef GRAPHICSWRAPPER_SDL2_HPP
#define GRAPHICSWRAPPER_SDL2_HPP
#include <iostream>
#include <array>
#include <stdexcept>
#include <cstring>
#include <functional>

#include "backendincludes.hpp"

#include "texture.hpp"
#include "texture_sdl2.hpp"
#include "graphicswrapper_internal.hpp"
#include "texturehandler.hpp"
#include "rect.hpp"
#include "color.hpp"

class GraphicsWrapper_SDL2: public GraphicsWrapper {
public:
	GraphicsWrapper_SDL2(Config config);
	~GraphicsWrapper_SDL2();

    void init_screen() override;
	void clear_screen() override;
	void update_screen() override;
	void render(Rect&, Rect&, int) override;
	void render_rect(Rect&, Color) override;
	void set_opacity(int opacity, int texture) override;
    void fetch_screen_size();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TextureHandler<Texture_SDL2*, std::function<Texture_SDL2*(std::string)>>* textures;
	Config config;
};


#endif // GRAPHICSWRAPPER_SDL2_HPP

#endif // GRAPHIC_BACKEND_SDL2
