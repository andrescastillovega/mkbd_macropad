#include <zephyr/kernel.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/usb.h>

#include "battery_anim.h"
#include "../battery_level_images.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

// Animation frames array
static const lv_img_dsc_t *battery_frames[] = {
    &battery_level_0,
    &battery_level_25,
    &battery_level_50,
    &battery_level_100
};

#define BATTERY_FRAME_COUNT 4
#define ANIMATION_DURATION_MS 2000
#define FRAME_DURATION_MS (ANIMATION_DURATION_MS / BATTERY_FRAME_COUNT)

// Animation state
typedef struct {
    uint8_t current_frame;
    bool is_animating;
} animation_state_t;

static animation_state_t anim_state = {
    .current_frame = 0,
    .is_animating = false
};

static lv_timer_t *anim_timer = NULL;

// Timer callback for frame updates
static void animation_timer_cb(lv_timer_t *timer) {
    if (!anim_state.is_animating) {
        return;
    }

    // Advance to next frame
    anim_state.current_frame = (anim_state.current_frame + 1) % BATTERY_FRAME_COUNT;

    // Update all widget instances
    struct zmk_widget_battery_anim *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        lv_img_set_src(widget->obj, battery_frames[anim_state.current_frame]);
    }
}

// Widget state structure
struct widget_state {
    bool should_animate;
};

static void widget_update_cb(struct widget_state state) {
    if (state.should_animate && !anim_state.is_animating) {
        // Start animation
        anim_state.is_animating = true;
        anim_state.current_frame = 0;
        
        if (anim_timer == NULL) {
            anim_timer = lv_timer_create(animation_timer_cb, FRAME_DURATION_MS, NULL);
        } else {
            lv_timer_resume(anim_timer);
        }
        
        // Update display immediately
        struct zmk_widget_battery_anim *widget;
        SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
            lv_img_set_src(widget->obj, battery_frames[anim_state.current_frame]);
        }
    } else if (!state.should_animate && anim_state.is_animating) {
        // Stop animation
        anim_state.is_animating = false;
        if (anim_timer != NULL) {
            lv_timer_pause(anim_timer);
        }
        
        // Show static battery image
        struct zmk_widget_battery_anim *widget;
        SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
            lv_img_set_src(widget->obj, &battery_level_100);
        }
    }
}

static struct widget_state get_state(const zmk_event_t *eh) {
    struct widget_state state = {0};
    
#if IS_ENABLED(CONFIG_ZMK_USB)
    state.should_animate = zmk_usb_is_powered();
#else
    state.should_animate = false;
#endif
    
    return state;
}

// Core macro that handles thread-safe event management
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_anim, struct widget_state,
                             widget_update_cb, get_state)

// Subscribe to USB connection events
#if IS_ENABLED(CONFIG_ZMK_USB)
ZMK_SUBSCRIPTION(widget_battery_anim, zmk_usb_conn_state_changed);
#endif

int zmk_widget_battery_anim_init(struct zmk_widget_battery_anim *widget, lv_obj_t *parent) {
    widget->obj = lv_img_create(parent);
    
    // Set initial image
    lv_img_set_src(widget->obj, &battery_level_100);
    
    // Configure widget properties
    lv_obj_set_size(widget->obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    
    // Add to widget list
    sys_slist_append(&widgets, &widget->node);
    
    // Initialize widget listener
    widget_battery_anim_init();
    
    return 0;
}

lv_obj_t *zmk_widget_battery_anim_obj(struct zmk_widget_battery_anim *widget) {
    return widget->obj;
}