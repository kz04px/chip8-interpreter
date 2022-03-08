#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <chrono>
#include "chip8.hpp"
#include "options.hpp"
#include "window.hpp"

using clockz = std::chrono::high_resolution_clock;

class Application {
   public:
    [[nodiscard]] Application(const char *title, const int w, const int h);

    [[nodiscard]] bool run() const;

    bool load_rom(const char *path);

    void step();

    void render();

    void events();

    void update();

   private:
    Window window_;
    Chip8 chip8_;
    std::chrono::time_point<clockz> last_step_;
    std::chrono::time_point<clockz> last_timer_;
    std::chrono::time_point<clockz> last_render_;
    bool quit_ = false;
    bool paused_ = false;
};

#endif
