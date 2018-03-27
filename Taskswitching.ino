#include <Arduino_FreeRTOS.h>
#include <stdio.h>
#include "debugio.h"
#include "simulavr_info.h"
#include "HardwareSerial.h"
#include "queue.h"
 
 
QueueHandle_t queueHandleT2toT1 = NULL;
QueueHandle_t queueHandleT1toT2 = NULL;
 
 
void loop()
{
  // DO nothing
}

/* Task2 with priority 1 */
static void MyTask2(void* pvParameters)
{ 
  uint32_t dataToSend = 0;
  uint32_t dataToReceive;
    
    
  int c=0;
  while(1)
  { 
      
    //PORTC:
    //4 MSB: Data send from Task2 to Task1
    //4 LSB: Data received in Task2 from Task1
    if(xQueueSend(queueHandleT2toT1, &dataToSend, 0)){
      PORTC = PORTC & 0x0F; //Clear the 4 MSB of PORTC  
      PORTC = (dataToSend<<4) | PORTC; //Store dataToSend in 4 MSB of PORTC 
    } 
    
    if(xQueueReceive(queueHandleT1toT2, &dataToReceive, 0)){
      PORTC = PORTC & 0xF0; //Clear the 4 LSB of PORTC  
      PORTC = dataToReceive | PORTC; //Store dataToReceive in 4 LSB of PORTC     
    } 
    
    dataToSend++;
      
    //Serial.print(F("Task2"));
    //vTaskDelay(100/portTICK_PERIOD_MS);
    //PORTC = PORTC ^ 0x01;
    //for(x2=0;x2<30000; x2++){}   
    //PORTC = PORTC ^ 0x01;
    
    vTaskDelay(10);
  }
}


/* Task1 with priority 2 */
static void MyTask1(void* pvParameters)
{ 
  int c=0;
  
  uint32_t dataToSend = 0;
  uint32_t dataToReceive;
  
  while(1)
  { 
    //PORTB:
    //4 MSB: Data received in Task1 from Task2
    //4 LSB: Data send from Task1 to Task2  
    if(xQueueReceive(queueHandleT2toT1, &dataToReceive, 0)){
      PORTB = PORTB & 0x0F; //Clear the 4 MSB of PORTB  
      PORTB = (dataToReceive<<4) | PORTB; //Store dataToReceive in 4 MSB of PORTB       
    } 
      
    if(xQueueSend(queueHandleT1toT2, &dataToSend, 0)){
      PORTB = PORTB & 0xF0; //Clear the 4 LSB of PORTB  
      PORTB = dataToReceive | PORTB; //Store dataToSend in 4 LSB of PORTB   
    } 
    
    dataToSend++;
      
    //printk( "TASK1 %d\n",c++ );
    //Serial.println(F("Task1"));
    //PORTB = PORTB ^ 0x01;
    //for(x2=0;x2<30000; x2++){}   
    //PORTB = PORTB ^ 0x01;
    
    vTaskDelay(5);
  }
}

/* Idle Task with priority Zero */ 
static void MyIdleTask(void* pvParameters)
{ 
  while(1){ 
    delay(5);
    //Serial.println(F("TASK0"));
  }
}

void setup()
{ 
  DDRB = 0x0F;
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(19200); // double baudrate compared to setting why?
  Serial.println(F("In Setup function"));
  
  queueHandleT2toT1 = xQueueCreate(5, sizeof(int));
  queueHandleT1toT2 = xQueueCreate(5, sizeof(int));
 
  xTaskCreate(MyIdleTask, "IdleTask", 256, NULL, 0, NULL); 
  xTaskCreate(MyTask1, "Task1", 256, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 256, NULL, 2, NULL );
  //vTaskStartScheduler();
  xPortStartScheduler();
}


