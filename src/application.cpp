#include "application.hpp"
#include <cassert>
#include "options.hpp"

Application::Application(const char *title, int w, int h)
    : quit_(false), paused_(false), window_(title, w, h), chip8_() {
}

bool Application::run() const {
    return !quit_;
}

bool Application::load_rom(const char *path) {
    assert(path);
    return chip8_.load(path);
}

void Application::step() {
    const std::uint8_t *keystate = SDL_GetKeyboardState(NULL);

    if (keystate) {
        chip8_.set_key(Input::Key_X, keystate[SDL_SCANCODE_X]);
        chip8_.set_key(Input::Key_1, keystate[SDL_SCANCODE_1]);
        chip8_.set_key(Input::Key_2, keystate[SDL_SCANCODE_2]);
        chip8_.set_key(Input::Key_3, keystate[SDL_SCANCODE_3]);
        chip8_.set_key(Input::Key_Q, keystate[SDL_SCANCODE_Q]);
        chip8_.set_key(Input::Key_W, keystate[SDL_SCANCODE_W]);
        chip8_.set_key(Input::Key_E, keystate[SDL_SCANCODE_E]);
        chip8_.set_key(Input::Key_A, keystate[SDL_SCANCODE_A]);
        chip8_.set_key(Input::Key_S, keystate[SDL_SCANCODE_S]);
        chip8_.set_key(Input::Key_D, keystate[SDL_SCANCODE_D]);
        chip8_.set_key(Input::Key_Z, keystate[SDL_SCANCODE_Z]);
        chip8_.set_key(Input::Key_C, keystate[SDL_SCANCODE_C]);
        chip8_.set_key(Input::Key_4, keystate[SDL_SCANCODE_4]);
        chip8_.set_key(Input::Key_R, keystate[SDL_SCANCODE_R]);
        chip8_.set_key(Input::Key_F, keystate[SDL_SCANCODE_F]);
        chip8_.set_key(Input::Key_V, keystate[SDL_SCANCODE_V]);
    }

    if (!paused_) {
        for (int i = 0; i < 9; ++i) {
            chip8_.step();
        }
    }
}

void Application::delay() {
    SDL_Delay(17);
}

void Application::events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit_ = true;
                break;
            case SDL_KEYDOWN:
                // Ignore all repeats
                if (event.key.repeat) {
                    break;
                }

                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit_ = true;
                        break;
                    case SDLK_SPACE:
                        paused_ = !paused_;
                        break;
                    case SDLK_F1:
                        options::debug = !options::debug;
                        break;
                    case SDLK_F2:
                        window_.toggle_fullscreen();
                        break;
                    case SDLK_F3:
                        options::borders = !options::borders;
                        break;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        window_.resize(event.window.data1, event.window.data2);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void Application::render() {
    window_.clear();
    window_.render(chip8_);

    if (paused_) {
        // TODO:
        // Draw "Paused" message
    }

    if (options::debug) {
        // Show pressed keys
        window_.render_inputs(chip8_);
    }

    window_.present();
}
