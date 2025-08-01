this proyect is a split macropad (left macropad as master that handles communication with the pc, and the right connects wirelessly to the master. any key stroke is sent from the master to the pc regardless of the stroke happened on the right or left side). The firmware is done with ZMK which has Zephyr bellow it.

both sides work with a niceNano v2. The left side has a screen.

each side has 5 rows and 6 columns.

Common Row pins 4,5,6,7,8

Left Columns pins 21,20,19,18,15,14.

Right Columns pins 14,15,18,19,20,21.

How to Build and Test ZMK Firmware
To build and test the ZMK firmware with custom display modifications:

Navigate to ZMK directory and activate virtual environment:

cd /mnt/c/Users/agred/proyecto_P1/mkbd_macropad/zmk
source .venv/bin/activate
Build the firmware for left side:

cd app
west build -p -b nice_nano_v2 -S zmk-usb-logging -- -DZMK_CONFIG="../../" -DSHIELD=mkbd_macropad_left
Build the firmware for right side:

west build -p -b nice_nano_v2 -S zmk-usb-logging -- -DZMK_CONFIG="../../" -DSHIELD=mkbd_macropad_right
Build outputs location:

Built firmware will be in: zmk/app/build/zephyr/zmk.uf2
This file can be flashed to the nice_nano_v2 controller
Key build parameters:

-p: Pristine build (clean build)
-b nice_nano_v2: Target board (nice nano v2)
-S zmk-usb-logging: Enable USB logging for debugging
-DZMK_CONFIG="../../": Points to the project config directory
-DSHIELD=mkbd_macropad_left/right: Specifies which side to build
Flash to device:

Put the nice_nano_v2 into bootloader mode (double-press reset)
Copy the zmk.uf2 file to the mounted USB drive
Device will reboot with the new firmware
Note: Always activate the Python virtual environment before building to ensure correct dependencies.