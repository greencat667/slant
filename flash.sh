#!/usr/bin/env bash
# Compile and flash Slant to a Waveshare ESP32-S3-ePaper-1.54 board.
# Run it in a normal Terminal on the computer the board is plugged into.
# Usage: ./flash.sh [serial-port]     e.g. ./flash.sh /dev/cu.usbmodem1101
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"
FQBN="esp32:esp32:esp32s3:PSRAM=opi,FlashSize=8M"
CORE_URL="https://espressif.github.io/arduino-esp32/package_esp32_index.json"

command -v arduino-cli >/dev/null || { echo "arduino-cli not found. Install it first (https://arduino.github.io/arduino-cli/)."; exit 1; }

arduino-cli core update-index --additional-urls "$CORE_URL" || echo "(index update failed - carrying on with what is installed)"
arduino-cli core install esp32:esp32@3.3.11 --additional-urls "$CORE_URL"
arduino-cli lib install "Adafruit GFX Library"

PORT="${1:-}"
if [ -z "$PORT" ]; then
  echo "Boards currently visible:"
  arduino-cli board list
  read -rp "Port to flash (e.g. /dev/cu.usbmodem1101, COM5): " PORT
fi
[ -n "$PORT" ] || { echo "No port given."; exit 1; }

arduino-cli compile --fqbn "$FQBN" slant
arduino-cli upload  --fqbn "$FQBN" -p "$PORT" slant
echo "Done. Press PWR or BOOT on the board to wake it."
