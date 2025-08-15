```cpp
#include <ElectricEye.h>

ElectricEye eye(A0, 500); // analog pin A0, threshold 500

void beamBroken() {
    Serial.println("Beam broken!");
}

void beamRestored() {
    Serial.println("Beam restored!");
}

void setup() {
    Serial.begin(115200);
    eye.begin();
    eye.onBeamBroken(beamBroken);
    eye.onBeamRestored(beamRestored);
}

void loop() {
    eye.update();
}
```