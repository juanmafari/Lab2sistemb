#include "led.h"

led_strip_t *name;

/*funcion de seteo e inicializacion del led*/
void configure(){   
    led_strip_config_t ws2812cConfig = LED_STRIP_DEFAULT_CONFIG(1, RMT_CHANNEL_0); //seteo de cantidad de leds y protocolo de comunicacion
    name = led_strip_new_rmt_ws2812(&ws2812cConfig); // nuevo led del tipo ws2812
    esp_err_t ret = led_rgb_init(&name); //inicializacion con retorno de tipo OK o err
    //devuelve el led para trabajar con el
}

/*diferentes colores esenciales para su uso*/
void white(){
    name->set_pixel(name, 0, 255, 255, 255);
    name->refresh(name, 500); 
}
void red(){
    name->set_pixel(name, 0, 255, 0, 0);
    name->refresh(name, 500); 
}
void green(){
    name->set_pixel(name, 0, 0, 255, 0);
    name->refresh(name, 500); 
}
void blue(){
    name->set_pixel(name, 0, 0, 0, 255);
    name->refresh(name, 500); 
}
void yellow(){
    name->set_pixel(name, 0, 255, 255, 0);
    name->refresh(name, 500); 
}
void cyan(){
    name->set_pixel(name, 0, 0, 255, 255);
    name->refresh(name, 500); 
}
void magenta(){
    name->set_pixel(name, 0, 255, 0, 255);
    name->refresh(name, 500); 
}


/*funciones que realizan un fade in y fade out de un color especifico*/
void redfade(){
    for (size_t i = 0; i < 256; i++)
    {
        name->set_pixel(name, 0, i, 0, 0);
        name->refresh(name, 500);
    }
    for (size_t u = 255; u > 0; u--)
    {
        name->set_pixel(name, 0, u, 0, 0);
        name->refresh(name, 500);
    }
}

void greenfade(){
    for (size_t i = 0; i < 256; i++)
    {
        name->set_pixel(name, 0, 0, i, 0);
        name->refresh(name, 500);
    }
    for (size_t u = 255; u > 0; u--)
    {
        name->set_pixel(name, 0, 0, u, 0);
        name->refresh(name, 500);
    }
}

void bluefade(){
    for (size_t i = 0; i < 256; i++)
    {
        name->set_pixel(name, 0, 0, 0, i);
        name->refresh(name, 500);
    }
    for (size_t u = 255; u > 0; u--)
    {
        name->set_pixel(name, 0, 0, 0, u);
        name->refresh(name, 500);
    }  
}

/*funcion que realiza un barrido entre colores simulando el espectro de la luz*/
void rainbow(){
   
    uint32_t C[3];
    static int counterArray = 0;
    uint32_t A[3][3] = {{0,0,255},{0,255,0},{255,0,0}}; //valores iniciales
    for (uint32_t z = 0; z < 256; z++) //barrido de valores de 0 a 255
    {
        if (z==0)
        {
            counterArray = (counterArray + 1)%3; //check de variable para cambiar el valor inicial, cambia al recorrer dos canaeles (uno arranca en 0)
        }
        uint32_t S[3][3] = {{0,z,-z},{z,-z,0},{-z,0,z}}; //3 instancias, sube un canal baja el otro
        for (int i = 0; i < 3; i++) {  
            C[i] = A[counterArray][i] + S[counterArray][i];  
        }  
        name->set_pixel(name, 0, C[0], C[1], C[2]);
        name->refresh(name, 500);  
    }  
}
/*funcion para llevar el led a apagarse*/
void clearblack(){
    name->set_pixel(name,0,0,0,0);
    name->refresh(name, 500);
}

void hextoco( char col[]){
    int values[5];
    for (size_t i = 0; i < 6; i++)
    {
        switch(col[i])
        {
            case '0':
                values[i] = 0 * (pow(16,abs((i%2)-1)));
                break;
            case '1':
                values[i] = 1 * (pow(16,abs((i%2)-1)));
                break;
            case '2':
                values[i] = 2 * (pow(16,abs((i%2)-1)));
                break;
            case '3':
                values[i] = 3 * (pow(16,abs((i%2)-1)));
                break;
            case '4':
                values[i] = 4 * (pow(16,abs((i%2)-1)));
                break;
            case '5':
                values[i] = 5 * (pow(16,abs((i%2)-1)));
                break;
            case '6':
                values[i] = 6 * (pow(16,abs((i%2)-1)));
                break;
            case '7':
                values[i] = 7 * (pow(16,abs((i%2)-1)));
                break;
            case '8':
                values[i] = 8 * (pow(16,abs((i%2)-1)));
                break;
            case '9':
                values[i] = 9 * (pow(16,abs((i%2)-1)));
                break;
            case 'a':
            case 'A':
                values[i] = 10 * (pow(16,abs((i%2)-1)));
                break;
            case 'b':
            case 'B':
                values[i] = 11 * (pow(16,abs((i%2)-1)));
                break;
            case 'c':
            case 'C':
                values[i] = 12 * (pow(16,abs((i%2)-1)));
                break;
            case 'd':
            case 'D':
                values[i] = 13 * (pow(16,abs((i%2)-1)));
                break;
            case 'e':
            case 'E':
                values[i] = 14 * (pow(16,abs((i%2)-1)));
                break;
            case 'f':
            case 'F':
                values[i] = 15 * (pow(16,abs((i%2)-1)));
                break;
        }
    }
    int r = values[0] + values[1];
    printf("rojo %d /n",r);
    int g = values[2] + values[3];
    printf("verde %d /n",g);
    int b = values[4] + values[5];
    printf("azul %d /n",b);
    name ->set_pixel(name,0,r,g,b);
    name->refresh(name, 500);
    
}
