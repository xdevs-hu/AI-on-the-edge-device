#ifdef ENABLE_GATT
#pragma once

#ifndef SERVERGATT_H
#define SERVERGATT_H

#include <string>

bool gattService_startAdvertising();
void gattServer_setMeterValue(std::string val);

#endif //SERVERGATT_H
#endif //ENABLE_GATT