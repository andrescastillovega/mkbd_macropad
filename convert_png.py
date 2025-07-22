import sys
import os
import argparse
from PIL import Image

def main():
    parser = argparse.ArgumentParser(description='Convert PNG image to LVGL C array')
    parser.add_argument('image_filename', help='Input PNG image filename')
    parser.add_argument('--invert', action='store_true', help='Invert colors (black becomes white)')
    parser.add_argument('--source-dir', '-s', default='.', help='Source directory (default: current directory)')
    parser.add_argument('--dest-dir', '-d', default='.', help='Destination directory (default: current directory)')
    
    args = parser.parse_args()
    
    input_filename = args.image_filename
    invert_colors = args.invert
    source_dir = args.source_dir
    dest_dir = args.dest_dir
    
    # Extract base name without extension
    base_name = os.path.splitext(input_filename)[0]
    
    # Construct full input path
    input_path = os.path.join(source_dir, input_filename)
    
    # Load the PNG image
    try:
        img = Image.open(input_path)
    except FileNotFoundError:
        print(f"Error: Image file '{input_path}' not found")
        sys.exit(1)
    
    # Handle RGBA images properly
    if img.mode == 'RGBA':
        # Create a black background for RGBA images
        background = Image.new('RGB', img.size, (0, 0, 0))
        background.paste(img, mask=img.split()[-1])  # Use alpha channel as mask
        img = background
    elif img.mode != 'RGB':
        img = img.convert('RGB')
    
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
                    pixel_value = pixels[pixel_idx]
                    pixel_is_white = pixel_value > 0  # Convert to boolean
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
    c_output_filename = os.path.join(dest_dir, f"{var_name}.c")
    h_output_filename = os.path.join(dest_dir, f"{var_name}.h")
    
    # Write C file
    with open(c_output_filename, 'w') as f:
        f.write(c_code)
    
    # Write header file
    with open(h_output_filename, 'w') as f:
        f.write(h_code)
    
    invert_status = " (inverted)" if invert_colors else ""
    print(f'Generated {c_output_filename} and {h_output_filename} from {input_path}{invert_status}')

if __name__ == "__main__":
    main()
