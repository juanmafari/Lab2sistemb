#include <stdio.h>
#include "esp_log.h"
#include "driver/touch_pad.h"
#include "led.h"

#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"

char *TAG = "Touch pad";


typedef struct touch_msg {
    touch_pad_intr_mask_t intr_mask;
    uint32_t pad_num;
    uint32_t pad_status;
    uint32_t pad_val;
} touch_event_t;

void touchsensor_interrupt_cb(void *arg)
{
    
    touch_event_t evt;
    evt.intr_mask = touch_pad_read_intr_status_mask();
    evt.pad_status = touch_pad_get_status();
    evt.pad_num = touch_pad_get_current_meas_channel();
    if (evt.pad_num == TOUCH_PAD_NUM11) {
        white();
    }

    touch_pad_clear_status();
}



void setup_touchpad()
{
    touch_pad_init();

    // Configure touch pads and thresholds
    touch_pad_config(TOUCH_PAD_NUM7);
    touch_pad_set_thresh(TOUCH_PAD_NUM7, 100);

    touch_pad_config(TOUCH_PAD_NUM9);
    touch_pad_set_thresh(TOUCH_PAD_NUM9, 100);

    touch_pad_config(TOUCH_PAD_NUM11);
    touch_pad_set_thresh(TOUCH_PAD_NUM11, 500);

    touch_pad_config(TOUCH_PAD_NUM13);
    touch_pad_set_thresh(TOUCH_PAD_NUM13, 100);

    // Register touchpad interrupt handler
    touch_pad_isr_register(touchsensor_interrupt_cb, NULL, TOUCH_PAD_INTR_MASK_ALL);

    // Enable touchpad interrupt
    touch_pad_intr_enable(TOUCH_PAD_INTR_MASK_ACTIVE | TOUCH_PAD_INTR_MASK_INACTIVE | TOUCH_PAD_INTR_MASK_TIMEOUT);

    // Enable touchpad scanning
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();
}

void app_main(void)
{
    configure();
    red();
    // Set logging level to avoid termination
    esp_log_level_set(TAG, ESP_LOG_INFO);
    ESP_LOGI(TAG, "AAAAAAAAAAAA");
    // Initialize touchpad
    setup_touchpad();
}
