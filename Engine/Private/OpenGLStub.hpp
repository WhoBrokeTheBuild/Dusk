#ifndef DUSK_OPENGL_STUB_HPP
#define DUSK_OPENGL_STUB_HPP

#include <cstdint>

typedef uint32_t GLenum;

#define GL_INVALID_ENUM             0x0500

// Data Types
#define GL_BYTE                     0x1400
#define GL_UNSIGNED_BYTE            0x1401
#define GL_SHORT                    0x1402
#define GL_UNSIGNED_SHORT           0x1403
#define GL_UNSIGNED_INT             0x1405
#define GL_FLOAT                    0x1406

// Texture Wrap
#define GL_REPEAT                   0x2901
#define GL_MIRRORED_REPEAT          0x8370
#define GL_CLAMP_TO_EDGE            0x812F
#define GL_CLAMP_TO_BORDER          0x812D

// Texture Filter / Mipmap Mode
#define GL_NEAREST                  0x2600
#define GL_NEAREST_MIPMAP_LINEAR    0x2702
#define GL_NEAREST_MIPMAP_NEAREST   0x2700
#define GL_LINEAR                   0x2601
#define GL_LINEAR_MIPMAP_LINEAR     0x2703
#define GL_LINEAR_MIPMAP_NEAREST    0x2701

// Primitive Topology
#define GL_POINTS				    0x0000
#define GL_LINES				    0x0001
#define GL_LINE_LOOP				0x0002
#define GL_LINE_STRIP				0x0003
#define GL_TRIANGLES				0x0004
#define GL_TRIANGLE_STRIP			0x0005
#define GL_TRIANGLE_FAN				0x0006

// Buffer Draw Order
#define GL_ARRAY_BUFFER             0x8892
#define GL_ELEMENT_ARRAY_BUFFER     0x8893

#endif // DUSK_OPENGL_STUB_HPP