// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_scrMain
void ui_scrMain_screen_init(void);
lv_obj_t * ui_scrMain;
lv_obj_t * ui_pnlMain;
lv_obj_t * ui_lblMilliseconds;
lv_obj_t * ui_lblMillisecondsValue;
lv_obj_t * ui_lblCdr;
lv_obj_t * ui_lblCdrValue;
void ui_event_Rotate(lv_event_t * e);
lv_obj_t * ui_Rotate;
lv_obj_t * ui_Label1;
void ui_event_btnCount(lv_event_t * e);
lv_obj_t * ui_btnCount;
lv_obj_t * ui_lblButton;
lv_obj_t * ui_lblCount;
lv_obj_t * ui_lblCountValue;
lv_obj_t * ui_Image1;
lv_obj_t * ui____initial_actions0;
const lv_image_dsc_t * ui_imgset_philips_80x[1] = {&ui_img_philips_80x106_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Rotate(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        OnRotateClicked(e);
    }
}
void ui_event_btnCount(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        OnAddOneClicked(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_scrMain_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_scrMain);
}
