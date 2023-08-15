
#include "Arduino.h"
#include <STM32FreeRTOS.h>
#include "LoRa_E220.h"
#include "stm32-lora-220.h"




void setup() {

stm32_e220_start();
 
}
  


void loop() {
	 // Suspend the loop since tasks are now being managed by FreeRTOS
    delay(1000);   

}



