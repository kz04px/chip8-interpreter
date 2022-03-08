#ifndef CHIP8_HPP_INCLUDED
#define CHIP8_HPP_INCLUDED

#include <array>
#include <cstdint>

enum class Input
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
    std::array<std::uint8_t, 4096> ram_ = {};
    // Registers
    std::uint8_t *v_ = &ram_[0x06A0];
    std::uint16_t i_ = 0;
    std::uint16_t pc_ = 0x200;
    std::uint16_t sp_ = 0x6CF;
    // Timers
    std::uint8_t dt_ = 0;
    std::uint8_t st_ = 0;
    // Keys
    std::array<bool, 16> keys_ = {};
};

#endif
