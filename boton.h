#ifndef BOTON_H
#define BOTON_H


typedef enum {BUTTON_PRESSED, BUTTON_RELEASED} button_state_t;

void button_init(void);
button_state_t button_get(void);

#endif