#include <zephyr/types.h>
#include <zephyr.h>
#include <drivers/uart.h>

#include <device.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>

#include <bluetooth/services/nus.h>

#include <dk_buttons_and_leds.h>

#include <settings/settings.h>

#include <stdio.h>

#include <logging/log.h>

extern bool ei_ble_rcv_cmd_flag;
extern char ei_ble_rcv_cmd_buffer[50];

void ble_nus_init(void);
void ble_write_thread(void);