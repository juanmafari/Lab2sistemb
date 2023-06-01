#include "touchpadv1.h"


const char *TAG = "Touch pad";

QueueHandle_t que_touch = NULL;
typedef struct touch_msg {
    touch_pad_intr_mask_t intr_mask;
    uint32_t pad_num;
    uint32_t pad_status;
    uint32_t pad_val;
} touch_event_t;

#define TOUCH_BUTTON_NUM    6

const touch_pad_t button[TOUCH_BUTTON_NUM] = {
    TOUCH_PAD_NUM6,     // "PHOTO".
    TOUCH_PAD_NUM2,    // "PLAY/PAUSE".
    TOUCH_PAD_NUM5,     // "RECORD".
    TOUCH_PAD_NUM11,    // "NETWORK".
    TOUCH_PAD_NUM1,    // "VOL_UP".
    TOUCH_PAD_NUM3,    // "VOL_DOWN".
    // If this pad be touched, other pads no response.
};

/*
 * Touch threshold. The threshold determines the sensitivity of the touch.
 * This threshold is derived by testing changes in readings from different touch channels.
 * If (raw_data - benchmark) > benchmark * threshold, the pad be activated.
 * If (raw_data - benchmark) < benchmark * threshold, the pad be inactivated.
 */
const float button_threshold[TOUCH_BUTTON_NUM] = {
    0.2, // 20%.
    0.2, // 20%.
    0.2, // 20%.
    0.1, // 10%.
    0.2, // 10%.
    0.2, // 10%.
};

/*
  Handle an interrupt triggered when a pad is touched.
  Recognize what pad has been touched and save it in a table.
 */
void touchsensor_interrupt_cb(void *arg)
{
    int task_awoken = pdFALSE;
    touch_event_t evt;

    evt.intr_mask = touch_pad_read_intr_status_mask();
    evt.pad_status = touch_pad_get_status();
    evt.pad_num = touch_pad_get_current_meas_channel();

    xQueueSendFromISR(que_touch, &evt, &task_awoken);
    if (task_awoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void tp_example_set_thresholds(void)
{
    uint32_t touch_value;
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        //read benchmark value
        touch_pad_read_benchmark(button[i], &touch_value);
        //set interrupt threshold.
        touch_pad_set_thresh(button[i], touch_value * button_threshold[i]);
        ESP_LOGI(TAG, "touch pad [%d] base %"PRIu32", thresh %"PRIu32, \
                 button[i], touch_value, (uint32_t)(touch_value * button_threshold[i]));
    }
}


void touchsensor_filter_set(touch_filter_mode_t mode)
{
    /* Filter function */
    touch_filter_config_t filter_info = {
        .mode = mode,           // Test jitter and filter 1/4.
        .debounce_cnt = 1,      // 1 time count.
        .noise_thr = 0,         // 50%
        .jitter_step = 4,       // use for jitter mode.
        .smh_lvl = TOUCH_PAD_SMOOTH_IIR_2,
    };
    touch_pad_filter_set_config(&filter_info);
    touch_pad_filter_enable();
    ESP_LOGI(TAG, "touch pad filter init");
}

void tp_example_read_task(void *pvParameter)
{
    touch_event_t evt = {0};
    /* Wait touch sensor init done */
    vTaskDelay(50 / portTICK_PERIOD_MS);
    tp_example_set_thresholds();

    while (1) {
        int ret = xQueueReceive(que_touch, &evt, (TickType_t)portMAX_DELAY);
        if (ret != pdTRUE) {
            continue;
        }
        if (evt.intr_mask & TOUCH_PAD_INTR_MASK_ACTIVE) {
            if (evt.pad_num == button[2]) {
                ESP_LOGW(TAG, "TouchSensor [%"PRIu32"] be activated, red", evt.pad_num);
                red();
                }
            else if (evt.pad_num == button[1])
            {
               ESP_LOGW(TAG, "TouchSensor [%"PRIu32"] be activated, green", evt.pad_num);
                green(); 
            }
            else if (evt.pad_num == button[3])
            {
               ESP_LOGW(TAG, "TouchSensor [%"PRIu32"] be activated, blue", evt.pad_num);
                blue(); 
            }
            else if (evt.pad_num == button[0])
            {
               ESP_LOGW(TAG, "TouchSensor [%"PRIu32"] be activated, purple", evt.pad_num);
                magenta(); 
            }
            else if (evt.pad_num == button[4])
            {
               ESP_LOGW(TAG, "TouchSensor [%"PRIu32"] be activated, cyan", evt.pad_num);
                cyan(); 
            }
            else if (evt.pad_num == button[5])
            {
               ESP_LOGW(TAG, "TouchSensor [%"PRIu32"] be activated, yellow", evt.pad_num);
                yellow(); 
            }
        }
        if (evt.intr_mask & TOUCH_PAD_INTR_MASK_INACTIVE) {
            
                    ESP_LOGI(TAG, "TouchSensor [%"PRIu32"] be inactivated, status mask 0x%"PRIu32, evt.pad_num, evt.pad_status);
                    white();
                
            }
        }
        if (evt.intr_mask & TOUCH_PAD_INTR_MASK_SCAN_DONE) {
            ESP_LOGI(TAG, "The touch sensor group measurement is done [%"PRIu32"].", evt.pad_num);
        }
        if (evt.intr_mask & TOUCH_PAD_INTR_MASK_TIMEOUT) {
            /* Add your exception handling in here. */
            ESP_LOGI(TAG, "Touch sensor channel %"PRIu32" measure timeout. Skip this exception channel!!", evt.pad_num);
            touch_pad_timeout_resume(); // Point on the next channel to measure.
    }
}


void touchPadStart(void){
    
    if (que_touch == NULL) {
        que_touch = xQueueCreate(TOUCH_BUTTON_NUM, sizeof(touch_event_t));
    }
    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI(TAG, "Initializing touch pad");
    /* Initialize touch pad peripheral. */
    touch_pad_init();
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        touch_pad_config(button[i]);
    }



    /* Filter setting */
    touchsensor_filter_set(TOUCH_PAD_FILTER_IIR_16);
    touch_pad_timeout_set(true, SOC_TOUCH_PAD_THRESHOLD_MAX);
    /* Register touch interrupt ISR, enable intr type. */
    touch_pad_isr_register(touchsensor_interrupt_cb, NULL, TOUCH_PAD_INTR_MASK_ALL);
    /* If you have other touch algorithm, you can get the measured value after the `TOUCH_PAD_INTR_MASK_SCAN_DONE` interrupt is generated. */
    touch_pad_intr_enable(TOUCH_PAD_INTR_MASK_ACTIVE | TOUCH_PAD_INTR_MASK_INACTIVE | TOUCH_PAD_INTR_MASK_TIMEOUT);

    /* Enable touch sensor clock. Work mode is "timer trigger". */
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();

    // Start a task to show what pads have been touched
    xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 4096, NULL, 5, NULL);
}