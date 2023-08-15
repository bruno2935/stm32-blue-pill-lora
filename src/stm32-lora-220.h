#include <stdint.h>

/**
 * Define the basic information about the Devices
*/
#define ESP_LORA_DEVICE_ID_1 0
#define ESP_LORA_DEVICE_ID_2 1

#define ESP_LORA_DEVICE_ACTION_WATER 0
#define ESP_LORA_DEVICE_ACTION_LIGH  1

#define ESP_LORA_ON  1
#define ESP_LORA_OFF 0


#define E220_CODE_SUCCESS 0
#define E220_CODE_FAIL 1


/*
* Device 0, actionType{0,1,2} or {0 -> TurnON OF PUMP, 1-> TurnON OF ligth, Parameter1{ON,OFF}}
*/
struct ControlCommand{
	uint8_t DeviceID;
	uint8_t actionType;
	uint8_t parameter1;
};

struct ControlResponse{
    uint8_t deviceID;
    uint8_t response;
};

void stm32_e220_start(void);