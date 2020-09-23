/**
 * @file lv_2048.h
 *
 */
#include"lv_2048_setting.h"
#include"lvgl.h"

#ifndef LV_2048_H
#define LV_2048_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

    enum Difficulty
    {
        EASY = 1, MEDIUM, HARD
    };

    void lv_2048(int dimension);

    void lv_2048_others();

    void RightShift();

    void LeftShift();

    void DownShift();

    void UpShift();

    extern int gameArray[64];

    extern int dimension;

    extern int score;

    extern int winNumber;

    extern int withdraw_chance;

    extern int destroy_chance;

    extern bool destroying;

    extern bool isGaming;

    extern enum Difficulty difficulty;

    extern bool If_Win;

    extern bool loginFlag;

    extern bool connectFlag;

    extern bool darkFlag;

    extern char userKey[10];

    void initialize();

    void trans();

    bool judgeChange();

    void LeftMove();

    void RightMove();

    void UpMove();

    void DownMove();

    int countZero();

    void Generate(int n);

    void GenerateRandomly();

    bool judgeLose();

    void msgbox_type_1(const char* key[3], char* info);

    lv_obj_t* win;

    lv_obj_t* win_login;

    extern char ranks[3][10];
    extern char values[3][10];

    extern lv_style_t style_0;
    extern lv_style_t style_modal;

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_2048_H*/
