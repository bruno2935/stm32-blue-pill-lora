#include "stm32-lora-220.h"
#include "Arduino.h"
#include "utils.h"
#include <STM32FreeRTOS.h>
#include <HardwareSerial.h>

HardwareSerial Serial2(USART2);   // PA3  (RX)  PA2  (TX)
// ---- Utils functions and globals def



/*********************************************************
* Convert the struct command in to a string buffer
**********************************************************/
void serializeCommand(const void *command, char *hexString)
{
	uint8_t serialized_command[sizeof(struct ControlCommand)];
	size_t len = sizeof(struct ControlCommand) / sizeof(uint8_t);
	const struct ControlCommand *controlData = (const struct ControlCommand *)command;
	// Copy the struct commands in to the uint8_t ControlData buffer
	memcpy(serialized_command, controlData, sizeof(struct ControlCommand));
	// Transform each hex ASCII representation to char
    for (size_t i = 0; i < len; ++i) {
        sprintf(hexString + i * 2, "%02X", serialized_command[i]);
    }
}

/*
* Transmite the Parameters
*/
void E220_TX(void *pvParameters)
{
	// String to hold the ControlCommand bytes
	char hexString[sizeof(struct ControlCommand) * 2 + 1]; // +1 for null terminator
	
	const ControlCommand *controlData = (const ControlCommand *)pvParameters;
	// Serialize the command struct in to hexString
	serializeCommand(controlData, hexString);
	// sprintf(message, "Serial-Command: %s", hexString);
	// Write the data in to the UART2, that is connect to the LoRa device.
	Serial2.println(hexString); 
}

/**
 * Converts bytes to the struct command.
*/
void deserializeCommand(const char *hexString , struct ControlCommand *command) 
{
	uint8_t bytes[sizeof(struct ControlCommand)];
	//Transform each caracter to uint8_t representation
	size_t len = sizeof(struct ControlCommand) / sizeof(uint8_t);
    for (size_t i = 0; i < len; ++i) {
        sscanf(hexString + i * 2, "%2hhX", &bytes[i]);
    }
    memcpy(command, bytes, sizeof(struct ControlCommand));
}




void transmmitOverLora(const void *Param)
{

		const ControlCommand *controlData = (const ControlCommand *)Param;
		E220_TX(&controlData);
		//Wait for ACK 
		E220_RX_ACK();

} \



static void task1(void *pvParameters)
{
	struct ControlCommand esp_lora_command = {ESP_LORA_DEVICE_ID_1
							,ESP_LORA_DEVICE_ACTION_WATER
							,ESP_LORA_ON};
    while (1) 
	{
		transmmitOverLora(&esp_lora_command);
		vTaskDelay(pdMS_TO_TICKS(1000)); // Delay the task for 1000 milliseconds

    }

}

void stm32_e220ttl_begin(void)
{
	// stm32_e220_start();
	Serial2.begin(9600);
	// Initialize the GPIO pins as outputs
    pinMode(PB0, OUTPUT);
    pinMode(PB10, OUTPUT);
    
    // Set the pins to a low state
    digitalWrite(PB0, LOW);
    digitalWrite(PB10, LOW);

}


void stm32_e220_start(void)
{

	stm32_e220ttl_begin();
	xTaskCreate(task1,"Task1",
              configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
    vTaskStartScheduler();

}