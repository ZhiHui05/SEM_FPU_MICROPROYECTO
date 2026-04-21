#include <stdio.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "boton.h"

#define GPIO_BUTTON GPIO_NUM_0

//void delay(uint16_t retardo);

void button_init(void)
{
    // zero-initialize the config structure.
    gpio_config_t io_conf = {};
    // bit mask of the pins that you want to set, e.g. GPIO18
    io_conf.pin_bit_mask = 1ULL << GPIO_BUTTON;
    // set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    // disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    // disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;

    // configure GPIO with the given settings
    gpio_config(&io_conf);
    printf("Hola, pulsador!\n");


}


button_state_t button_get(){
    button_state_t state;
   
        uint8_t pin_level;
        pin_level = gpio_get_level(GPIO_BUTTON);
        if (pin_level == 0)
        {
            //printf("0\n");
            state = 0;
        }
        else
        {
            //printf("1\n");
            state = 1;
        }
        //delay(50);
        return state;
    
}
