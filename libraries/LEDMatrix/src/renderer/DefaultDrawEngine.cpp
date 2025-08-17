#include "DefaultDrawEngine.h"

/**
 * @brief Construct a new DefaultDrawEngine
 * @param pin Optional pin for built-in LED testing
 */
DefaultDrawEngine::DefaultDrawEngine(uint8_t pin)
    : DrawEngine(nullptr), _pin(pin)
{
}

/**
 * @brief Initialize the hardware
 */
void DefaultDrawEngine::begin()
{
    pinMode(_pin, OUTPUT);
    if (_logger)
        _logger->info("DefaultDrawEngine initialized");
}

/**
 * @brief Render a prebuilt frame
 * This default implementation just blinks the built-in LED for demo purposes.
 */
void DefaultDrawEngine::renderFrame(const uint8_t frame[LEDMatrix::Rows::EIGHT][LEDMatrix::Columns::TWELVE])
{
    // Optional: Blink LED for each render for visual feedback
    digitalWrite(_pin, HIGH);
    delay(50);
    digitalWrite(_pin, LOW);

    if (_logger)
        _logger->debug("DefaultDrawEngine: renderFrame called");
}

/**
 * @brief Set brightness (dummy, does nothing here)
 */
void DefaultDrawEngine::setBrightness(uint8_t level)
{
    if (_logger)
        _logger->debug(String("DefaultDrawEngine: setBrightness ") + level);
}
