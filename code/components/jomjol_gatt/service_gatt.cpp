#ifdef ENABLE_GATT

#include "service_gatt.h"
#include "BLECast.h"

struct AdvData {
  char* data;
} advdat;

BLECast bleCast("esp32");

bool gattService_startAdvertising() {
    return bleCast.begin();
}

#endif //ENABLE_GATT
