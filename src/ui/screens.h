#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *button_add;
    lv_obj_t *obj0;
    lv_obj_t *milliseconds;
    lv_obj_t *light;
    lv_obj_t *count;
    lv_obj_t *milliseconds_value;
    lv_obj_t *cdr_value;
    lv_obj_t *count_value;
    lv_obj_t *rotate;
    lv_obj_t *grad_r;
    lv_obj_t *grad_g;
    lv_obj_t *grad_b;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/