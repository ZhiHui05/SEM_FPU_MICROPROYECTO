#ifndef PI_H
#define PI_H
#include "freertos/semphr.h"

// Declaración externa del semáforo
extern SemaphoreHandle_t done_sem;

void menu(uint64_t time,uint32_t iteraciones);

void calcular_pi_leibniz(void *pvParameters);
void calcular_pi_montecarlo(void *pvParameters);
void calcular_pi_nilakantha(void *pvParameters);

#endif