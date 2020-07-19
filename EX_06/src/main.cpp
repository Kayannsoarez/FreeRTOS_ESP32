/*********************************************************************
 *  Consumo de Memoria: High Water Mark 
 *  Esse exemplo exibe como verificar o consumo de memória da tarefa
 *  Por: Kayann Soares
**********************************************************************/
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* Mapeamento de Pinos */
#define LED 2
#define LEDoffB 18

/* Variaveis para armazenamento do Handle das Tasks */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

/* Prototipos das Tasks */
void vTaskBlink(void *pvParameters);
void vTask2(void *pvParameters);

/*variáveis auxiliares*/
int aux = 500;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  /* Criação das Tasks */
  xTaskCreatePinnedToCore(vTaskBlink, "TASK1", configMINIMAL_STACK_SIZE, (void*)LED, 1, &task1Handle, APP_CPU_NUM);
  xTaskCreatePinnedToCore(vTask2, "TASK2", configMINIMAL_STACK_SIZE+1024, (void*)aux, 2, &task2Handle, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(vTaskBlink, "TASK3", configMINIMAL_STACK_SIZE, (void*)LEDoffB, 1, &task3Handle, APP_CPU_NUM);
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(3000);
  
}

void vTaskBlink(void *pvParameters){
  UBaseType_t uxHighWaterMark;
  int pin = (int)pvParameters;
  
  pinMode(pin, OUTPUT);
  
  while(1){
    digitalWrite(pin,!digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(200));

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" : ");
    Serial.println(uxHighWaterMark);
  }
}

void vTask2(void *pvParameters){
  UBaseType_t uxHighWaterMark;
  int cont = (int)pvParameters;

  while(1){
    Serial.println("TASK 2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000));

    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" : ");
    Serial.println(uxHighWaterMark);
  }
}