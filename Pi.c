#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "Pi.h"
#include "esp_log.h"


static const char *TAG1 = "LEIBNIZ_PI";
static const char *TAG2 = "MONTECARLO_PI";
static const char *TAG3 = "NILAKANTHA_PI";

void menu(uint64_t time,uint32_t iteraciones){

    ESP_LOGI("APP_MAIN", "Cálculo de PI usando FPU del ESP32-S3...");
    
    if(time > 0 && time <= 1000){
        
        xTaskCreate(calcular_pi_leibniz, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
        
    }
    else if (time > 1000 && time < 3000){
        
        xTaskCreate(calcular_pi_nilakantha, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
        
    }else if (time >=3000){
        
        xTaskCreate(calcular_pi_montecarlo, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
        
    }
        xSemaphoreTake(done_sem, portMAX_DELAY);
        ESP_LOGI("APP_MAIN", "¡Cálculo finalizado correctamente!");

}

//===================================
void calcular_pi_leibniz(void *pvParameters) {
    uint16_t iteraciones = *(uint16_t *)pvParameters;
    double pi = 0.0;
    int signo = 1;
    ESP_LOGI("TASK:","Inciando Funcion");
    int64_t start = esp_timer_get_time();

    for (uint16_t k = 0; k < iteraciones; k++) {
        pi += signo * (1.0f / (2 * k + 1));
        signo *= -1;

        if (k % 10000 == 0) {
            vTaskDelay(1); // Cede control brevemente
        }
        //Por cada 1000 iteracion nos muestra el calculo de pi
        
        if (k % 1000 == 0 && k != 0) {
        double pi_actual = pi * 4.0f;
        vTaskDelay(10);
        ESP_LOGI(TAG1, "Iteración %d → π ≈ %.40f", k, pi_actual);
        printf("\n");
        }
        
    }

    pi *= 4.0f;
    int64_t end = esp_timer_get_time();
    printf("---------------------------------------------------------------\n");
    printf("Aproximación de PI con %d iteraciones: %.40f\n", iteraciones, pi);
    ESP_LOGI(TAG1, "Tiempo de cálculo: %lld microsegundos", end - start);
    printf("---------------------------------------------------------------\n");


    xSemaphoreGive(done_sem); //Semaphoro termina
    vTaskDelete(NULL); // Finaliza la tarea
}    

//===================================================
//Menos preciso pero mas dinamico y visual
void calcular_pi_montecarlo(void *pvParameters) {
    uint16_t muestras = *(uint16_t *)pvParameters;
    int dentro_circulo = 0;

    // Inicializar el generador de números aleatorios con una semilla
    srand((unsigned int) esp_timer_get_time());

    int64_t start = esp_timer_get_time();  // Tiempo inicial

    for (uint16_t i = 0; i < muestras; i++) {
        float x = (float)rand() / RAND_MAX;
        float y = (float)rand() / RAND_MAX;

        if ((x * x + y * y) <= 1.0f) {
            dentro_circulo++;
        }

        // Mostrar progreso cada 1000 muestras
        if (i % 1000 == 0) {
            double pi_aprox = 4.0f * dentro_circulo / (i + 1);
            vTaskDelay(10);
            ESP_LOGI(TAG2, "Iteración %d → π ≈ %.40f", i, pi_aprox);
            printf("\n");
            
        }

        // Evitar que el watchdog se dispare
        if (i % 10000 == 0) {
            vTaskDelay(1);
        }
    }


    double pi_final = 4.0f * dentro_circulo / muestras;
    int64_t end = esp_timer_get_time();  // Tiempo final
    printf("---------------------------------------------------------------\n");
    printf("Aproximación de PI con %d iteraciones: %.40f\n", muestras, pi_final);
    ESP_LOGI(TAG2, "Tiempo de cálculo: %lld microsegundos", end - start);
    printf("---------------------------------------------------------------\n");

    xSemaphoreGive(done_sem); //Semaphoro termina
    vTaskDelete(NULL);
}

//====================================================
//El mas preciso de todos
void calcular_pi_nilakantha(void *pvParameters) {
    uint16_t iteraciones = *(uint16_t *)pvParameters;
    float pi = 3.0f;
    int signo = 1;

    int64_t start = esp_timer_get_time();  // Tiempo inicial

    for (uint16_t n = 1; n <= iteraciones; n++) {
        float denominador = (2.0f * n) * (2.0f * n + 1) * (2.0f * n + 2);
        float termino = 4.0f / denominador;
        pi += signo * termino;
        signo *= -1;

        // Mostrar progreso cada 1000 iteraciones
        if (n % 1000 == 0) {
            vTaskDelay(10);
            ESP_LOGI(TAG3, "Iteración %d → π ≈ %.40f", n, pi);
            printf("\n");
        }

        // Prevenir el watchdog
        if (n % 10000 == 0) {
            vTaskDelay(1);
        }
    }

    int64_t end = esp_timer_get_time();  // Tiempo final
    printf("---------------------------------------------------------------\n");
    printf("Aproximación de PI con %d iteraciones: %.40f\n", iteraciones, pi);
    ESP_LOGI(TAG3, "Tiempo de cálculo: %lld microsegundos", end - start);

    xSemaphoreGive(done_sem); //Semaphoro termina
    vTaskDelete(NULL);
}