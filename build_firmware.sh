#!/bin/bash

# Define base paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ZMK_DIR="$SCRIPT_DIR/zmk"
CONFIG_DIR="$SCRIPT_DIR"
# WEST_VERBOSITY="-q"
WEST_VERBOSITY=""

# parse flags
# while [[ $# -gt 0 ]]; do
#   case "$1" in
#     -v|--verbose)
#       WEST_VERBOSITY="-v"
#       shift
#       ;;
#     *)
#       echo "Unknown option: $1"
#       usage
#       ;;
#   esac
# done

# Activate Python venv
cd "$ZMK_DIR"
source .venv/bin/activate

# Build the “left” side
cd app
west $WEST_VERBOSITY build \
  -p -d build/left \
  -b nice_nano_v2 \
  -S zmk-usb-logging \
  -- \
    -DZMK_CONFIG="$CONFIG_DIR" \
    -DSHIELD=mkbd_macropad_left

echo "✅ Build complete: build/left"

# Build the “right” side
west $WEST_VERBOSITY build \
  -p -d build/right \
  -b nice_nano_v2 \
  -S zmk-usb-logging \
  -- \
    -DZMK_CONFIG="$CONFIG_DIR" \
    -DSHIELD=mkbd_macropad_right

echo "✅ Build complete: build/right"

# Build the reset settings
west $WEST_VERBOSITY build \
  -p -d build/settings_reset \
  -b nice_nano_v2 \
  -- \
    -DSHIELD=settings_reset

echo "✅ Build complete: build/settings_reset"

cd $SCRIPT_DIR

