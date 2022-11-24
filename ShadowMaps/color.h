#ifndef COLOR_H
#define COLOR_H

// Some Basic Colors
#define LIGHTGRAY  { 200, 200, 200, 255 }   // Light Gray
#define GRAY       { 130, 130, 130, 255 }   // Gray
#define DARKGRAY   { 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     { 253, 249, 0, 255 }     // Yellow
#define GOLD       { 255, 203, 0, 255 }     // Gold
#define ORANGE     { 255, 161, 0, 255 }     // Orange
#define PINK       { 255, 109, 194, 255 }   // Pink
#define RED        { 230, 41, 55, 255 }     // Red
#define MAROON     { 190, 33, 55, 255 }     // Maroon
#define GREEN      { 0, 228, 48, 255 }      // Green
#define LIME       { 0, 158, 47, 255 }      // Lime
#define DARKGREEN  { 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    { 102, 191, 255, 255 }   // Sky Blue
#define BLUE       { 0, 121, 241, 255 }     // Blue
#define DARKBLUE   { 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     { 200, 122, 255, 255 }   // Purple
#define VIOLET     { 135, 60, 190, 255 }    // Violet
#define DARKPURPLE { 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      { 211, 176, 131, 255 }   // Beige
#define BROWN      { 127, 106, 79, 255 }    // Brown
#define DARKBROWN  { 76, 63, 47, 255 }      // Dark Brown

#define WHITE      { 255, 255, 255, 255 }   // White
#define BLACK      { 0, 0, 0, 255 }         // Black
#define BLANK      { 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    { 255, 0, 255, 255 }     // Magenta

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

#endif // COLOR_H
