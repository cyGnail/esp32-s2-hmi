#include "lv_2048_setting.h"
#include "lvgl.h"
#include "lv_2048.h"

static lv_obj_t* logbtn;
static lv_obj_t* rankbtn;
static lv_obj_t* oobtn;
static lv_obj_t* btn_in;
static lv_obj_t* btn_cancel;
static lv_obj_t* btn_out;

static lv_obj_t* key;
static lv_obj_t* password;

static lv_obj_t* kb;
static lv_obj_t* obj;

static lv_obj_t* mbox;

bool darkFlag = false;

static void opa_anim(void* bg, lv_anim_value_t v)
{
    lv_obj_set_style_local_bg_opa(bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, v);
}

static void mbox_event_cb(lv_obj_t* obj, lv_event_t evt)
{
    if (evt == LV_EVENT_DELETE && obj == mbox) {
        /* Delete the parent modal background */
        lv_obj_del_async(lv_obj_get_parent(mbox));
        mbox = NULL; /* happens before object is actually deleted! */
    }
    else if (evt == LV_EVENT_VALUE_CHANGED) {
        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Log out"))
        {
            loginFlag = false;
            lv_textarea_set_text(key, "");
            lv_textarea_set_text(password, "");

            lv_2048(dimension);
        }

        if (!strcmp(lv_msgbox_get_active_btn_text(obj), "Back"))
        {
            isGaming = true;
            lv_obj_set_hidden(win_login, true);
            lv_2048(dimension);
        }

        /* A button was clicked */
        lv_msgbox_start_auto_close(mbox, 0);
    }
}

static void btn_event_cb(lv_obj_t* btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (btn != rankbtn && btn != oobtn)
        {
            isGaming = true;
            lv_obj_set_hidden(win, true);
            if (btn == logbtn)
            {
                isGaming = false;
                lv_login_win();
            }
        }
        else
        {
            static const char* btns4[] = { "Ok" ,"" };
            lv_obj_t* obj = lv_obj_create(lv_scr_act(), NULL);
            lv_obj_reset_style_list(obj, LV_OBJ_PART_MAIN);
            lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style_modal);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);

            mbox = lv_msgbox_create(obj, NULL);
            lv_obj_set_event_cb(mbox, mbox_event_cb);

            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_time(&a, 500);
            lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_50);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)opa_anim);
            lv_anim_start(&a);

            if (btn == oobtn)
            {
                lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, -50);
                lv_msgbox_set_text(mbox, "Copyright @2020\n\nSEU\n\n040174\nLYC QB QDH");
                lv_msgbox_add_btns(mbox, btns4);
            }
            else
            {
                lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, -205);
                lv_msgbox_set_text(mbox, "Ranking");

                lv_obj_t* table = lv_table_create(mbox, NULL);
                lv_table_set_col_cnt(table, 2);
                lv_table_set_row_cnt(table, 4);
                lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);

                /*Make the cells of the first row center aligned */
                lv_table_set_cell_align(table, 0, 0, LV_LABEL_ALIGN_LEFT);
                lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_RIGHT);

                /*Align the price values to the right in the 2nd column*/
                lv_table_set_cell_align(table, 1, 1, LV_LABEL_ALIGN_RIGHT);
                lv_table_set_cell_align(table, 2, 1, LV_LABEL_ALIGN_RIGHT);
                lv_table_set_cell_align(table, 3, 1, LV_LABEL_ALIGN_RIGHT);

                lv_table_set_cell_type(table, 0, 0, 2);
                lv_table_set_cell_type(table, 0, 1, 2);


                /*Fill the first column*/
                lv_table_set_cell_value(table, 0, 0, "User Key");
                lv_table_set_cell_value(table, 1, 0, ranks[0]);
                lv_table_set_cell_value(table, 2, 0, ranks[1]);
                lv_table_set_cell_value(table, 3, 0, ranks[2]);

                /*Fill the second column*/
                lv_table_set_cell_value(table, 0, 1, "Score");
                lv_table_set_cell_value(table, 1, 1, values[0]);
                lv_table_set_cell_value(table, 2, 1, values[1]);
                lv_table_set_cell_value(table, 3, 1, values[2]);

                lv_msgbox_add_btns(mbox, btns4);
            }
        }
    }
}

static void switch_event_handler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        if (lv_switch_get_state(obj))
        {
            darkFlag = true;

            lv_obj_set_style_local_bg_color(win, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_set_style_local_text_color(win, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);

            lv_obj_set_style_local_bg_color(lv_scr_act(), LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_style_set_bg_color(&style_0, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_set_style_local_text_color(lv_scr_act(), LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);

            if (win_login)
            {
                lv_obj_set_style_local_bg_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
                lv_obj_set_style_local_text_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
            }
        }
        else
        {
            darkFlag = false;

            lv_obj_set_style_local_bg_color(win, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
            lv_obj_set_style_local_text_color(win, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);

            lv_obj_set_style_local_bg_color(lv_scr_act(), LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
            lv_style_set_bg_color(&style_0, LV_STATE_DEFAULT, LV_COLOR_WHITE);
            lv_obj_set_style_local_text_color(lv_scr_act(), LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);

            if (win_login)
            {
                lv_obj_set_style_local_bg_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
                lv_obj_set_style_local_text_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            }
        }
    }
}

void lv_setting_win()
{
    if (!win)
    {
        /*Create a window*/
        win = lv_win_create(lv_scr_act(), NULL);
        lv_win_set_title(win, "Setting");                        /*Set the title*/

        lv_obj_set_style_local_bg_color(win, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_style_local_text_color(win, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);

     /*Add some dummy content*/
        lv_obj_t* btn1 = lv_btn_create(win, NULL);
        lv_obj_set_pos(btn1, 200, 340);
        lv_obj_set_size(btn1, 120, 50);
        lv_obj_set_event_cb(btn1, btn_event_cb);

        lv_obj_t* label1 = lv_label_create(btn1, NULL);
        lv_label_set_text(label1, "Ok");

        lv_obj_t* btn2 = lv_btn_create(win, btn1);
        lv_obj_set_pos(btn2, 480, 340);

        lv_obj_t* label2 = lv_label_create(btn2, NULL);
        lv_label_set_text(label2, "Cancel");

        logbtn = lv_btn_create(win, NULL);
        lv_obj_set_size(logbtn, 200, 70);
        lv_obj_align(logbtn, NULL, LV_ALIGN_IN_TOP_MID, 0, 230);
        lv_obj_set_event_cb(logbtn, btn_event_cb);

        rankbtn = lv_btn_create(win, NULL);
        lv_obj_set_size(rankbtn, 200, 70);
        lv_obj_align(rankbtn, NULL, LV_ALIGN_IN_TOP_MID, 0, 130);
        lv_obj_set_event_cb(rankbtn, btn_event_cb);

        lv_obj_t* label3 = lv_label_create(logbtn, NULL);
        lv_label_set_text(label3, "Press here to log in!");

        lv_obj_t* label4 = lv_label_create(rankbtn, NULL);
        lv_label_set_text(label4, "Ranking");

        lv_obj_t* sw1 = lv_switch_create(win, NULL);
        lv_obj_align(sw1, NULL, LV_ALIGN_IN_TOP_MID, 0, 50);
        lv_obj_set_event_cb(sw1, switch_event_handler);
        lv_obj_set_size(sw1, 100, 40);

        lv_obj_t* label5 = lv_label_create(win, NULL);
        lv_label_set_text(label5, "DARK");
        lv_obj_align(label5, NULL, LV_ALIGN_IN_TOP_MID, -60, 60);

        oobtn = lv_btn_create(win, NULL);
        lv_obj_set_size(oobtn, 50, 50);
        lv_obj_align(oobtn, NULL, LV_ALIGN_IN_TOP_MID, 350, 350);
        lv_obj_set_event_cb(oobtn, btn_event_cb);

        lv_obj_t* label6 = lv_label_create(oobtn, NULL);
        lv_label_set_text(label6, LV_SYMBOL_EYE_OPEN);
    }
    lv_obj_set_hidden(win, false);
    isGaming = false;
}

static void btn_log_event_cb(lv_obj_t* btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (btn == btn_cancel)
        {
            lv_obj_set_hidden(win, false);
            lv_obj_set_hidden(win_login, true);
        }
        else
        {
            static const char* btns2[] = { "Ok","" };
            static const char* btns3[] = { "Log out","Cancel","" };
            static const char* btns4[] = { "Back","Cancel","" };

            lv_obj_t* obj = lv_obj_create(lv_scr_act(), NULL);
            lv_obj_reset_style_list(obj, LV_OBJ_PART_MAIN);
            lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style_modal);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);

            if (btn == btn_in)
            {
                mbox = lv_msgbox_create(obj, NULL);
                lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, -50);
                lv_obj_set_event_cb(mbox, mbox_event_cb);

                if (!connectFlag)
                {
                    lv_msgbox_add_btns(mbox, btns4);
                    lv_msgbox_set_text(mbox, "WiFi unconnected!\nPlease connect in the home page first.");
                }
                else
                {
                    if (loginFlag)
                    {
                        lv_msgbox_add_btns(mbox, btns2);
                        lv_msgbox_set_text(mbox, "You have already logged in!");
                    }
                    else if (strcmp(lv_textarea_get_text(key), "") && !strcmp(lv_textarea_get_text(password), "2048") && sizeof(lv_textarea_get_text(key)) < 10)
                    {
                        loginFlag = true;
                        lv_msgbox_add_btns(mbox, btns2);
                        lv_msgbox_set_text(mbox, "Successfully log in!\nYou can enjoy more now!");
                        strcpy(userKey, lv_textarea_get_text(key));

                        isGaming = true;
                        lv_obj_set_hidden(win_login, true);
                        lv_2048(dimension);
                    }
                    else
                    {
                        lv_msgbox_add_btns(mbox, btns2);
                        lv_msgbox_set_text(mbox, "Failed!\nUnmatched password or invalid user key!");
                    }
                }
            }

            if (btn == btn_out)
            {
                mbox = lv_msgbox_create(obj, NULL);
                lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, -50);
                lv_obj_set_event_cb(mbox, mbox_event_cb);
                if (loginFlag)
                {
                    lv_msgbox_add_btns(mbox, btns3);
                    lv_msgbox_set_text(mbox, "Are you sure to log out?\nYou will lose your priviledge!");
                }
                else
                {
                    lv_msgbox_add_btns(mbox, btns2);
                    lv_msgbox_set_text(mbox, "You haven't logged in yet!");
                }
            }

            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_time(&a, 500);
            lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_50);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)opa_anim);
            lv_anim_start(&a);
        }
    }
}

static void win_log_event_cb(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (kb)
            lv_obj_set_hidden(kb, true);
        lv_obj_set_hidden(obj, true);
    }
}

static void keyboard(lv_obj_t* ta)
{
    if (!obj)
    {
        obj = lv_obj_create(win_login, NULL);
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_reset_style_list(obj, LV_OBJ_PART_MAIN);
        lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style_modal);
        lv_obj_set_size(obj, 740, 300);
        lv_obj_align(obj, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        lv_obj_set_event_cb(obj, win_log_event_cb);
    }
    lv_obj_set_hidden(obj, false);

    if (!kb)
    {
        kb = lv_keyboard_create(win_login, NULL);
        lv_obj_set_size(kb, 740, 220);
        lv_obj_align(kb, NULL, LV_ALIGN_IN_TOP_MID, 0, 240);
    }
    lv_obj_set_hidden(kb, false);

    lv_keyboard_set_textarea(kb, ta);
    lv_keyboard_set_cursor_manage(kb, true); /* Automatically show/hide cursors on text areas */
}

static void ta_event_cb(lv_obj_t* ta, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        keyboard(ta);
    }
}

void lv_login_win()
{
    if (!win_login)
    {
        /*Create a window*/
        win_login = lv_win_create(lv_scr_act(), NULL);
        lv_win_set_title(win_login, "Log in");                        /*Set the title*/

        btn_in = lv_btn_create(win_login, NULL);
        lv_obj_set_pos(btn_in, 160, 340);
        lv_obj_set_size(btn_in, 120, 50);
        lv_obj_set_event_cb(btn_in, btn_log_event_cb);

        lv_obj_t* label1 = lv_label_create(btn_in, NULL);
        lv_label_set_text(label1, "Log in");

        btn_cancel = lv_btn_create(win_login, btn_in);
        lv_obj_set_pos(btn_cancel, 520, 340);

        lv_obj_t* label2 = lv_label_create(btn_cancel, NULL);
        lv_label_set_text(label2, "Cancel");

        btn_out = lv_btn_create(win_login, btn_in);
        lv_obj_set_pos(btn_out, 340, 340);

        lv_obj_t* label3 = lv_label_create(btn_out, NULL);
        lv_label_set_text(label3, "Log out");

        /*User key*/
        key = lv_textarea_create(win_login, NULL);
        lv_obj_align(key, NULL, LV_ALIGN_IN_TOP_MID, 0, 60);
        lv_obj_set_height(key, 40);
        lv_textarea_set_text(key, "");
        lv_textarea_set_pwd_mode(key, false);
        lv_textarea_set_one_line(key, true);
        lv_textarea_set_cursor_hidden(key, true);
        lv_obj_set_event_cb(key, ta_event_cb);

        lv_obj_t* key_label = lv_label_create(win_login, NULL);
        lv_label_set_text(key_label, "User Key:");
        lv_obj_align(key_label, key, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

        /*Password*/
        password = lv_textarea_create(win_login, key);
        lv_obj_align(password, NULL, LV_ALIGN_IN_TOP_MID, 0, 140);
        lv_textarea_set_pwd_mode(password, true);

        lv_obj_t* pwd_label = lv_label_create(win_login, NULL);
        lv_label_set_text(pwd_label, "Password:");
        lv_obj_align(pwd_label, password, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

        if (darkFlag)
        {
            lv_obj_set_style_local_bg_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_set_style_local_text_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        }
        else
        {
            lv_obj_set_style_local_bg_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
            lv_obj_set_style_local_text_color(win_login, LV_WIN_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        }
    }
    lv_obj_set_hidden(win_login, false);
}
