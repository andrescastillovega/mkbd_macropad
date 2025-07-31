## Project Memories

### How to Build and Test ZMK Firmware

To build and test the ZMK firmware with custom display modifications:

1. **Navigate to ZMK directory and activate virtual environment**:
   ```bash
   cd /home/avena/Documents/projects/mkbd_macropad/zmk
   source .venv/bin/activate
   ```

2. **Build the firmware for left side**:
   ```bash
   cd app
   west build -p -b nice_nano_v2 -S zmk-usb-logging -- -DZMK_CONFIG="../../" -DSHIELD=mkbd_macropad_left
   ```

3. **Build the firmware for right side**:
   ```bash
   west build -p -b nice_nano_v2 -S zmk-usb-logging -- -DZMK_CONFIG="../../" -DSHIELD=mkbd_macropad_right
   ```

4. **Build outputs location**:
   - Built firmware will be in: `zmk/app/build/zephyr/zmk.uf2`
   - This file can be flashed to the nice_nano_v2 controller

5. **Key build parameters**:
   - `-p`: Pristine build (clean build)
   - `-b nice_nano_v2`: Target board (nice nano v2)
   - `-S zmk-usb-logging`: Enable USB logging for debugging
   - `-DZMK_CONFIG="../../"`: Points to the project config directory
   - `-DSHIELD=mkbd_macropad_left/right`: Specifies which side to build

6. **Flash to device**:
   - Put the nice_nano_v2 into bootloader mode (double-press reset)
   - Copy the `zmk.uf2` file to the mounted USB drive
   - Device will reboot with the new firmware

**Note**: Always activate the Python virtual environment before building to ensure correct dependencies.

# important-instruction-reminders
Do what has been asked; nothing more, nothing less.
NEVER create files unless they're absolutely necessary for achieving your goal.
ALWAYS prefer editing an existing file to creating a new one.
NEVER proactively create documentation files (*.md) or README files. Only create documentation files if explicitly requested by the User.