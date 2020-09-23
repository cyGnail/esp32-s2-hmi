#include "lv_2048.h"
#include "time.h"
#include "lvgl.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "esp_spiffs.h"
#include <sys/unistd.h>
#include <sys/stat.h>


lv_style_t style_0;
static lv_style_t style_2;
static lv_style_t style_4;
static lv_style_t style_8;
static lv_style_t style_16;
static lv_style_t style_32;
static lv_style_t style_64;
static lv_style_t style_128;
static lv_style_t style_256;
static lv_style_t style_512;
static lv_style_t style_1024;
static lv_style_t style_2048;
static lv_style_t style_4096;
static lv_style_t style_8192;
static lv_style_t style_16384;

static bool setFinish = false;

static int interval = 2;
int gameArray[64] = { 0 };     //global parameter
static int transmit[64] = { 0 };   //transmit matrix
static int k[8] = { 0 };
static int buffer[64] = { 0 };

int dimension = 4;            //global parameter
int winNumber = 2048;
int withdraw_chance = 3;
int destroy_chance = 3;
int score = 0;
bool If_Win = false;
bool loginFlag = false;
bool connectFlag = false;
bool isGaming = true;
bool destroying = false;
enum Difficulty difficulty = EASY;
static enum Difficulty temp = EASY;

static lv_obj_t* btn[64];
static lv_obj_t* label[64];

static lv_obj_t* label_score;
static lv_obj_t* btn1;
static lv_obj_t* btn2;
static lv_obj_t* btn3;
static lv_obj_t* btn4;

static lv_obj_t* btn_easy;
static lv_obj_t* btn_medium;
static lv_obj_t* btn_hard;

static lv_obj_t* slider;
static lv_obj_t* label0;   //dimension

static lv_obj_t* btn_setting;
static lv_obj_t* btn_cut;
static lv_obj_t* btn_wifi;
static lv_obj_t* btn_restart;
static lv_obj_t* btn_revoke;

static lv_obj_t* mbox_restart;
lv_style_t style_modal;

static int destroyNumber = -1;
static lv_obj_t* destroy_mask;

static lv_style_t style_arc;

static lv_obj_t* information;
static int NO = 0;

char ranks[3][10] = { "Nobody","Nobody","Nobody"};
char values[3][10] = { "0","0","0" };

char userKey[10] = "Visitor";

static void style_reset(lv_style_t* dst, lv_color_t color)
{
    lv_style_init(dst);
    lv_style_copy(dst, &style_0);
    lv_style_set_bg_color(dst, LV_STATE_DEFAULT, color);
}

static void style_init()
{
    /*Initialize styles*/
    lv_style_init(&style_0);

    /*Set radius and background color*/
    lv_style_set_radius(&style_0, LV_STATE_DEFAULT, 20);
    lv_style_set_bg_opa(&style_0, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_0, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    /*Set border styles*/
    lv_style_set_border_color(&style_0, LV_STATE_DEFAULT, LV_COLOR_SILVER);
    lv_style_set_border_color(&style_0, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_border_opa(&style_0, LV_STATE_DEFAULT, LV_OPA_70);
    lv_style_set_border_width(&style_0, LV_STATE_DEFAULT, 3);
    lv_style_set_border_width(&style_0, LV_STATE_PRESSED, 5);

    /*Set text styles*/
    lv_style_set_text_color(&style_0, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    /*Copy the first style and reset in another way*/
    style_reset(&style_2, LV_COLOR_GRAY);
    style_reset(&style_4, LV_COLOR_NAVY);
    style_reset(&style_8, LV_COLOR_PURPLE);
    style_reset(&style_16, LV_COLOR_BLUE);
    style_reset(&style_32, LV_COLOR_GREEN);
    style_reset(&style_64, LV_COLOR_OLIVE);
    style_reset(&style_128, LV_COLOR_CYAN);
    style_reset(&style_256, LV_COLOR_LIME);
    style_reset(&style_512, LV_COLOR_YELLOW);
    style_reset(&style_1024, LV_COLOR_ORANGE);
    style_reset(&style_2048, LV_COLOR_MAGENTA);
    style_reset(&style_4096, LV_COLOR_RED);
    style_reset(&style_8192, LV_COLOR_MAROON);
    style_reset(&style_16384, LV_COLOR_BLACK);

    setFinish = true;

    /*Other styles*/
    lv_style_init(&style_modal);
    lv_style_set_bg_color(&style_modal, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_style_init(&style_arc);
}

static void setNewStyle(lv_obj_t* btn, int value)
{
    lv_obj_reset_style_list(btn, LV_BTN_PART_MAIN);

    switch (value)
    {
    case 2:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_2);
        break;
    case 4:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_4);
        break;
    case 8:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_8);
        break;
    case 16:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_16);
        break;
    case 32:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_32);
        break;
    case 64:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_64);
        break;
    case 128:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_128);
        break;
    case 256:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_256);
        break;
    case 512:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_512);
        break;
    case 1024:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_1024);
        break;
    case 2048:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_2048);
        break;
    case 4096:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_4096);
        break;
    case 8192:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_8192);
        break;
    case 16384:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_16384);
        break;
    default:
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_0);
        break;
    }
}

static void destroyMe(lv_obj_t* obj, lv_event_t evt)
{
    if (evt == LV_EVENT_CLICKED)
    {
        if (destroying)
        {
            for (int i = 0; i < dimension; i++)
                for (int j = 0; j < dimension; j++)
                    if (obj == btn[i + j * dimension])
                        destroyNumber = i + j * dimension;

            if (gameArray[destroyNumber] != 0)
            {
                static const char* btns6[] = { "Destroy","Cancel","" };
                msgbox_type_1(btns6, "Destroy this one?");
            }
        }
    }
}

static void info_anim(void* bg, lv_anim_value_t v)
{
    lv_obj_set_x(bg, v);
}

void lv_2048(int dimension)
{
    int squareSize = 442 / dimension - 2;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (!btn[i + j * 8])
            {
                btn[i + j * 8] = lv_btn_create(lv_scr_act(), NULL);
                lv_obj_set_event_cb(btn[i + j * 8], destroyMe);
            }
            lv_obj_set_hidden(btn[i + j * 8], true);
        }

    /*Generate game matrix*/
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
        {
            if (label_score)
                lv_label_set_text_fmt(label_score, "YOUR SCORE: %d", score);

            lv_obj_set_hidden(btn[i + j * dimension], false);

            if (!label[i + j * dimension])
                label[i + j * dimension] = lv_label_create(btn[i + j * dimension], NULL);

            /*Set value in text*/
            lv_label_set_text_fmt(label[i + j * dimension], "%d", gameArray[i + j * dimension]);
            if (gameArray[i + j * dimension] == 0)
                lv_obj_set_hidden(label[i + j * dimension], true);
            else
                lv_obj_set_hidden(label[i + j * dimension], false);

            lv_label_set_align(label[i + j * dimension], LV_LABEL_ALIGN_CENTER);

            /*Set different position and size*/
            lv_obj_set_pos(btn[i + j * dimension], 40 + j * (squareSize + interval), 20 + i * (squareSize + interval));
            lv_obj_set_size(btn[i + j * dimension], squareSize, squareSize);
            lv_obj_set_click(btn[i + j * dimension], true);

            /*Renew different styles*/
            setNewStyle(btn[i + j * dimension], gameArray[i + j * dimension]);
        }

    if (!information)
    {
        lv_obj_set_style_local_bg_color(lv_scr_act(), LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_style_local_text_color(lv_scr_act(), LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        information = lv_label_create(lv_scr_act(), NULL);
        lv_obj_align(information, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 30, 0);
    }

    if (!loginFlag && connectFlag)
        lv_label_set_text(information, "Visitor mode: Haven't logged in. WiFi connected.");
    else if (!loginFlag && !connectFlag)
        lv_label_set_text(information, "Visitor mode: Haven't logged in. WiFi unconnected.");
    else if (loginFlag && connectFlag)
        lv_label_set_text(information, "Full mode: Enjoy all functions. WiFi connected.");
    else if (loginFlag && !connectFlag)
        lv_label_set_text(information, "Full mode: Enjoy all functions. WiFi unconnected.");

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, information);
    lv_anim_set_time(&a, 5000);

    if (lv_obj_get_x(information) == 30)
    {
        lv_anim_set_values(&a, 30, 380);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)info_anim);
    }
    if (lv_obj_get_x(information) == 380)
    {
        lv_anim_set_values(&a, 380, 30);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)info_anim);
    }
    lv_anim_start(&a);
}

static void opa_anim(void* bg, lv_anim_value_t v)
{
    lv_obj_set_style_local_bg_opa(bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, v);
}

static void arc_anim(void* bg, lv_anim_value_t v)
{
    lv_arc_set_angles(bg, 0, v);
    lv_obj_clean(bg);
    lv_obj_t* label = lv_label_create(bg, NULL);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, -13, 0);
    lv_label_set_text_fmt(label, "%d/100", v * 100 / 360);

    if (lv_arc_get_angle_end(bg) == 360)
    {
        lv_msgbox_set_text(mbox_restart, "Connected!");
        connectFlag = true;

        static const char* btns2[] = { "Ok","" };
        lv_msgbox_add_btns(mbox_restart, btns2);

        lv_2048(dimension);
    }
}

static void mbox_restart_event_cb(lv_obj_t* obj, lv_event_t evt)
{
    if (evt == LV_EVENT_DELETE && obj == mbox_restart) {
        /* Delete the parent modal background */
        lv_obj_del_async(lv_obj_get_parent(mbox_restart));
        mbox_restart = NULL; /* happens before object is actually deleted! */

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Record"))
        {
            static const char* btns[] = { "Ok","" };
            switch (NO)
            {
            case 1:
                msgbox_type_1(btns, "You are NO.1!\nCongratulations!"); break;
            case 2:
                msgbox_type_1(btns, "You are NO.2!\nCongratulations!"); break;
            case 3:
                msgbox_type_1(btns, "You are NO.3!\nCongratulations!"); break;
            default:
                msgbox_type_1(btns, "Sorry!\nYou are not in the rankings!"); break;
            }
        }

        isGaming = true;
    }
    else if (evt == LV_EVENT_VALUE_CHANGED) {
        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Restart"))
        {
            dimension = lv_slider_get_value(slider);

            difficulty = temp;
            lv_obj_reset_style_list(btn_easy, LV_BTN_PART_MAIN);
            lv_obj_add_style(btn_easy, LV_BTN_PART_MAIN, &style_2);
            lv_obj_reset_style_list(btn_medium, LV_BTN_PART_MAIN);
            lv_obj_add_style(btn_medium, LV_BTN_PART_MAIN, &style_2);
            lv_obj_reset_style_list(btn_hard, LV_BTN_PART_MAIN);
            lv_obj_add_style(btn_hard, LV_BTN_PART_MAIN, &style_2);
            switch (temp)
            {
            case EASY:
                lv_obj_reset_style_list(btn_easy, LV_BTN_PART_MAIN);
                lv_obj_add_style(btn_easy, LV_BTN_PART_MAIN, &style_4);
                break;
            case MEDIUM:
                lv_obj_reset_style_list(btn_medium, LV_BTN_PART_MAIN);
                lv_obj_add_style(btn_medium, LV_BTN_PART_MAIN, &style_4);
                break;
            case HARD:
                lv_obj_reset_style_list(btn_hard, LV_BTN_PART_MAIN);
                lv_obj_add_style(btn_hard, LV_BTN_PART_MAIN, &style_4);
                break;
            }

            initialize();
            lv_2048(dimension);
        }
        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Continue"))
        {
            If_Win = false;
            winNumber = 65535;
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Log in"))
        {
            temp = difficulty;
            lv_setting_win();
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Cancel"))
        {
            temp = difficulty;
            lv_slider_set_value(slider, dimension, 10);
            lv_label_set_text_fmt(label0, "Dimension: %d", dimension);
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Withdraw"))
        {
            score -= 1024 * (4 - withdraw_chance);
            if (withdraw_chance > 0)
                withdraw_chance--;

            for (int i = 0; i < dimension; i++)
                for (int j = 0; j < dimension; j++)
                    gameArray[i + j * dimension] = buffer[i + j * dimension];

            lv_2048(dimension);
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Destroy"))
        {
            if (!destroy_mask)
            {
                destroy_mask = lv_obj_create(lv_scr_act(), NULL);
                lv_obj_reset_style_list(destroy_mask, LV_OBJ_PART_MAIN);
                lv_obj_add_style(destroy_mask, LV_OBJ_PART_MAIN, &style_modal);
                lv_obj_set_pos(destroy_mask, LV_HOR_RES - 300, 0);
                lv_obj_set_size(destroy_mask, 300, LV_VER_RES);
            }
            lv_obj_set_hidden(destroy_mask, false);

            if (!destroying)
            {
                destroying = true;
                score -= 1024 * (4 - destroy_chance);
                if (destroy_chance > 0)
                    destroy_chance--;

                lv_anim_t a;
                lv_anim_init(&a);
                lv_anim_set_var(&a, destroy_mask);
                lv_anim_set_time(&a, 500);
                lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_50);
                lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)opa_anim);
                lv_anim_start(&a);
            }
            else
            {
                gameArray[destroyNumber] = 0;
                destroying = false;
                if (destroy_mask)
                    lv_obj_set_hidden(destroy_mask, true);
            }

            lv_2048(dimension);
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Disconnect"))
        {
            connectFlag = false;

            lv_2048(dimension);
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Record"))
        {
            if (score > atoi(values[2]))
            {
                if (score > atoi(values[1]))
                {
                    if (score > atoi(values[0]))
                    {
                        strcpy(values[2], values[1]);
                        strcpy(ranks[2], ranks[1]);

                        strcpy(values[1], values[0]);
                        strcpy(ranks[1], ranks[0]);

                        itoa(score, values[0], 10);
                        strcpy(ranks[1], userKey);

                        NO = 1;
                    }
                    else
                    {
                        strcpy(values[2], values[1]);
                        strcpy(ranks[2], ranks[1]);
                        itoa(score, values[1], 10);
                        strcpy(ranks[1], userKey);

                        NO = 2;
                    }
                }
                else
                {
                    itoa(score, values[2], 10);
                    strcpy(ranks[1], userKey);

                    NO = 3;
                }

                FILE* f = fopen("/spiffs/rank.txt", "w");

                for (int i = 0; i < 3; i++)
                {
                    fputs(ranks[i], f);
                    fputs("\t", f);
                    fputs(values[i], f);
                    fputs("\t", f);
                }

                fclose(f);
            }
            else NO = 0;

            initialize();
            lv_2048(dimension);
        }

        /* A button was clicked */
        lv_msgbox_start_auto_close(mbox_restart, 0);
    }
}

void msgbox_type_1(const char* key[3], char* info)
{
    /* Create a full-screen background */
    isGaming = false;

    /* Create a base object for the modal background */
    lv_obj_t* obj = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_reset_style_list(obj, LV_OBJ_PART_MAIN);
    lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style_modal);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);

    /* Create the message box as a child of the modal background */
    mbox_restart = lv_msgbox_create(obj, NULL);
    if (key)lv_msgbox_add_btns(mbox_restart, key);
    lv_msgbox_set_text(mbox_restart, info);
    if (key) lv_obj_align(mbox_restart, NULL, LV_ALIGN_CENTER, 0, 0);
    else lv_obj_align(mbox_restart, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_obj_set_event_cb(mbox_restart, mbox_restart_event_cb);

    /* Fade the message box in with an animation */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_time(&a, 500);
    lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_50);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)opa_anim);
    lv_anim_start(&a);
}

static void btn_restart_event_cb(lv_obj_t* btn, lv_event_t evt)
{
    static const char* btns2[] = { "Restart","Cancel","" };
    if (evt == LV_EVENT_CLICKED)
        msgbox_type_1(btns2, "Are you sure to restart?\nYour data will be cleared!");
}

static void btn_setting_event_cb(lv_obj_t* btn, lv_event_t evt)
{
    if (evt == LV_EVENT_CLICKED)
        lv_setting_win();
}

static void btn_revoke_event_cb(lv_obj_t* btn, lv_event_t evt)
{
    if (evt == LV_EVENT_CLICKED)
    {
        static const char* btns5[] = { "Log in","Cancel","" };
        static const char* btns6[] = { "Withdraw","Cancel","" };
        static const char* btns7[] = { "Ok","" };

        char *info;
        switch (withdraw_chance)
        {
        case 1:info = "Are you sure to withdraw?\n1 times left.\nYou will pay lots of points!"; break;
        case 2:info = "Are you sure to withdraw?\n2 times left.\nYou will pay lots of points!"; break;
        case 3:info = "Are you sure to withdraw?\n3 times left.\nYou will pay lots of points!"; break;
        default:info = "You have no chance unless you restart!"; break;
        }

        if (!loginFlag)
            msgbox_type_1(btns5, "This function is unaccessible to you unless you log in!\nPlease log in in settings first.");
        else if (withdraw_chance > 0)
            msgbox_type_1(btns6, info);
        else
            msgbox_type_1(btns7, info);
    }
}

static void btn_cut_event_cb(lv_obj_t* btn, lv_event_t evt)
{
    if (evt == LV_EVENT_CLICKED)
    {
        static const char* btns5[] = { "Log in","Cancel","" };
        static const char* btns6[] = { "Destroy","Cancel","" };
        static const char* btns7[] = { "Ok","" };

        if (countZero() >= dimension * dimension - 1)
        {
            msgbox_type_1(btns7, "No cube can be destroyed now!");
        }
        else
        {
            char* info;
            switch (destroy_chance)
            {
            case 1:info = "Are you sure to destroy a cube?\n1 times left.\nYou will pay lots of points!"; break;
            case 2:info = "Are you sure to destroy a cube?\n2 times left.\nYou will pay lots of points!"; break;
            case 3:info = "Are you sure to destroy a cube?\n3 times left.\nYou will pay lots of points!"; break;
            default:info = "You have no chance unless you restart!"; break;
            }

            if (!loginFlag)
                msgbox_type_1(btns5, "This function is unaccessible to you unless you log in!\nPlease log in in settings first.");
            else if (withdraw_chance > 0)
                msgbox_type_1(btns6, info);
            else
                msgbox_type_1(btns7, info);
        }
    }
}

static void btn_wifi_event_cb(lv_obj_t* btn, lv_event_t evt)
{
    if (evt == LV_EVENT_CLICKED)
    {
        if (!connectFlag)
        {
            msgbox_type_1(NULL, "Connecting..");

            if (mbox_restart)
            {
                lv_obj_t* arc = lv_arc_create(mbox_restart, NULL);
                lv_arc_set_start_angle(arc, 0);
                lv_arc_set_end_angle(arc, 360);
                lv_obj_set_size(arc, 150, 150);
                lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, 0);
                lv_obj_reset_style_list(arc, LV_ARC_PART_BG);

                lv_anim_t a;
                lv_anim_init(&a);
                lv_anim_set_var(&a, arc);
                lv_anim_set_time(&a, 2000);
                lv_anim_set_values(&a, 0, 360);
                lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)arc_anim);
                lv_anim_start(&a);
            }
        }
        else
        {
            static const char* btns2[] = { "Ok","Disconnect","" };
            msgbox_type_1(btns2, "You have already connected.");
        }
    }
}

static void btn_difficulty_event_cb(lv_obj_t* btn, lv_event_t evt)
{
    if (evt == LV_EVENT_CLICKED)
    {
        if (btn == btn_easy)
            temp = EASY;
        if (btn == btn_medium)
            temp = MEDIUM;
        if (btn == btn_hard)
            temp = HARD;
 
        if (difficulty != temp)
        {
            static const char* btns5[] = { "Log in","Cancel","" };
            static const char* btns6[] = { "Restart","Cancel","" };
            if (!loginFlag)
                msgbox_type_1(btns5, "You have no privilegde to change difficulty unless you log in!\nPlease log in in settings first.");
            else
                msgbox_type_1(btns6, "You must restart if the difficulty is changed!\nAre you sure?");
        }
    }
}

static void btn_event_cb(lv_obj_t* btn, lv_event_t event)
{
    static const char* btns3[] = { "Record","Continue","" };

    if (event == LV_EVENT_CLICKED) {
        for(int i=0;i<dimension;i++)
            for (int j = 0; j < dimension; j++)            
                buffer[i + j * dimension] = gameArray[i + j * dimension];

        if (btn == btn1)
            UpShift();
        if (btn == btn2)
            DownShift();
        if (btn == btn3)
            LeftShift();
        if (btn == btn4)
            RightShift();
        if (judgeChange())
        {
            lv_2048(dimension);
        }
        if (judgeLose())
            msgbox_type_1(btns3, "You lose!\nWould you record your score and restart?");
        else if (If_Win)
            msgbox_type_1(btns3, "You win!\nWould you continue or record your score and restart?");
    }
}

static void slider_event_cb(lv_obj_t* slider, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        static const char* btns4[] = { "Restart","Cancel","" };
        static const char* btns5[] = { "Log in","Cancel","" };

        if (!loginFlag)
        {
            lv_slider_set_value(slider, dimension, 10);
            msgbox_type_1(btns5, "You have no privilegde to change dimension unless you log in!\nPlease log in in settings first.");
        }
        else
        {
            /*Refresh the text*/
            lv_label_set_text_fmt(label0, "Dimension: %d", lv_slider_get_value(slider));
            msgbox_type_1(btns4, "You must restart if the dimension is changed!\nAre you sure?");
        }
    }
}

/*Initialize other components*/
void lv_2048_others()
{
    if (!setFinish)
        style_init();

    initialize();

    btn_setting = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn_setting, 709, 330);
    lv_obj_set_size(btn_setting, 50, 50);
    lv_obj_t* label_setting = lv_label_create(btn_setting, NULL);
    lv_label_set_text(label_setting, LV_SYMBOL_SETTINGS);

    btn_wifi = lv_btn_create(lv_scr_act(), btn_setting);
    lv_obj_set_pos(btn_wifi, 520, 400);
    lv_obj_t* label_wifi = lv_label_create(btn_wifi, NULL);
    lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);

    btn_cut = lv_btn_create(lv_scr_act(), btn_setting);
    lv_obj_set_pos(btn_cut, 646, 400);
    lv_obj_t* label_cut = lv_label_create(btn_cut, NULL);
    lv_label_set_text(label_cut, LV_SYMBOL_CUT);

    btn_restart = lv_btn_create(lv_scr_act(), btn_setting);
    lv_obj_set_pos(btn_restart, 583, 400);
    lv_obj_t* label_restart = lv_label_create(btn_restart, NULL);
    lv_label_set_text(label_restart, LV_SYMBOL_REFRESH);

    btn_revoke = lv_btn_create(lv_scr_act(), btn_setting);
    lv_obj_set_pos(btn_revoke, 709, 400);
    lv_obj_t* label_revoke = lv_label_create(btn_revoke, NULL);
    lv_label_set_text(label_revoke, LV_SYMBOL_PREV);

    lv_obj_set_event_cb(btn_setting, btn_setting_event_cb);
    lv_obj_set_event_cb(btn_restart, btn_restart_event_cb);
    lv_obj_set_event_cb(btn_revoke, btn_revoke_event_cb);
    lv_obj_set_event_cb(btn_cut, btn_cut_event_cb);
    lv_obj_set_event_cb(btn_wifi, btn_wifi_event_cb);

    /*Up*/
    btn1 = lv_btn_create(lv_scr_act(), NULL);     
    lv_obj_set_pos(btn1, 580, 40);                            
    lv_obj_set_size(btn1, 120, 50);                         
    lv_obj_set_event_cb(btn1, btn_event_cb);                 

    lv_obj_t* label1 = lv_label_create(btn1, NULL);         
    lv_label_set_text(label1, LV_SYMBOL_UP);

    /*Down*/
    btn2 = lv_btn_create(lv_scr_act(), btn1);
    lv_obj_set_pos(btn2, 580, 140);

    lv_obj_t* label2 = lv_label_create(btn2, NULL);
    lv_label_set_text(label2, LV_SYMBOL_DOWN);

    /*Right*/
    btn3 = lv_btn_create(lv_scr_act(), btn1);
    lv_obj_set_pos(btn3, 520, 60);

    lv_obj_t* label3 = lv_label_create(btn3, NULL);
    lv_obj_set_size(btn3, 50, 120);
    lv_label_set_text(label3, LV_SYMBOL_LEFT);

    /*Left*/
    btn4 = lv_btn_create(lv_scr_act(), btn3);
    lv_obj_set_pos(btn4, 710, 60);

    lv_obj_t* label4 = lv_label_create(btn4, NULL);
    lv_label_set_text(label4, LV_SYMBOL_RIGHT);

    label_score = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text_fmt(label_score, "%d", score);
    lv_label_set_align(label_score, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_pos(label_score, 570, 220);

    /*Difficulty*/
    btn_easy = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn_easy, 515, 260);
    lv_obj_set_size(btn_easy, 80, 40);

    lv_obj_t* label_easy = lv_label_create(btn_easy, NULL);
    lv_label_set_text(label_easy, "EASY");

    lv_obj_reset_style_list(btn_easy, LV_BTN_PART_MAIN);
    lv_obj_add_style(btn_easy, LV_BTN_PART_MAIN, &style_2);

    lv_obj_set_event_cb(btn_easy, btn_difficulty_event_cb);

    btn_medium = lv_btn_create(lv_scr_act(), btn_easy);
    lv_obj_set_pos(btn_medium, 600, 260);
    lv_obj_t* label_medium = lv_label_create(btn_medium, NULL);
    lv_label_set_text(label_medium, "MEDIUM");

    btn_hard = lv_btn_create(lv_scr_act(), btn_easy);
    lv_obj_set_pos(btn_hard, 685, 260);
    lv_obj_t* label_hard = lv_label_create(btn_hard, NULL);
    lv_label_set_text(label_hard, "HARD");

    lv_obj_reset_style_list(btn_easy, LV_BTN_PART_MAIN);
    lv_obj_add_style(btn_easy, LV_BTN_PART_MAIN, &style_4);

    /*Reset dimension*/
    slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_pos(slider, 520, 360);
    lv_obj_set_width(slider, 150);                        /*Set the width*/
    lv_slider_set_range(slider, 2, 8);
    lv_slider_set_value(slider, 4, 10);
    lv_obj_set_event_cb(slider, slider_event_cb);         /*Assign an event function*/

    /* Create a label below the slider */
    label0 = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_pos(label0, 520,330);
    lv_label_set_text(label0, "Dimension: 4");
    lv_obj_set_auto_realign(slider, true);                          /*To keep center alignment when the width of the text changes*/

    lv_2048(dimension);
}

void initialize()
{
    winNumber = 2048;
    If_Win = false;
    score = 0;
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            gameArray[i + j * dimension] = 0;

    withdraw_chance = 3;
    destroy_chance = 3;
    Generate(4);
    Generate(2);
}

/*Copy the matrix to the transmit matrix*/
void trans()
{
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
        {
            transmit[i + j * dimension] = gameArray[i + j * dimension];
            gameArray[i + j * dimension] = 0;
        }
}

/*Judge if the matrix has been changed*/
bool judgeChange()
{
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            if (transmit[i + j * dimension] != gameArray[i + j * dimension])
                return true;
    return false;
}

void RightMove()
{
    for (int m = 0; m < dimension; m++) k[m] = 0;
    trans();
    for (int i = 0; i < dimension; i++)
        for (int j = (dimension - 1); j >= 0; j--)
        {
            if (transmit[i + j * dimension] != 0)
            {
                gameArray[i + (dimension - 1 - k[i]) * dimension] = transmit[i + j * dimension];
                k[i]++;
            }
        }
}

void LeftMove()
{
    for (int m = 0; m < dimension; m++) k[m] = 0;
    trans();
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
        {
            if (transmit[i + j * dimension] != 0)
            {
                gameArray[i + k[i] * dimension] = transmit[i + j * dimension];
                k[i]++;
            }
        }
}

void DownMove()
{
    for (int m = 0; m < dimension; m++) k[m] = 0;
    trans();
    for (int j = 0; j < dimension; j++)
        for (int i = (dimension - 1); i >= 0; i--)
        {
            if (transmit[i + j * dimension] != 0)
            {
                gameArray[dimension - 1 - k[j] + j * dimension] = transmit[i + j * dimension];
                k[j]++;
            }
        }
}

void UpMove()
{
    for (int m = 0; m < dimension; m++) k[m] = 0;
    trans();
    for (int j = 0; j < dimension; j++)
        for (int i = 0; i < dimension; i++)
        {
            if (transmit[i + j * dimension] != 0)
            {
                gameArray[k[j] + j * dimension] = transmit[i + j * dimension];
                k[j]++;
            }
        }
}

void RightShift()
{
    RightMove();
    for (int j = (dimension - 1); j > 0; j--)
        for (int i = 0; i < dimension; i++)
        {
            if (gameArray[i + j * dimension] == (gameArray[i + (j - 1) * dimension]) && (gameArray[i + j * dimension] != 0))
            {
                gameArray[i + j * dimension] *= 2;
                gameArray[i + (j - 1) * dimension] = 0;
                score += gameArray[i + j * dimension];
                if (gameArray[i + j * dimension] == winNumber)
                {
                    If_Win = true;
                    break;
                }
            }
            if (If_Win) break;
        }
    if (If_Win) return;

    bool temp = judgeChange();

    RightMove();

    if (temp) GenerateRandomly();
}

void LeftShift()
{
    LeftMove();
    for (int j = 0; j < (dimension - 1); j++)
        for (int i = 0; i < dimension; i++)
        {
            if (gameArray[i + j * dimension] == (gameArray[i + (j + 1) * dimension]) && (gameArray[i + j * dimension] != 0))
            {
                gameArray[i + j * dimension] *= 2;
                gameArray[i + (j + 1) * dimension] = 0;
                score += gameArray[i + j * dimension];
                if (gameArray[i + j * dimension] == winNumber)
                {
                    If_Win = true;
                    break;
                }
            }
            if (If_Win) break;
        }                                                          
    if (If_Win) return;                                          

    bool temp = judgeChange();

    LeftMove();                                                

    if (temp) GenerateRandomly();
}

void DownShift()
{
    DownMove();
    for (int i = (dimension - 1); i > 0; i--)
        for (int j = 0; j < dimension; j++)
        {
            if (gameArray[i + j * dimension] == (gameArray[i - 1 + j * dimension]) && (gameArray[i + j * dimension] != 0))
            {
                gameArray[i + j * dimension] *= 2;
                gameArray[i - 1 + j * dimension] = 0;
                score += gameArray[i + j * dimension];
                if (gameArray[i + j * dimension] == winNumber)
                {
                    If_Win = true;
                    break;
                }
            }
            if (If_Win) break;
        }
    if (If_Win) return;

    bool temp = judgeChange();

    DownMove();

    if (temp) GenerateRandomly();
}

void UpShift()
{
    UpMove();
    for (int i = 0; i < (dimension - 1); i++)
        for (int j = 0; j < dimension; j++)
        {
            if (gameArray[i + j * dimension] == (gameArray[i + 1 + j * dimension]) && (gameArray[i + j * dimension] != 0))
            {
                gameArray[i + j * dimension] *= 2;
                gameArray[i + 1 + j * dimension] = 0;
                score += gameArray[i + j * dimension];
                if (gameArray[i + j * dimension] == winNumber)
                {
                    If_Win = true;
                    break;
                }
            }
            if (If_Win) break;
        }
    if (If_Win) return;

    bool temp = judgeChange();

    UpMove();

    if (temp) GenerateRandomly();
}

/*Get the number of empty rooms*/
int countZero()      
{
    int count = 0;
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
            if (gameArray[i + j * dimension] == 0) count++;
    return count;
}

/*Gnenerate numbers randomly*/
void Generate(int n)
{
    if (countZero() != 0)
    {
        srand((unsigned)time(NULL));
        int gi = rand() % (countZero()) + 1;
        int count = 0;
        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                if (gameArray[i + j * dimension] == 0) count++;
                if (count == gi)
                {
                    gameArray[i + j * dimension] = n;
                    break;
                }
            }
            if (count == gi) break;
        }
    }
}

void GenerateRandomly()
{
    srand((unsigned)time(NULL));
    int gi = rand() % (10) + 1;
    switch (difficulty)
    {
    case EASY:
        Generate(2);
        break;
    case MEDIUM:
        if (gi < 6)
            Generate(4);
        else
            Generate(2);
        break;
    case HARD:
        if (gi < 6)
            Generate(4);
        else if (gi < 8)
            Generate(2);
        else Generate(8);
        break;
    }
}

/*Judge if lost*/
bool judgeLose()
{
    if (!If_Win && countZero() == 0)
    {
        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                if (j < (dimension - 1) && (gameArray[i + j * dimension] == gameArray[i + (j + 1) * dimension])) return false;
                if (i < (dimension - 1) && (gameArray[i + j * dimension] == gameArray[i + 1 + j * dimension])) return false;
            }
        }
        return true;
    }
    return false;
}
