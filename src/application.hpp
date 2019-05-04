#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <chrono>
#include "chip8.hpp"
#include "options.hpp"
#include "window.hpp"

using clockz = std::chrono::high_resolution_clock;

class Application {
   public:
    Application(const char *title, int w, int h);
    bool run() const;
    bool load_rom(const char *path);
    void step();
    void render();
    void events();
    void update();

   private:
    bool quit_;
    bool paused_;
    Window window_;
    Chip8 chip8_;
    std::chrono::time_point<clockz> last_step_;
    std::chrono::time_point<clockz> last_timer_;
    std::chrono::time_point<clockz> last_render_;
};

#endif
