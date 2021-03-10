#include "ei_ble_com.h"
#include <string.h>
#include <zephyr.h>

#include "cJSON.h"
#include "cJSON_os.h"
#include "ei_device_nordic_nrf52.h"
#include "ei_config_types.h"

//TODO: need to use one from ei_config.h
#define EDGE_IMPULSE_MAX_FREQUENCIES_L (5)
extern const char * ei_config_get_api_key_pointer(void);
extern const char * ei_config_get_mgmt_url_pointer(void);

void ei_ble_connect_handshake (uint8_t * ei_ble_con_hs) {
    ei_printf("vss1 send back handshake data\n");

    uint32_t err_code = 0;

    const ei_device_sensor_t *list;
    size_t list_size;

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

    if(NULL == cJSON_AddStringToObject(handshake, "address", ei_config_get_mgmt_url_pointer())){
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

    if(NULL == cJSON_AddStringToObject(hello, "apiKey", ei_config_get_api_key_pointer())){
        ei_printf("error: cJSON_AddStringToObject");
    }

    if(NULL == cJSON_AddStringToObject(hello, "deviceId", EiDevice.get_id_pointer())){
        ei_printf("error: cJSON_AddStringToObject");
    }

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

    int r = EiDevice.get_sensor_list((const ei_device_sensor_t **)&list, &list_size);
    if (r != 0) {
        ei_printf("Failed to get sensor list (%d)\n", r);
        return;
    }

    for(size_t i = 0; i < list_size; i++){
        cJSON *sensor = cJSON_CreateObject();
        cJSON_AddStringToObject(sensor, "name", list[i].name);
        cJSON_AddNumberToObject(sensor, "maxSampleLengthS", (const double)list[i].max_sample_length_s);
        frequencies = cJSON_AddArrayToObject(sensor, "frequencies");
        for (int j = 0; j < EDGE_IMPULSE_MAX_FREQUENCIES_L; j++){
            if (list[i].frequencies[j] != 0.0f){
                cJSON_AddItemToArray(frequencies, cJSON_CreateNumber((const double) list[i].frequencies[j]));
            }
        }
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