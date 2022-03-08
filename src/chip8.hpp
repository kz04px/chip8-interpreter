#ifndef CHIP8_HPP_INCLUDED
#define CHIP8_HPP_INCLUDED

#include <cstdint>

enum Input
{
    Key_X = 0x0,
    Key_1 = 0x1,
    Key_2 = 0x2,
    Key_3 = 0x3,
    Key_Q = 0x4,
    Key_W = 0x5,
    Key_E = 0x6,
    Key_A = 0x7,
    Key_S = 0x8,
    Key_D = 0x9,
    Key_Z = 0xA,
    Key_C = 0xB,
    Key_4 = 0xC,
    Key_R = 0xD,
    Key_F = 0xE,
    Key_V = 0xF,
};

class Chip8 {
   public:
    [[nodiscard]] Chip8();
    bool load(const char *path);
    void step();
    [[nodiscard]] bool pixel(const int x, const int y) const;
    void set_key(const Input a, const bool s);
    [[nodiscard]] bool get_key(const Input a) const;
    void timers();

   private:
    // RAM
    std::uint8_t ram_[4096];
    // Registers
    std::uint8_t *v_;
    std::uint16_t i_;
    std::uint16_t pc_;
    std::uint16_t sp_;
    // Timers
    std::uint8_t dt_;
    std::uint8_t st_;
    // Keys
    bool keys_[16];
};

#endif
