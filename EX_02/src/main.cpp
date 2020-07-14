/***************************************************************
 *  Deletando tasks
 *  Esse exemplo exibe como deletar uma task no FreeRTOS
 *  Por: Kayann Soares
***************************************************************/
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* Mapeamento de Pinos */
#define LED 2

/* Variaveis para armazenamento do Handle das Tasks */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

/* Prototipos das Tasks */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  /* Criação das tasks */
  xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle);

}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000);
  
}

void vTask1(void *pvParameters){
  pinMode(LED, OUTPUT);
  
  while(1){
    digitalWrite(LED,!digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void vTask2(void *pvParameters){
  int cont = 0;

  while(1){
    Serial.println("TASK 2: " + String(cont++));

    if(cont>=10){
      if(task1Handle != NULL){
        Serial.println("Deletando Task 1");
        vTaskDelete(task1Handle);
        digitalWrite(LED, LOW);
        task1Handle = NULL;
      }

      if(cont>=15){
        Serial.println("Deletando Task 2");
        vTaskDelete(NULL);
      }

    }
    vTaskDelay(pdMS_TO_TICKS(1000));

  }
}
