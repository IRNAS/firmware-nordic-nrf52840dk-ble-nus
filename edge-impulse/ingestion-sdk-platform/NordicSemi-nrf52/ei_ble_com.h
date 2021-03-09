/* Include ----------------------------------------------------------------- */
#include "ei_device_info.h"
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

#include <cstdio>

#include "cJSON.h"
#include "cJSON_os.h"

extern void ei_printf(const char *format, ...);

void ei_ble_connect_handshake(uint8_t * ei_ble_con_hs);

void setup_cjson(void);