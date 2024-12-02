#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_button_add(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    
    if (event == LV_EVENT_CLICKED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 8, 0, e);
    }
}

static void event_handler_cb_main_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        action_on_rotate(e);
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(90), LV_PCT(90));
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Milliseconds
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.milliseconds = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Milliseconds:");
                }
                {
                    // Light
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.light = obj;
                    lv_obj_set_pos(obj, 0, 30);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Light:");
                }
                {
                    // Count
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.count = obj;
                    lv_obj_set_pos(obj, 0, 60);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Count:");
                }
                {
                    // MillisecondsValue
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.milliseconds_value = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "-");
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // CdrValue
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.cdr_value = obj;
                    lv_obj_set_pos(obj, 0, 30);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "cdr");
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // CountValue
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.count_value = obj;
                    lv_obj_set_pos(obj, 0, 60);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // ButtonAdd
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_add = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_add_event_cb(obj, event_handler_cb_main_button_add, LV_EVENT_ALL, flowState);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Add 1");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // Rotate
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.rotate = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj0 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Rotate");
                            lv_obj_add_event_cb(obj, event_handler_cb_main_obj0, LV_EVENT_ALL, flowState);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // GradR
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.grad_r = obj;
                    lv_obj_set_pos(obj, 0, -40);
                    lv_obj_set_size(obj, LV_PCT(50), 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // GradG
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.grad_g = obj;
                    lv_obj_set_pos(obj, 0, -20);
                    lv_obj_set_size(obj, LV_PCT(50), 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // GradB
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.grad_b = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(50), 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff0000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
    {
        const char *new_val = evalTextProperty(flowState, 7, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.count_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.count_value;
            lv_label_set_text(objects.count_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
}


static const char *screen_names[] = { "Main" };
static const char *object_names[] = { "main", "button_add", "obj0", "milliseconds", "light", "count", "milliseconds_value", "cdr_value", "count_value", "rotate", "grad_r", "grad_g", "grad_b" };

void create_screens() {
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
