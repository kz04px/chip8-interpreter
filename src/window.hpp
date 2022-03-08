#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include "chip8.hpp"

class Window {
   public:
    [[nodiscard]] Window(const char *title, int w, int h);
    ~Window();
    void resize(int w, int h);
    void clear();
    void render(const Chip8 &chip8);
    void render_inputs(const Chip8 &chip8);
    void present();
    void toggle_fullscreen();

   private:
    int width_;
    int height_;
    bool fullscreen_;
    SDL_Window *window_;
    SDL_Renderer *renderer_;
};

#endif
