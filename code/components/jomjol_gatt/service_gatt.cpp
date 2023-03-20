#ifdef ENABLE_GATT
#include <string>

#include "service_gatt.h"
#include "BLECast.h"

BLECast bleCast("esp32:");

bool gattService_startAdvertising() {
    return bleCast.begin();
}

void gattServer_setMeterValue(std::string val) {
  bleCast.extendDeviceName(val);
}
#endif //ENABLE_GATT
