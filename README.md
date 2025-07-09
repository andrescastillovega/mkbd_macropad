# MKBD Macropad ZMK Shield

A split 3x4 macropad shield for ZMK firmware, designed to work with Nice!Nano v2 controllers.

## Features

- **Split Design**: Two 3x4 halves for ergonomic use
- **OLED Display**: 128x32 SSD1306 OLED on left half
- **Wireless**: Bluetooth connectivity via Nice!Nano v2
- **Customizable**: Full ZMK keymap support with layers

## Hardware Configuration

### Matrix Layout
Each half features a 3x4 key matrix (12 keys total per half):
```
[ 1 ] [ 2 ] [ 3 ]
[ 4 ] [ 5 ] [ 6 ]
[ 7 ] [ 8 ] [ 9 ]
[FN ] [ 0 ] [ENT]
```

### Pin Assignment
- **Columns**: Pro Micro pins 21, 20, 19
- **Rows**: Pro Micro pins 4, 5, 6, 7
- **Diode Direction**: COL2ROW
- **OLED**: I2C on address 0x3C (left half only)

## Firmware Building

The firmware is automatically built using GitHub Actions for both halves:

- `mkbd_macropad_left` - Left half with OLED display
- `mkbd_macropad_right` - Right half (no display)

### Build Targets
```yaml
- board: nice_nano_v2
  shield: mkbd_macropad_left
- board: nice_nano_v2
  shield: mkbd_macropad_right
```

## Default Keymap

### Layer 0 (Default)
```
NUM_1  NUM_2  NUM_3
NUM_4  NUM_5  NUM_6
NUM_7  NUM_8  NUM_9
MO(1)  NUM_0  ENTER
```

### Layer 1 (Function)
```
BT_SEL_0  BT_SEL_1  BT_CLR
F1        F2        F3
F4        F5        F6
TRANS     BSPC      DEL
```

## Installation

1. Download the latest firmware from the [Releases](../../releases) page
2. Flash `mkbd_macropad_left_nice_nano_v2.uf2` to the left half
3. Flash `mkbd_macropad_right_nice_nano_v2.uf2` to the right half
4. Pair both halves with your device via Bluetooth

## Customization

To customize the keymap:

1. Edit the `.keymap` files in `boards/shields/mkbd_macropad/`
2. Commit your changes to trigger automatic firmware building
3. Download the updated firmware from the Actions tab

## Files Structure

```
boards/shields/mkbd_macropad/
├── mkbd_macropad.overlay          # Base overlay (unused - for reference)
├── mkbd_macropad.keymap           # Base keymap (unused - for reference)
├── mkbd_macropad_left.overlay     # Left half hardware definition
├── mkbd_macropad_left.keymap      # Left half keymap
├── mkbd_macropad_left.conf        # Left half configuration
├── mkbd_macropad_right.overlay    # Right half hardware definition
├── mkbd_macropad_right.keymap     # Right half keymap
├── mkbd_macropad_right.conf       # Right half configuration
├── custom_status_screen.c         # Custom OLED display code
├── CMakeLists.txt                 # Build configuration
├── Kconfig.shield                 # Shield configuration
└── Kconfig.defconfig              # Default configuration
```

## Hardware Requirements

- 2x Nice!Nano v2 controllers
- 1x SSD1306 128x32 OLED display (for left half)
- 24x MX-compatible switches
- 24x 1N4148 diodes
- Custom PCB or handwired matrix

## Bluetooth Pairing

The function layer includes Bluetooth controls:
- **BT_SEL_0**: Select profile 0
- **BT_SEL_1**: Select profile 1  
- **BT_CLR**: Clear current profile

## License

This project is open source. See the individual file headers for specific licensing information.