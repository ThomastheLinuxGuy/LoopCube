// Only include if enabled
#ifdef INPUT_BACKEND_SDL2

#ifndef EVENTWRAPPER_SDL2_HPP
#define EVENTWRAPPER_SDL2_HPP
#include <iostream>
#include <vector>
#include <array>

#include "backendincludes.hpp"

#include "eventwrapper_internal.hpp"

class EventWrapper_SDL2: public EventWrapper {
public:
	EventWrapper_SDL2();
	~EventWrapper_SDL2();

	virtual void init() override;
	virtual void update_controllers() override;

	virtual void listen() override;
private:
	SDL_Joystick* controller;
	SDL_Event event;
};

#endif // EVENTWRAPPER_SDL2_HPP
#endif // INPUT_BACKEND_SDL2
