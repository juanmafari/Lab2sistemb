#include <stdio.h>
#include "tomicro.h"
#include "esp_rom_sys.h"

void delay_ms(int delayMs)
{
    int delayU = delayMs*1000;
    esp_rom_delay_us(delayU);
}
