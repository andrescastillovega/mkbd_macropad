from PIL import Image

# Load the PNG image
img = Image.open('../../P1_screen.png')
img = img.convert('1')  # Ensure 1-bit format
width, height = img.size

# Generate C array
pixels = list(img.getdata())

# Convert to LVGL format (1 bit per pixel, packed into bytes)
bytes_per_line = (width + 7) // 8
image_data = []

for y in range(height):
    for byte_x in range(bytes_per_line):
        byte_val = 0
        for bit in range(8):
            x = byte_x * 8 + bit
            if x < width:
                pixel_idx = y * width + x
                if pixels[pixel_idx]:  # White pixel
                    byte_val |= (1 << (7 - bit))
        image_data.append(byte_val)

# Generate C file
c_code = f"""#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

const LV_ATTRIBUTE_MEM_ALIGN uint8_t p1_keyboard_img_map[] = {{
"""

# Add data in rows of 16 bytes
for i in range(0, len(image_data), 16):
    row = image_data[i:i+16]
    hex_values = [f'0x{b:02x}' for b in row]
    c_code += '  ' + ', '.join(hex_values)
    if i + 16 < len(image_data):
        c_code += ','
    c_code += '\n'

c_code += f"""
}};

const lv_img_dsc_t p1_keyboard_img = {{
  .header.always_zero = 0,
  .header.w = {width},
  .header.h = {height},
  .data_size = {len(image_data)},
  .header.cf = LV_IMG_CF_ALPHA_1BIT,
  .data = p1_keyboard_img_map,
}};
"""

# Write to file
with open('../../boards/shields/mkbd_macropad/p1_keyboard_img.c', 'w') as f:
    f.write(c_code)

print('Generated p1_keyboard_img.c')
