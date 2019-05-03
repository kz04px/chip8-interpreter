#include <iostream>
#include "application.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "No path to ROM specified" << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init() error: " << SDL_GetError() << std::endl;
        return 2;
    }

    try {
        Application app("Chip8", 1024, 512);

        // Load ROM
        bool r = app.load_rom(argv[1]);
        if (!r) {
            std::cerr << "Failed to load ROM " << argv[1] << std::endl;
            return 2;
        }

        while (app.run()) {
            app.events();
            app.step();
            app.render();
            app.delay();
        }
    } catch (const std::bad_alloc &ex) {
        std::cerr << ex.what() << std::endl;
    }

    SDL_Quit();

    return 0;
}
