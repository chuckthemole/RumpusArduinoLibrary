#ifndef OUTPUT_BASE_H
#define OUTPUT_BASE_H

#include <Arduino.h>

/**
 * @brief Abstract base class for all user output devices.
 *
 * This class defines the common interface for displaying messages,
 * clearing the screen/console, and general output operations.
 */
class OutputBase
{
public:
    virtual ~OutputBase() {}

    /**
     * @brief Initialize the output device.
     *
     * Must be called during setup() before using the output device.
     */
    virtual void begin() = 0;

    /**
     * @brief Clear the output device (screen, console, etc.).
     */
    virtual void clear() = 0;

    /**
     * @brief Print a message without a newline.
     *
     * @param msg The message to print.
     */
    virtual void print(const String &msg) = 0;

    /**
     * @brief Print a message followed by a newline.
     *
     * @param msg The message to print.
     */
    virtual void println(const String &msg) = 0;
};

#endif // OUTPUT_BASE_H
