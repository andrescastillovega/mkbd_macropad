# ZMK Display Animations Implementation Guide

## Overview

This guide provides technical details on implementing display animations in ZMK using LVGL's animation system. Based on analysis of the zmk-nice-oled repository and LVGL documentation.

## Animation Implementation in ZMK

### Core Concepts
- Animations use LVGL's animation system underneath
- Frame-based animations (typically 16 frames)
- Configurable timing (default 60fps = 960ms per frame)

### Configuration Options
- `CONFIG_NICE_OLED_GEM_ANIMATION=n` - Disable animations
- `CONFIG_NICE_OLED_GEM_ANIMATION_MS` - Control animation speed
- Widget-specific timing controls

### Performance Considerations
- Animations are processor-intensive
- Can be selectively disabled to save battery
- Frame optimization techniques similar to retro gaming

### Common Animation Types
- Vertical scrolling widgets
- Character animations (like the Luna dog)
- State-based transitions (WPM, modifiers, HID indicators)

The animations integrate with ZMK's widget system using the standard `ZMK_DISPLAY_WIDGET_LISTENER` pattern but add timer-based frame updates for smooth motion.

## Technical Approach - Detailed Implementation

### 1. Frame Management System

**Multi-frame Animation Storage:**
```c
// Store animation frames as image arrays
const lv_img_dsc_t *animation_frames[] = {
    &frame_0, &frame_1, &frame_2, /* ... */ &frame_15
};

// Animation state tracking
typedef struct {
    uint8_t current_frame;
    uint8_t frame_count;
    uint32_t last_update_ms;
    uint32_t frame_duration_ms;
    bool is_playing;
} animation_state_t;
```

### 2. Timer-Based Frame Updates

**LVGL Timer Integration:**
```c
// Create dedicated animation timer
static lv_timer_t *anim_timer;

static void animation_timer_cb(lv_timer_t *timer) {
    animation_state_t *state = (animation_state_t*)timer->user_data;
    
    if (state->is_playing) {
        // Advance to next frame
        state->current_frame = (state->current_frame + 1) % state->frame_count;
        
        // Update widget display
        lv_img_set_src(widget->img_obj, animation_frames[state->current_frame]);
    }
}

// Initialize with configurable frame rate
anim_timer = lv_timer_create(animation_timer_cb, frame_duration_ms, &anim_state);
```

### 3. LVGL Animation System Integration

**Property Animation (Smooth Transitions):**
```c
// Create smooth position/opacity animations
static void create_slide_animation(lv_obj_t *obj, int32_t start_x, int32_t end_x) {
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, start_x, end_x);
    lv_anim_set_time(&anim, 500);  // 500ms duration
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
    lv_anim_start(&anim);
}
```

### 4. ZMK Event-Driven Animation Triggers

**State-Based Animation Control:**
```c
// Integrate with ZMK widget event system
static struct widget_state get_state(const zmk_event_t *eh) {
    struct widget_state state = {0};
    
    if (zmk_usb_is_powered()) {
        state.animation_type = ANIM_CHARGING;
        state.should_animate = true;
    } else if (zmk_activity_get_state() == ZMK_ACTIVITY_ACTIVE) {
        state.animation_type = ANIM_ACTIVE;
        state.should_animate = true;
    } else {
        state.should_animate = false;
    }
    
    return state;
}

static void widget_update_cb(struct widget_state state) {
    if (state.should_animate && !anim_state.is_playing) {
        // Start animation
        anim_state.is_playing = true;
        lv_timer_resume(anim_timer);
    } else if (!state.should_animate && anim_state.is_playing) {
        // Stop animation
        anim_state.is_playing = false;
        lv_timer_pause(anim_timer);
    }
}
```

### 5. Memory-Optimized Animation Techniques

**Frame Compression & Management:**
```c
// Use LVGL's built-in animimg widget for efficiency
typedef struct {
    lv_animimg_t animimg;
    const void **frame_array;
    uint8_t frame_count;
    uint32_t duration_ms;
} zmk_anim_widget_t;

int zmk_anim_widget_init(zmk_anim_widget_t *widget, lv_obj_t *parent) {
    widget->obj = lv_animimg_create(parent);
    lv_animimg_set_src(widget->obj, widget->frame_array, widget->frame_count);
    lv_animimg_set_duration(widget->obj, widget->duration_ms);
    return 0;
}
```

### 6. Performance Optimization Patterns

**Conditional Animation Execution:**
```c
// Battery-aware animation control
#if CONFIG_ZMK_WIDGET_BATTERY_STATUS
    if (zmk_battery_state_of_charge() < 20) {
        // Disable animations when battery is low
        animation_config.enabled = false;
    }
#endif

// CPU load management
static void throttle_animation_fps(void) {
    if (system_cpu_load > 80) {
        lv_timer_set_period(anim_timer, frame_duration_ms * 2);  // Half FPS
    } else {
        lv_timer_set_period(anim_timer, frame_duration_ms);       // Normal FPS
    }
}
```

## Key LVGL Animation Components

### Animation Structure (`lv_anim_t`)
- `var`: Variable to animate
- `exec_cb`: Function to execute for animation
- `start_value` / `end_value`: Animation range
- `time`: Animation duration in ms
- `path_cb`: Animation curve/easing function
- `ready_cb`: Callback when animation completes

### Timer System (`lv_timer_t`)
- `period`: How often timer should run
- `timer_cb`: Timer callback function
- `repeat_count`: Number of times to repeat
- `user_data`: Custom data for callback

### AnimImg Widget (`lv_animimg_t`)
- Built-in LVGL widget for frame-based animations
- Automatic frame cycling
- Configurable duration and repeat settings

## Best Practices

1. **Use LVGL's built-in `animimg` widget for frame-based animations**
2. **Implement battery-aware animation control**
3. **Use ZMK's event system to trigger animations**
4. **Optimize frame storage for memory constraints**
5. **Provide configuration options to disable animations**
6. **Monitor CPU usage and throttle animations if needed**

## Configuration Template

```c
// Kconfig options
config ZMK_WIDGET_ANIMATED_LOGO
    bool "Animated logo widget"
    depends on ZMK_DISPLAY
    select LV_USE_ANIMIMG
    select LV_USE_TIMER

config ZMK_WIDGET_ANIMATED_LOGO_DURATION_MS
    int "Animation duration in milliseconds"
    depends on ZMK_WIDGET_ANIMATED_LOGO
    default 960
    help
      Duration for each animation frame in milliseconds.
      Lower values = faster animation, higher CPU usage.
```

This technical approach provides smooth, efficient animations while respecting ZMK's resource constraints and event-driven architecture.