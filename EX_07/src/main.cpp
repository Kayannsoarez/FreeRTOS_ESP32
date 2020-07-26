/******************************************************************
 *  Criação, Adição e Leitura de Fila
 *  Ex exibe como trabalhar com dados de Filas através de 2 Tasks
 *  Por: Kayann Soares
*******************************************************************/
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

/* Mapeamento de Pinos */
#define LED 2

/* Variaveis da Fila */
QueueHandle_t xFila;

/* Variaveis para armazenamento do Handle das Tasks */
TaskHandle_t task1Handle;
TaskHandle_t task2Handle;

/* Prototipos das Tasks */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


void setup(){
  // put your setup code here, to run once:
  BaseType_t xReturned;

  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xFila = xQueueCreate(5, sizeof(int));

  if(xFila == NULL){
    Serial.println("Não foi possivel criar a Fila.");
    while(1);
  }

/* Criação e Verificação das tasks */
  xReturned = xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1Handle);

  if(xReturned == pdFAIL){
    Serial.println("Task1 Não Criada");
    while(1);
  }

  xReturned = xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle);

    if(xReturned == pdFAIL){
    Serial.println("Task2 Não Criada");
    while(1);
  }

}
 

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED,!digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));

}


void vTask1(void *pvParameters){
  int count = 0;

  while(1){
    if(count<10){
      xQueueSend(xFila, &count, portMAX_DELAY);
      count++;

    }else{
      count = 0;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
void vTask2(void *pvParameters){
  int valorREC = 0;

  while(1){
    if(xQueueReceive(xFila, &valorREC, pdMS_TO_TICKS(1000))==pdTRUE){
      Serial.println("Valor Recebido: " + String(valorREC));

    }else{
      Serial.println("TIMEOUT");
    }
  }
}