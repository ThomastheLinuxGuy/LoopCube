# -*- mode: makefile -*-
CXX ?= g++
CXXFLAGS = -Wall -Wextra -pipe -pedantic $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)
SRC_DIR = ./src
OBJ_DIR = ./obj
TARGET = loopcube
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,obj/%.o,$(SRC))

ifeq ($(strip $(DATA_LOCATION)),)
CXXFLAGS += -DDATA_LOCATION=\".\"
else
CXXFLAGS += -DDATA_LOCATION=\"$(DATA_LOCATION)\"
endif

CXXFLAGS += -DINPUT_BACKEND_SDL2 -DGRAPHIC_BACKEND_SDL2 -D_SDL2

all: debug

obj/%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $^ $(LDFLAGS) 


debug: CXXFLAGS += -g
debug: setup
debug: $(TARGET)

release: CXXFLAGS += -O2 -ffast-math
release: setup
release: $(TARGET)

install:
	@echo "Installing... Make sure your binary has DATA_LOCATION set to a loopcube folder"
	mkdir -p $(DATA_LOCATION)
	cp -r data $(DATA_LOCATION)/data
	install -Dm755 $(TARGET) $(PREFIX)/$(TARGET)

uninstall:
	@echo "Uninstalling... type yes if prompted (be careful)"
	rm -Ir $(DATA_LOCATION)
	rm -ir $(PREFIX)/$(TARGET)

setup:
	@mkdir -p obj/src/

.PHONY: all clean setup debug release install
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET)
