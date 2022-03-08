#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include "chip8.hpp"

class Window {
   public:
    [[nodiscard]] Window(const char *title, const int w, const int h);

    ~Window();

    void resize(const int w, const int h);

    void clear();

    void render(const Chip8 &chip8);

    void render_inputs(const Chip8 &chip8);

    void present();

    void toggle_fullscreen();

   private:
    int width_ = 1024;
    int height_ = 512;
    bool fullscreen_ = false;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
};

#endif
