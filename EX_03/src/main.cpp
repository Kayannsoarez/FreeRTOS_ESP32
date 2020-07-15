/***************************************************************
 *  Parando e Reiniciando tasks
 *  Esse exemplo exibe como parar e reiniciar tarefas no FreeRTOS
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

  /* Criação das Tasks */
  xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &task1Handle);
  xTaskCreate(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task2Handle);

}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(3000);
  
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

    if(cont==10){
      Serial.println("Suspendendo Task 1...");
      digitalWrite(LED, LOW);
      vTaskSuspend(task1Handle);
 
    }
    else if(cont==15){
      Serial.println("Reiniciando Task 1...");
      vTaskResume(task1Handle);
      cont = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
