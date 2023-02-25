// Copyright (c) 2022 Xiaomi Guo
// Modern Ruby Game Engine (RGM) is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//          http://license.coscl.org.cn/MulanPSL2
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE. See the
// Mulan PSL v2 for more details.

#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
// Parameters:
//
//   Texture2D samplerState+shaderTexture;
//
//
// Registers:
//
//   Name                       Reg   Size
//   -------------------------- ----- ----
//   samplerState+shaderTexture s0       1
//

    ps_2_x
    def c0, 0.298999995, 0.587000012, 0.114, 0
    dcl t0.xy
    dcl_2d s0
    texld r0, t0, s0
    dp3 r0.x, r0, c0
    mov r0.y, r0.w
    mov r0, r0.xxxy
    mov oC0, r0

// approximately 5 instruction slots used (1 texture, 4 arithmetic)
#endif

extern constexpr unsigned char rgm_shader_gray_dx9_data[] = {
    1,   2,   255, 255, 254, 255, 36,  0,   67,  84,  65,  66,  28,  0,   0,
    0,   99,  0,   0,   0,   1,   2,   255, 255, 1,   0,   0,   0,   28,  0,
    0,   0,   0,   129, 0,   0,   92,  0,   0,   0,   48,  0,   0,   0,   3,
    0,   0,   0,   1,   0,   0,   0,   76,  0,   0,   0,   0,   0,   0,   0,
    115, 97,  109, 112, 108, 101, 114, 83,  116, 97,  116, 101, 43,  115, 104,
    97,  100, 101, 114, 84,  101, 120, 116, 117, 114, 101, 0,   171, 4,   0,
    7,   0,   1,   0,   4,   0,   1,   0,   0,   0,   0,   0,   0,   0,   112,
    115, 95,  50,  95,  97,  0,   77,  105, 99,  114, 111, 115, 111, 102, 116,
    32,  40,  82,  41,  32,  72,  76,  83,  76,  32,  83,  104, 97,  100, 101,
    114, 32,  67,  111, 109, 112, 105, 108, 101, 114, 32,  49,  48,  46,  49,
    0,   171, 81,  0,   0,   5,   0,   0,   15,  160, 135, 22,  153, 62,  162,
    69,  22,  63,  213, 120, 233, 61,  0,   0,   0,   0,   31,  0,   0,   2,
    0,   0,   0,   128, 0,   0,   3,   176, 31,  0,   0,   2,   0,   0,   0,
    144, 0,   8,   15,  160, 66,  0,   0,   3,   0,   0,   15,  128, 0,   0,
    228, 176, 0,   8,   228, 160, 8,   0,   0,   3,   0,   0,   1,   128, 0,
    0,   228, 128, 0,   0,   228, 160, 1,   0,   0,   2,   0,   0,   2,   128,
    0,   0,   255, 128, 1,   0,   0,   2,   0,   0,   15,  128, 0,   0,   64,
    128, 1,   0,   0,   2,   0,   8,   15,  128, 0,   0,   228, 128, 255, 255,
    0,   0};