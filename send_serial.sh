#!/bin/bash
set -euo pipefail

# Default values
DEVICE="/dev/ttyACM0"
BAUD_RATE=115200

# Function to show usage
usage() {
  echo "Usage: $0 [-d device] [-b baud_rate] filename"
  echo "  -d device    : Serial device (default: /dev/ttyACM0)"
  echo "  -b baud_rate : Baud rate (default: 115200)"
  echo "  filename     : File to send"
  exit 1
}

# Parse command line options
while getopts "d:b:h" opt; do
  case "$opt" in
  d) DEVICE="$OPTARG" ;;
  b) BAUD_RATE="$OPTARG" ;;
  h) usage ;;
  ?) usage ;;
  esac
done

# Shift to get the filename
shift $((OPTIND - 1))

# Check if filename is provided
if [ $# -ne 1 ]; then
  usage
fi

FILENAME="$1"

# Check if file exists
if [ ! -f "$FILENAME" ]; then
  echo "Error: File '$FILENAME' not found."
  exit 1
fi

# Check if device exists
if [ ! -c "$DEVICE" ]; then
  echo "Error: Device '$DEVICE' not found or is not a character device."
  exit 1
fi

# Check if stty is available
if ! command -v stty >/dev/null; then
  echo "Error: 'stty' command not found. Please install it to use this script."
  exit 1
fi

# Configure serial port
stty -F "$DEVICE" "$BAUD_RATE" cs8 -cstopb -parenb raw

# Start listening to serial output in the background
cat "$DEVICE" &
LISTENER_PID=$!

# Send file to serial port
if ! cat "$FILENAME" >"$DEVICE"; then
  echo "Error: Failed to send file to the device."
  kill "$LISTENER_PID" 2>/dev/null
  exit 1
fi

# Signal end of file and flush
echo "{{{END}}}" >>"$DEVICE"
echo -e "\n" >"$DEVICE"
echo "File '$FILENAME' successfully sent to device '$DEVICE' at baud rate $BAUD_RATE."

# Wait for user to terminate the script
echo "Listening to serial output. Press Ctrl+C to stop."
trap "kill $LISTENER_PID 2>/dev/null" EXIT
wait
