/**
 * File: color.h
 *
 */
#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

/**
 * Struct: Color_t
 * Defines a 24-bit RGB color.
 */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color_t;

/**
 * Function: Color_cfg
 * Configures the given <Color_t> object and returns a pointer to itself as
 * a convenience.
 */
Color_t * Color_cfg(Color_t *pThis, uint8_t r, uint8_t g, uint8_t b);

Color_t * Color_copy(Color_t *pThis, const Color_t *pRhs);

/**
 * Function: Color
 * Dynamically allocated a new <Color_t> object, and initializes it
 * with <Color_cfg>.
 *
 * Aborts the program if there isn't enough memory to allocate.
 */
Color_t * Color(const uint8_t r, const uint8_t g, const uint8_t b);

/**
 * Function: Color_clone
 * Dynamically allocates a new <Color_t> object and configures it to match
 * the given source object.
 *
 * As with <Color>, aborts the program is there isn't enough memory.
 */
Color_t * Color_clone(const Color_t *pRhs);

#endif
//end inclusion filter

