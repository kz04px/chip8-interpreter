#include "chip8.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

const std::uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

Chip8::Chip8()
    : ram_{},
      v_{&ram_[0x06A0]},
      i_{0},
      pc_{0x200},
      sp_{0x6CF},
      dt_{0},
      st_{0},
      keys_{0} {
    assert(v_);
    memcpy(ram_, fontset, 80);
}

bool Chip8::load(const char *path) {
    assert(path);

    FILE *file = fopen(path, "rb");
    if (!file) {
        return false;
    }
    fread(ram_ + 0x200, 4096 - 0x200, 1, file);
    fclose(file);
    return true;
}

void Chip8::set_key(const Input a, const bool s) {
    keys_[static_cast<int>(a)] = s;
}

bool Chip8::get_key(const Input a) const {
    return keys_[static_cast<int>(a)];
}

bool Chip8::pixel(const int x, const int y) const {
    assert(0 <= x && x < 64);
    assert(0 <= y && y < 32);
    return (ram_[0x0700 + (x / 8) + (8 * y)] >> (7 - (x % 8))) & 1;
}

void Chip8::step() {
    assert(pc_ + 1 < 4096);
    assert(i_ <= 0xFFF);
    assert(sp_ <= 0x6CF);

    const std::uint16_t opcode = (ram_[pc_] << 8) + ram_[pc_ + 1];
    const std::uint16_t nnn = opcode & 0x0FFF;
    const std::uint8_t kk = opcode & 0x00FF;
    const std::uint8_t x = (opcode & 0x0F00) >> 8;
    const std::uint8_t y = (opcode & 0x00F0) >> 4;
    const std::uint8_t n = (opcode & 0x000F);

    // Instructions
    switch (opcode & 0xF000) {
        case 0x0000: {
            // 00EE - RET
            if (opcode == 0x00EE) {
                pc_ = (ram_[sp_ + 1] << 8) + ram_[sp_ + 2] + 2;
                sp_ += 2;
            }
            // 00E0 - CLS
            else if (opcode == 0x00E0) {
                memset(&ram_[0x0700], 0, 8 * 32);
                pc_ += 2;
            }
            // 0nnn - SYS addr
            else {
                pc_ = nnn;
            }
            break;
        }
        // 1nnn - JP addr
        case 0x1000: {
            pc_ = nnn;
            break;
        }
        // 2nnn - CALL addr
        case 0x2000: {
            ram_[sp_] = pc_ & 0x00FF;
            ram_[sp_ - 1] = (pc_ & 0xFF00) >> 8;
            sp_ -= 2;
            pc_ = nnn;
            break;
        }
        // 3xkk - SE Vx, byte
        case 0x3000: {
            assert(x < 16);
            pc_ += v_[x] == kk ? 4 : 2;
            break;
        }
        // 4xkk - SNE Vx, byte
        case 0x4000: {
            assert(x < 16);
            pc_ += v_[x] != kk ? 4 : 2;
            break;
        }
        // 6xkk - LD Vx, byte
        case 0x6000: {
            assert(x < 16);
            v_[x] = kk;
            pc_ += 2;
            break;
        }
        // 7xkk - ADD Vx, byte
        case 0x7000: {
            assert(x < 16);
            v_[x] += kk;
            pc_ += 2;
            break;
        }
        // Annn - LD I, addr
        case 0xA000: {
            i_ = nnn;
            pc_ += 2;
            break;
        }
        // Bnnn - JP V0, addr
        case 0xB000: {
            pc_ = v_[0x0] + nnn;
            break;
        }
        // Cxkk - RND Vx, byte
        case 0xC000: {
            assert(x < 16);
            v_[x] = (rand() % 256) & kk;
            pc_ += 2;
            break;
        }
        // Dxyn - DRW Vx, Vy, nibble
        case 0xD000: {
            assert(x < 16);
            assert(y < 16);
            assert(i_ + n <= 4096);
            v_[0xF] = 0;
            for (int a = 0; a < n; ++a) {
                const int xpos = v_[x] % 64;
                const int ypos = (v_[y] + a) % 32;
                const std::uint8_t left = ram_[i_ + a] >> (xpos % 8);
                const std::uint8_t right = ram_[i_ + a] << (8 - xpos % 8);

                const int idx_left = 0x0700 + xpos / 8 + 8 * ypos;
                v_[0xF] |= ram_[idx_left] & left;
                ram_[idx_left] ^= left;

                const int idx_right = 0x0700 + (xpos + 8) / 8 + 8 * ypos;
                v_[0xF] |= ram_[idx_right] & right;
                ram_[idx_right] ^= right;
            }

            if (v_[0xF]) {
                v_[0xF] = 1;
            }

            pc_ += 2;
            break;
        }
        case 0xE000:
            switch (opcode & 0xF0FF) {
                // Ex9E - SKP Vx
                case 0xE09E: {
                    assert(x < 16);
                    assert(v_[x] < 16);
                    pc_ += keys_[v_[x]] == true ? 4 : 2;
                    break;
                }
                // ExA1 - SKNP Vx
                case 0xE0A1: {
                    assert(x < 16);
                    assert(v_[x] < 16);
                    pc_ += keys_[v_[x]] != true ? 4 : 2;
                    break;
                }
                default: {
                    assert(false);
                    break;
                }
            }
            break;
        case 0xF000:
            switch (opcode & 0xF0FF) {
                // Fx07 - LD Vx, DT
                case 0xF007: {
                    assert(x < 16);
                    v_[x] = dt_;
                    pc_ += 2;
                    break;
                }
                // Fx0A - LD Vx, K
                case 0xF00A: {
                    assert(x < 16);
                    for (int i = 0; i < 16; ++i) {
                        if (keys_[i]) {
                            v_[x] = i;
                            pc_ += 2;
                            break;
                        }
                    }
                    break;
                }
                // Fx15 - LD DT, Vx
                case 0xF015: {
                    assert(x < 16);
                    dt_ = v_[x];
                    pc_ += 2;
                    break;
                }
                // Fx18 - LD ST, Vx
                case 0xF018: {
                    assert(x < 16);
                    st_ = v_[x];
                    pc_ += 2;
                    break;
                }
                // Fx1E - ADD I, Vx
                case 0xF01E: {
                    assert(x < 16);
                    i_ += v_[x];
                    pc_ += 2;
                    break;
                }
                // Fx29 - LD F, Vx
                case 0xF029: {
                    assert(x < 16);
                    i_ = 5 * v_[x];
                    pc_ += 2;
                    break;
                }
                // Fx33 - LD B, Vx
                case 0xF033: {
                    assert(x < 16);
                    assert(i_ + 2 < 4096);
                    ram_[i_ + 0] = (v_[x] / 100) % 10;
                    ram_[i_ + 1] = (v_[x] / 10) % 10;
                    ram_[i_ + 2] = (v_[x] / 1) % 10;
                    pc_ += 2;
                    break;
                }
                // Fx55 - LD [I], Vx
                case 0xF055: {
                    assert(x < 16);
                    assert(i_ + x < 4096);
                    for (int a = 0; a <= x; ++a) {
                        ram_[i_ + a] = v_[a];
                    }
                    pc_ += 2;
                    break;
                }
                // Fx65 - LD Vx, [I]
                case 0xF065: {
                    assert(x < 16);
                    assert(i_ + x < 4096);
                    for (int a = 0; a <= x; ++a) {
                        v_[a] = ram_[i_ + a];
                    }
                    pc_ += 2;
                    break;
                }
                default:
                    assert(false);
                    break;
            }
            break;
        default:
            switch (opcode & 0xF00F) {
                // 5xy0 - SE Vx, Vy
                case 0x5000: {
                    assert(x < 16);
                    assert(y < 16);
                    pc_ += v_[x] == v_[y] ? 4 : 2;
                    break;
                }
                // 8xy0 - LD Vx, Vy
                case 0x8000: {
                    assert(x < 16);
                    assert(y < 16);
                    v_[x] = v_[y];
                    pc_ += 2;
                    break;
                }
                // 8xy1 - OR Vx, Vy
                case 0x8001: {
                    assert(x < 16);
                    assert(y < 16);
                    v_[x] |= v_[y];
                    pc_ += 2;
                    break;
                }
                // 8xy2 - AND Vx, Vy
                case 0x8002: {
                    assert(x < 16);
                    assert(y < 16);
                    v_[x] &= v_[y];
                    pc_ += 2;
                    break;
                }
                // 8xy3 - XOR Vx, Vy
                case 0x8003: {
                    assert(x < 16);
                    assert(y < 16);
                    v_[x] ^= v_[y];
                    pc_ += 2;
                    break;
                }
                // 8xy4 - ADD Vx, Vy
                case 0x8004: {
                    assert(x < 16);
                    assert(y < 16);
                    v_[0xF] = (int)v_[x] + (int)v_[y] > 255 ? 1 : 0;
                    v_[x] += v_[y];
                    pc_ += 2;
                    break;
                }
                // 8xy5 - SUB Vx, Vy
                case 0x8005: {
                    v_[0xF] = v_[x] > v_[y] ? 1 : 0;
                    v_[x] -= v_[y];
                    pc_ += 2;
                    break;
                }
                // 8xy6 - SHR Vx {, Vy}
                case 0x8006: {
                    v_[0xF] = v_[x] & 1;
                    v_[x] = v_[x] >> 1;
                    pc_ += 2;
                    break;
                }
                // 8xy7 - SUBN Vx, Vy
                case 0x8007: {
                    v_[0xF] = v_[y] > v_[x] ? 1 : 0;
                    v_[x] = v_[y] - v_[x];
                    pc_ += 2;
                    break;
                }
                // 8xyE - SHL Vx {, Vy}
                case 0x800E: {
                    v_[0xF] = (v_[x] >> 7) & 1;
                    v_[x] = v_[x] << 1;
                    pc_ += 2;
                    break;
                }
                // 9xy0 - SNE Vx, Vy
                case 0x9000: {
                    pc_ += v_[x] != v_[y] ? 4 : 2;
                    break;
                }
                default: {
                    assert(false);
                    break;
                }
            }
            break;
    }

    // Delay timer
    if (dt_ > 0) {
        dt_--;
    }

    // Sound timer
    if (st_ > 0) {
        if (st_ == 1) {
            // TODO:
            // -- Beep
        }
        st_--;
    }
}
