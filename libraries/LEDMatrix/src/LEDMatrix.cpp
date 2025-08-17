#include "LEDMatrix.h"

/**
 * @file LEDMatrix.cpp
 * @brief Implementation file for the LEDMatrix abstract interface.
 *
 * Currently this class is purely abstract.
 *
 * Keeping this for now:
 * 1. Virtual destructor: Ensures proper cleanup when deleting derived objects
 *    via a LEDMatrix* pointer.
 * 2. Shared helper functions: Future utility methods for frame manipulation,
 *    color conversion, or validation can live here.
 * 3. Consistency: Keeps the library structure uniform and maintainable,
 *    making it easier to extend in the future.
 */

// LEDMatrix::~LEDMatrix()
// {
//     // Virtual destructor ensures proper cleanup in derived classes.
// }
