# MKBD Macropad - ZMK Shield

A custom 12-key macropad shield for ZMK firmware with Bluetooth connectivity and OLED display support.

## Features

- **12-key layout**: 3x4 matrix configuration
- **Bluetooth Low Energy**: Connect to multiple devices
- **OLED Display**: 128x32 SSD1306 display with custom status screen
- **USB/BLE dual connectivity**: Use wired or wireless
- **Two-layer keymap**: Default numbers + function layer

## Hardware Requirements

- **Controller**: Pro Micro compatible board (nice!nano v2 recommended for wireless)
- **Display**: SSD1306 128x32 OLED (I2C address 0x3C)
- **Switches**: 12x mechanical key switches
- **Diodes**: 12x 1N4148 diodes
- **PCB**: Custom PCB or handwired matrix

### Pin Configuration

| Function | Pro Micro Pin |
|----------|---------------|
| Col 0    | 21            |
| Col 1    | 20            |
| Col 2    | 19            |
| Row 0    | 4             |
| Row 1    | 5             |
| Row 2    | 6             |
| Row 3    | 7             |
| SDA      | 2             |
| SCL      | 3             |

## Keymap

### Default Layer (Layer 0)
```
┌─────┬─────┬─────┐
│  1  │  2  │  3  │
├─────┼─────┼─────┤
│  4  │  5  │  6  │
├─────┼─────┼─────┤
│  7  │  8  │  9  │
├─────┼─────┼─────┤
│ MO1 │  0  │ ENT │
└─────┴─────┴─────┘
```

### Function Layer (Layer 1)
```
┌─────┬─────┬─────┐
│BT_0 │BT_1 │BTCLR│
├─────┼─────┼─────┤
│ F1  │ F2  │ F3  │
├─────┼─────┼─────┤
│ F4  │ F5  │ F6  │
├─────┼─────┼─────┤
│     │BSPC │ DEL │
└─────┴─────┴─────┘
```

**Legend:**
- `MO1`: Momentary layer 1 (hold for function keys)
- `BT_0/1`: Select Bluetooth profile 0/1
- `BTCLR`: Clear Bluetooth bonds
- `ENT`: Enter key
- `BSPC`: Backspace
- `DEL`: Delete

## File Structure

```
mkbd_macropad/
├── README.md                 # This file
├── CMakeLists.txt           # Build configuration
├── Kconfig.defconfig        # Default configuration options
├── Kconfig.shield           # Shield identifier
├── custom_status_screen.c   # Custom OLED display code
├── mkbd_macropad.conf       # Firmware configuration
├── mkbd_macropad.keymap     # Key layout definition
└── mkbd_macropad.overlay    # Hardware definition (device tree)
```

## Building the Firmware

### Prerequisites

1. **Install ZMK Development Environment**
   ```bash
   # Install dependencies (Ubuntu/Debian)
   sudo apt update
   sudo apt install git cmake ninja-build gperf ccache dfu-util device-tree-compiler
   
   # Install Python dependencies
   pip3 install --user -U west
   
   # Add ~/.local/bin to PATH
   echo 'export PATH=~/.local/bin:$PATH' >> ~/.bashrc
   source ~/.bashrc
   ```

2. **Clone ZMK Repository**
   ```bash
   git clone https://github.com/zmkfirmware/zmk.git
   cd zmk
   west init -l app/
   west update
   ```

3. **Install Zephyr SDK**
   ```bash
   cd ~
   wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.1/zephyr-sdk-0.16.1_linux-x86_64.tar.gz
   tar xvf zephyr-sdk-0.16.1_linux-x86_64.tar.gz
   cd zephyr-sdk-0.16.1
   ./setup.sh
   ```

### Build Commands

Navigate to the ZMK app directory and build:

```bash
cd zmk/app

# For nice!nano v2 (wireless)
west build -b nice_nano_v2 -- -DSHIELD=mkbd_macropad

# For Pro Micro (wired)
west build -b pro_micro -- -DSHIELD=mkbd_macropad

# Clean build (if needed)
west build -b nice_nano_v2 -- -DSHIELD=mkbd_macropad -p
```

The compiled firmware will be located at:
- `build/zephyr/zmk.uf2` (for nice!nano v2)
- `build/zephyr/zmk.hex` (for Pro Micro)

### Flashing

**For nice!nano v2:**
1. Connect the board via USB
2. Double-tap the reset button to enter bootloader mode
3. Copy `zmk.uf2` to the mounted drive
4. The board will automatically reboot with new firmware

**For Pro Micro:**
1. Connect the board via USB
2. Reset the board (short RST to GND)
3. Flash using avrdude:
   ```bash
   avrdude -p atmega32u4 -c avr109 -P /dev/ttyACM0 -U flash:w:zmk.hex
   ```

## Customization

### Modifying the Keymap

Edit `mkbd_macropad.keymap` to change key assignments. Refer to [ZMK keycodes documentation](https://zmk.dev/docs/codes) for available keys.

### Custom Display Content

Modify `custom_status_screen.c` to change the OLED display content. The file uses LVGL for graphics rendering.

### Hardware Configuration

Edit `mkbd_macropad.overlay` to change:
- Pin assignments
- Matrix configuration
- Display settings

## Troubleshooting

### Common Issues

1. **Build fails**: Ensure all dependencies are installed and ZMK is up to date
2. **Keys not working**: Check pin assignments and diode orientation
3. **Display not working**: Verify I2C connections and address (0x3C)
4. **Bluetooth issues**: Clear bonds and re-pair devices

### Getting Help

- [ZMK Documentation](https://zmk.dev/)
- [ZMK Discord Server](https://discord.gg/8cfMkQksSB)
- [ZMK GitHub Issues](https://github.com/zmkfirmware/zmk/issues)

## License

This project is licensed under the MIT License - see the ZMK project license for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Changelog

- **v1.0.0**: Initial release with basic 12-key layout and OLED support