#include "window.hpp"
#include <cassert>
#include <stdexcept>
#include "options.hpp"

Window::Window(const char *title, const int w, const int h) : width_(w), height_(h) {
    assert(title);

    window_ = SDL_CreateWindow(title,
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width_,
                               height_,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window_) {
        throw std::bad_alloc();
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        throw std::bad_alloc();
    }

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

Window::~Window() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void Window::resize(const int w, const int h) {
    assert(renderer_);

    width_ = w;
    height_ = h;

    const auto rect = SDL_Rect(0, 0, width_, height_);
    SDL_RenderSetViewport(renderer_, &rect);
    SDL_RenderSetClipRect(renderer_, &rect);
}

void Window::clear() {
    assert(window_);
    assert(renderer_);

    SDL_SetRenderDrawColor(renderer_, 50, 50, 50, 255);
    SDL_RenderClear(renderer_);
}

void Window::render(const Chip8 &chip8) {
    assert(window_);
    assert(renderer_);

    const int pixel_width = width_ / 64;
    const int pixel_height = height_ / 32;

    // Draw game
    SDL_SetRenderDrawColor(renderer_, 0, 191, 255, 255);
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            const bool pixel = chip8.pixel(x, y);

            if (pixel) {
                if (options::borders) {
                    const auto rect =
                        SDL_Rect(pixel_width * x + 1, pixel_height * y + 1, pixel_width - 2, pixel_height - 2);
                    SDL_RenderFillRect(renderer_, &rect);
                } else {
                    const auto rect = SDL_Rect(pixel_width * x, pixel_height * y, pixel_width, pixel_height);
                    SDL_RenderFillRect(renderer_, &rect);
                }
            }
        }
    }
}

void Window::render_inputs(const Chip8 &chip8) {
    assert(window_);
    assert(renderer_);

    const int width = 16;
    const int height = 16;
    // clang-format off
    const Input order[16] = {Input::Key_1, Input::Key_2, Input::Key_3, Input::Key_4,
                             Input::Key_Q, Input::Key_W, Input::Key_E, Input::Key_R,
                             Input::Key_A, Input::Key_S, Input::Key_D, Input::Key_F,
                             Input::Key_Z, Input::Key_X, Input::Key_C, Input::Key_V};
    // clang-format on

    for (int i = 0; i < 16; ++i) {
        const int xpos = i % 4;
        const int ypos = i / 4;

        if (chip8.get_key(order[i])) {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 100);
        } else {
            SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 100);
        }

        const auto rect = SDL_Rect(width * xpos, height * ypos, width - 2, height - 2);
        SDL_RenderFillRect(renderer_, &rect);
    }
}

void Window::present() {
    SDL_RenderPresent(renderer_);
}

void Window::toggle_fullscreen() {
    fullscreen_ = !fullscreen_;

    if (fullscreen_) {
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(window_, 0);
    }
}
