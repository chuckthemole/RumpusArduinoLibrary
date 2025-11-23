#pragma once

// These are exported from your custom linker file
extern "C"
{
    extern char __DTCM_START__, __DTCM_END__;
    extern char __AXI_START__, __AXI_END__;
    extern char __SRAM1_START__, __SRAM1_END__;
    extern char __SRAM2_START__, __SRAM2_END__;
}

// Helper to compute region size in bytes
inline int regionSize(char *start, char *end)
{
    return (int)(end - start);
}

// --- Region Sizes (fixed) ---
inline int dtcmTotal() { return regionSize(&__DTCM_START__, &__DTCM_END__); }
inline int axiTotal() { return regionSize(&__AXI_START__, &__AXI_END__); }
inline int sram1Total() { return regionSize(&__SRAM1_START__, &__SRAM1_END__); }
inline int sram2Total() { return regionSize(&__SRAM2_START__, &__SRAM2_END__); }
