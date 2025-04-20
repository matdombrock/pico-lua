#!/bin/bash

# Check if the input file is provided
if [ $# -ne 1 ]; then
  echo "Usage: $0 <script.lua>"
  exit 1
fi

input_file="$1"
output_file="./lua_test/script.lua.h"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
  echo "Error: File '$input_file' not found!"
  exit 1
fi

# Write to the output C file
echo "Converting $input_file to $output_file..."

{
  echo "const char *lua_script = \"\\"
  while IFS= read -r line; do
    # Escape backslashes and double quotes in the Lua script
    escaped_line=$(echo "$line" | sed 's/\\/\\\\/g; s/"/\\"/g')
    echo "$escaped_line\\n\\"
  done <"$input_file"
  echo "\";"
} >"$output_file"

echo "Conversion complete: $output_file"
