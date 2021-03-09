#include "ei_ble_com.h"
#include <string.h>
#include <zephyr.h>

#include "cJSON.h"
#include "cJSON_os.h"
#include "ei_device_nordic_nrf52.h"

void ei_ble_connect_handshake (uint8_t * ei_ble_con_hs) {
    ei_printf("vss send back handshake data\n");

    uint8_t num_of_sensors = 2;
    uint8_t id_buffer[32] = { 0 };
    size_t id_size;

    char *string = NULL;
    cJSON *message = NULL;
    cJSON *hello = NULL;
    cJSON *sensors = NULL;
    cJSON *frequencies = NULL;
    size_t index = 0;


    cJSON *handshake = cJSON_CreateObject();

    if(NULL == cJSON_AddStringToObject(handshake, "type", "ws")){
        ei_printf("error: cJSON_AddStringToObject");
    }

    if(NULL == cJSON_AddStringToObject(handshake, "direction", "tx")){
        ei_printf("error: cJSON_AddStringToObject");
    }

    if(NULL == cJSON_AddStringToObject(handshake, "address", "wss://studio.edgeimpulse.com")){
        ei_printf("error: cJSON_AddStringToObject");
    }

    message = cJSON_AddObjectToObject(handshake, "message");
    if(NULL == message){
        ei_printf("error: cJSON_AddObjectToObject");
    }

    hello = cJSON_AddObjectToObject(message, "hello");
    if(NULL == hello){
        ei_printf("error: cJSON_AddObjectToObject");
    }

    if(NULL == cJSON_AddNumberToObject(hello, "version", 3)){
        ei_printf("error: cJSON_AddStringToObject");
    }

    if(NULL == cJSON_AddStringToObject(hello, "apiKey", "ei_1234")){
        ei_printf("error: cJSON_AddStringToObject");
    }

    if(NULL == cJSON_AddStringToObject(hello, "deviceId", EiDevice.get_id_pointer())){
        ei_printf("error: cJSON_AddStringToObject");
    }
    //r = ei_config_get_context()->get_device_type(id_buffer, &id_size);
    if(NULL == cJSON_AddStringToObject(hello, "deviceType", EiDevice.get_type_pointer())){
        ei_printf("error: cJSON_AddStringToObject");
    }

    if(NULL == cJSON_AddStringToObject(hello, "connection", "ip")){
        ei_printf("error: cJSON_AddStringToObject");
    }

    sensors = cJSON_AddArrayToObject(hello, "sensors");
    if(NULL == sensors){
        ei_printf("error: cJSON_AddArrayToObject");
    }

    for(int i = 0; i < num_of_sensors; i++){
        cJSON *sensor = cJSON_CreateObject();
        cJSON_AddStringToObject(sensor, "name", "Accelerometer");
        cJSON_AddNumberToObject(sensor, "maxSampleLengthS", 60000);
        frequencies = cJSON_AddArrayToObject(sensor, "frequencies");
        cJSON_AddItemToArray(sensors, sensor);
    }

    if(NULL == cJSON_AddFalseToObject(hello, "supportsSnapshotStreaming")){
        ei_printf("error: cJSON_AddStringToObject");
    }


    string = cJSON_Print(handshake);
    if (string == NULL)
    {
        ei_printf("Failed to print monitor.\n");
    }
    else
    {
        ei_printf("handshake message: \n");
        ei_printf("%s\n", string);
    }

    cJSON_Delete(handshake);
    
}

void setup_cjson(void)
{
    // set allocators
    ei_printf("Setting up CJSON Hooks\n");
    static cJSON_Hooks hooks = {
        .malloc_fn = k_malloc,
        .free_fn = k_free,
    };
    cJSON_InitHooks(&hooks);
    //cJSON_Init();
    ei_printf("Setting up CJSON Hooks done\n");
}