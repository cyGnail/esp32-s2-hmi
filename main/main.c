#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "math.h"

#include "esp_log.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_spiffs.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ft5x06.h"
#include "lcd.h"
#include "lvgl.h"
#include "lv_2048.h"
#include "lv_2048_setting.h"

#define SENSITIVE 10
#define TAG "main"

#define LVGL_BUFFER_SIZE  (sizeof(lv_color_t) * (LV_HOR_RES_MAX * LV_VER_RES_MAX))
#define LVGL_INIT_PIXCNT  (LV_HOR_RES_MAX * LV_VER_RES_MAX)

/*!< The implemention of display function */
void IRAM_ATTR disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t len = (sizeof(uint16_t) * ((area->y2 - area->y1 + 1)*(area->x2 - area->x1 + 1)));

    lcd_set_index(area->x1, area->y1, area->x2, area->y2);
    lcd_write_data((uint8_t *)color_p, len);
    lv_disp_flush_ready(disp_drv);
}

bool IRAM_ATTR disp_input(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint16_t x = 0, y = 0;
    if (ft5x06_pos_read(&x, &y)) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    data->point.x = x;
    data->point.y = y;
    return false;
}

static void hmi_tick_task(void * arg)
{
    while(1) {
        lv_tick_inc(10);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

static void readRank(char c[10], FILE* fp)
{
    memset(c, '\0', 10);
    for (int i = 0;; i++)
    {
        int temp = fgetc(fp);
        if (temp == '\t')
            break;
        c[i] = temp;
    }
}

static void begin(lv_task_t * t)
{
    lv_2048_others();
}

void gui_task(void *arg)
{
    lv_init();

    /*!< Create GUI task */
    xTaskCreate(
        (TaskFunction_t)        hmi_tick_task,
        (const char * const)    "HMI Tick Task",
        (const uint32_t)        1 * 1024,
        (void * const)          NULL,
        (UBaseType_t)           10,
        (TaskHandle_t * const)  NULL);   
   
    
    /*Create a display buffer*/
    static lv_disp_buf_t disp_buf;
    static lv_color_t *lv_buf = NULL;

    lv_buf = (lv_color_t *)heap_caps_malloc(LVGL_BUFFER_SIZE, MALLOC_CAP_SPIRAM);

    lv_disp_buf_init(&disp_buf, lv_buf, NULL, LVGL_INIT_PIXCNT);

    /*Create a display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = disp_flush;
    lv_disp_drv_register(&disp_drv);

    /*!< Register input devcie */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = disp_input;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t* touchpad_indev = lv_indev_drv_register(&indev_drv);

    /*!< Run LVGL Demo */
    lv_task_t* task0 = lv_task_create(begin, 100, LV_TASK_PRIO_LOW, NULL);
    lv_task_once(task0);

    bool pressing = false;
    lv_indev_data_t touchpad_data;
    lv_point_t pre_point;
    int deltax, deltay;

    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    ESP_LOGI(TAG, "Reading rank.txt");

    // Open for reading hello.txt
    FILE* f = fopen("/spiffs/rank.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open rank.txt");
        return;
    }

    readRank(ranks[0], f);
    readRank(values[0], f);
    readRank(ranks[1], f);
    readRank(values[1], f);
    readRank(ranks[2], f);
    readRank(values[2], f);

    fclose(f);

    while (1)
    {
        lv_task_handler();
        vTaskDelay(10 / portTICK_RATE_MS);

        _lv_indev_read(touchpad_indev, &touchpad_data);
        if (touchpad_data.state == LV_INDEV_STATE_REL)
        { 
            pressing = false; 
            deltax = touchpad_data.point.x - pre_point.x;
            deltay = touchpad_data.point.y - pre_point.y;
            if (touchpad_data.point.x < 500 || pre_point.x < 500)
            {
                if (fabs(deltax) > SENSITIVE || fabs(deltay) > SENSITIVE)
                {
                    for (int i = 0; i < dimension; i++)
                        for (int j = 0; j < dimension; j++)
                            buffer[i + j * dimension] = gameArray[i + j * dimension];

                    if (fabs(deltax) > fabs(deltay))  //Horizontal
                    {
                        if (deltax > 0)  //Right
                        {
                            if (isGaming && !destroying)
                            {
                                RightShift();
                                lv_2048(dimension);
                            }
                        }
                        else if (isGaming && !destroying)  //Left
                        {
                            LeftShift();
                            lv_2048(dimension);
                        }
                    }
                    else if (deltay > 0)   //Vertical
                    {
                        if (isGaming && !destroying)  //Down
                        {
                            DownShift();
                            lv_2048(dimension);
                        }
                    }
                    else if (isGaming && !destroying)  //Up
                    {
                        UpShift();
                        lv_2048(dimension);
                    }

                    static const char* btns3[] = { "Record","Continue","" };
                    if (judgeLose())
                        msgbox_type_1(btns3, "You lose!\nWould you record your score and restart?");
                    else if (If_Win)
                        msgbox_type_1(btns3, "You win!\nWould you continue or record your score and restart?");
                }
            }
            pre_point.x = touchpad_data.point.x;
            pre_point.y = touchpad_data.point.y;
        }
        else if (touchpad_data.state == LV_INDEV_STATE_PR)
        { 
            if (!pressing)
            { 
                pre_point.x = touchpad_data.point.x;
                pre_point.y = touchpad_data.point.y;
                pressing = true;
            }
        }
    }
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

void app_main(void)
{
    lcd_init();
    ft5x06_init();
    xTaskCreate(gui_task, "gui_task", 20 * 1024, NULL, 10, NULL);
}
