#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "Pi.h"
#include "boton.h"
#include "esp_log.h"


SemaphoreHandle_t done_sem;
SemaphoreHandle_t button_sem;


static uint32_t iteraciones = 35000; // Numero de iteraciones para calcular PI
void app_main(void) {
    done_sem = xSemaphoreCreateBinary();
    button_init();
    uint64_t start = 0;
    uint64_t end = 0;
    uint64_t duracion = 0;
    bool boton_presionado = false;
    //uint8_t option = rand() % 3;
    printf("Cálculo de PI usando FPU del ESP32-S3\n");
    printf("-----------------------------------------\n");
    printf("Dendiendo del tiempo que pulses elegiras:\n");
    printf("< 1s -> Serie de Leibniz\n");
    printf("1-3s ->Serie de  Nilakantha\n");
    printf("> 3s ->Serie de MonteCarlos\n");

    for(;;){
        button_state_t state = button_get();

        if (!boton_presionado && state == 0) {  // botón PRESIONADO (activo bajo)
            boton_presionado = true;
            start = esp_timer_get_time();
            printf("Botón PRESIONADO\n");
        }

        if (boton_presionado && state == 1) {  // botón LIBERADO
            boton_presionado = false;
            end = esp_timer_get_time();
            duracion = end - start;  // en microsegundos

            printf("Botón LIBERADO → duración: %lld ms\n", duracion / 1000);
            menu(duracion /1000,iteraciones);
            
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // chequeo cada 10ms


    }

    

    #if 0
    
    ESP_LOGI("APP_MAIN", "Cálculo de PI usando FPU del ESP32-S3...");
    
    switch (option)
    {
    case 1:
        xTaskCreate(calcular_pi_leibniz, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
        break;
    case 2:
        xTaskCreate(calcular_pi_nilakantha, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
        break;
    default:
        xTaskCreate(calcular_pi_montecarlo, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
        break;
    }
    //xTaskCreate(calcular_pi_leibniz, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
    //xTaskCreate(calcular_pi_nilakantha, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);
    //xTaskCreate(calcular_pi_montecarlo, "calcular_pi", (1UL<<16), &iteraciones, 5, NULL);

    // Espera a que el semáforo se libere
    xSemaphoreTake(done_sem, portMAX_DELAY);
    ESP_LOGI("APP_MAIN", "¡Cálculo finalizado correctamente!");
    #endif
    
}
