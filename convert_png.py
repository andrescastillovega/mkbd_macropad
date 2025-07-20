import sys
import os
from PIL import Image

def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("Usage: python3 convert_png.py <image_filename> [--invert]")
        print("Example: python3 convert_png.py P1_screen.png")
        print("Example: python3 convert_png.py P1_screen.png --invert")
        sys.exit(1)
    
    input_filename = sys.argv[1]
    invert_colors = len(sys.argv) == 3 and sys.argv[2] == "--invert"
    
    # Extract base name without extension
    base_name = os.path.splitext(input_filename)[0]
    
    # Load the PNG image
    try:
        img = Image.open(input_filename)
    except FileNotFoundError:
        print(f"Error: Image file '{input_filename}' not found")
        sys.exit(1)
    
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
                    # Apply color inversion logic based on flag
                    pixel_is_white = pixels[pixel_idx]
                    if invert_colors:
                        # Invert: Black pixels become white in output
                        if not pixel_is_white:
                            byte_val |= (1 << (7 - bit))
                    else:
                        # Normal: White pixels stay white in output
                        if pixel_is_white:
                            byte_val |= (1 << (7 - bit))
            image_data.append(byte_val)
    
    # Generate C variable names based on filename
    var_name = base_name.replace('-', '_').replace(' ', '_')
    
    # Generate C file
    c_code = f"""#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

const LV_ATTRIBUTE_MEM_ALIGN uint8_t {var_name}_map[] = {{
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

const lv_img_dsc_t {var_name} = {{
  .header.always_zero = 0,
  .header.w = {width},
  .header.h = {height},
  .data_size = {len(image_data)},
  .header.cf = LV_IMG_CF_ALPHA_1BIT,
  .data = {var_name}_map,
}};
"""
    
    # Generate header file content
    header_guard = f"{var_name.upper()}_H"
    h_code = f"""#ifndef {header_guard}
#define {header_guard}

#ifdef __cplusplus
extern "C" {{
#endif

#include <lvgl.h>

LV_IMG_DECLARE({var_name});

#ifdef __cplusplus
}} /*extern "C"*/
#endif

#endif /*{header_guard}*/
"""
    
    # Generate output filenames
    c_output_filename = f"boards/shields/mkbd_macropad/{var_name}.c"
    h_output_filename = f"boards/shields/mkbd_macropad/{var_name}.h"
    
    # Write C file
    with open(c_output_filename, 'w') as f:
        f.write(c_code)
    
    # Write header file
    with open(h_output_filename, 'w') as f:
        f.write(h_code)
    
    invert_status = " (inverted)" if invert_colors else ""
    print(f'Generated {c_output_filename} and {h_output_filename} from {input_filename}{invert_status}')

if __name__ == "__main__":
    main()
