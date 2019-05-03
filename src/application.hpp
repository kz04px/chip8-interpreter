#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "chip8.hpp"
#include "options.hpp"
#include "window.hpp"

class Application {
   public:
    Application(const char *title, int w, int h);
    bool run() const;
    bool load_rom(const char *path);
    void step();
    void render();
    void events();
    void delay();

   private:
    bool quit_;
    bool paused_;
    Window window_;
    Chip8 chip8_;
};

#endif
