#include "window.hpp"
#include <cassert>
#include <stdexcept>
#include "options.hpp"

Window::Window(const char *title, int w, int h)
    : width_(w), height_(h), fullscreen_(false) {
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

void Window::resize(int w, int h) {
    assert(renderer_);

    width_ = w;
    height_ = h;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = width_;
    rect.h = height_;
    SDL_RenderSetViewport(renderer_, &rect);
    SDL_RenderSetClipRect(renderer_, &rect);
}

void Window::clear() {
    assert(window_);
    assert(renderer_);

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

void Window::render(const Chip8 &chip8) {
    assert(window_);
    assert(renderer_);

    const int pixel_width = width_ / 64;
    const int pixel_height = height_ / 32;

    // Draw game
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            const bool pixel = chip8.pixel(x, y);

            if (pixel == true) {
                if (options::borders) {
                    SDL_Rect rect;
                    rect.x = pixel_width * x + 1;
                    rect.y = pixel_height * y + 1;
                    rect.w = pixel_width - 2;
                    rect.h = pixel_height - 2;
                    SDL_RenderFillRect(renderer_, &rect);
                } else {
                    SDL_Rect rect;
                    rect.x = pixel_width * x;
                    rect.y = pixel_height * y;
                    rect.w = pixel_width;
                    rect.h = pixel_height;
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
    const int order[16] = {0x1, 0x2, 0x3, 0xC,
                           0x4, 0x5, 0x6, 0xD,
                           0x7, 0x8, 0x9, 0xE,
                           0xA, 0x0, 0xB, 0xF};
    // clang-format on

    for (int i = 0; i < 16; ++i) {
        const int xpos = i % 4;
        const int ypos = i / 4;

        if (chip8.get_key(static_cast<Input>(order[i]))) {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 100);
        } else {
            SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 100);
        }

        SDL_Rect rect;
        rect.x = width * xpos;
        rect.y = height * ypos;
        rect.w = width - 2;
        rect.h = height - 2;
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
