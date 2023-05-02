#include <stdio.h>
#include "esp_err.h"
#include "led_strip.h"
#include <string.h> 
#include <stdio.h> 
#include <math.h>
#include "driver/rmt.h"



void white(led_strip_t *name);
void red(led_strip_t *name);
void green(led_strip_t *name);
void blue(led_strip_t *name);
void yellow(led_strip_t *name);
void cyan(led_strip_t *name);
void magenta(led_strip_t *name);

void redfade(led_strip_t *name);
void greenfade(led_strip_t *name);
void bluefade(led_strip_t *name);


void rainbow(led_strip_t *name);

void clearblack(led_strip_t *name);

void hextoco(led_strip_t *name, char col[]);

led_strip_t configure();