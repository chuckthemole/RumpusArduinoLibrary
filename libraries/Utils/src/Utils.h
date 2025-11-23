#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h> // always needed
#include <stdlib.h>  // malloc(), free()
#include <stdint.h>  // uint32_t

// Needed for stack pointer (ARM / STM32 / GIGA)
#if defined(__arm__) || defined(ARDUINO_GIGA)
extern "C" uint32_t __get_MSP(void);
#endif

// ------------------------------------------------------------
// AVR (Uno, Mega, Nano, etc.)
// ------------------------------------------------------------
#if defined(__AVR__)

extern "C" char __heap_start__, *__brkval;

inline int freeRAM()
{
    char *brk = __brkval ? __brkval : &__heap_start__;
    return (int)SP - (int)brk;
}

// ------------------------------------------------------------
// ARDUINO GIGA R1
// ------------------------------------------------------------
#elif defined(ARDUINO_GIGA)

// Provided by STM32 linker
extern char _end;
extern char _estack;

inline int freeRAM()
{
    // allocate 4 bytes to get current heap end
    char *heap_end = (char *)malloc(4);
    char *stack_ptr = (char *)__get_MSP();

    int freeBytes = stack_ptr - heap_end;
    if (freeBytes < 0)
        freeBytes = 0;

    free(heap_end);
    return freeBytes;
}

// ------------------------------------------------------------
// GENERIC ARM (STM32, SAMD, ESP32 etc.)
// ------------------------------------------------------------
#elif defined(__arm__)

extern char _end;
extern char _estack;

inline int freeRAM()
{
    char *heap_end = (char *)malloc(4);
    char *stack_ptr = (char *)__get_MSP();

    int freeBytes = stack_ptr - heap_end;
    free(heap_end);

    return freeBytes > 0 ? freeBytes : 0;
}

// ------------------------------------------------------------
// FALLBACK
// ------------------------------------------------------------
#else
inline int freeRAM() { return 0; }
#endif

#endif
