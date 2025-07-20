#include <lvgl.h>

// Placeholder battery level images
// These should be replaced with actual battery level images

// Simple 16x8 pixel battery icons (placeholder data)
static const uint8_t battery_0_data[] = {
    0x00, 0x00, 0x7F, 0xFE, 0x40, 0x02, 0x40, 0x02,
    0x40, 0x02, 0x40, 0x02, 0x7F, 0xFE, 0x00, 0x00
};

static const uint8_t battery_25_data[] = {
    0x00, 0x00, 0x7F, 0xFE, 0x40, 0x02, 0x43, 0x02,
    0x43, 0x02, 0x40, 0x02, 0x7F, 0xFE, 0x00, 0x00
};

static const uint8_t battery_50_data[] = {
    0x00, 0x00, 0x7F, 0xFE, 0x40, 0x02, 0x47, 0x82,
    0x47, 0x82, 0x40, 0x02, 0x7F, 0xFE, 0x00, 0x00
};

static const uint8_t battery_100_data[] = {
    0x00, 0x00, 0x7F, 0xFE, 0x40, 0x02, 0x4F, 0xE2,
    0x4F, 0xE2, 0x40, 0x02, 0x7F, 0xFE, 0x00, 0x00
};

const lv_img_dsc_t battery_level_0 = {
    .header.cf = LV_IMG_CF_ALPHA_1BIT,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 16,
    .header.h = 8,
    .data_size = sizeof(battery_0_data),
    .data = battery_0_data,
};

const lv_img_dsc_t battery_level_25 = {
    .header.cf = LV_IMG_CF_ALPHA_1BIT,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 16,
    .header.h = 8,
    .data_size = sizeof(battery_25_data),
    .data = battery_25_data,
};

const lv_img_dsc_t battery_level_50 = {
    .header.cf = LV_IMG_CF_ALPHA_1BIT,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 16,
    .header.h = 8,
    .data_size = sizeof(battery_50_data),
    .data = battery_50_data,
};

const lv_img_dsc_t battery_level_100 = {
    .header.cf = LV_IMG_CF_ALPHA_1BIT,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 16,
    .header.h = 8,
    .data_size = sizeof(battery_100_data),
    .data = battery_100_data,
};