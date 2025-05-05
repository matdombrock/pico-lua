#!/bin/bash

name=$1
input_file=$2
output_file=$3

# Check if the input file exists
if [ ! -f "$input_file" ]; then
  echo "Error: File '$input_file' not found!"
  exit 1
fi

# Write to the output C file
echo "Converting $input_file to $output_file..."

{
  echo "const char *pl_script_$name = \"\\"
  while IFS= read -r line; do
    # Escape backslashes and double quotes in the Lua script
    escaped_line=$(echo "$line" | sed 's/\\/\\\\/g; s/"/\\"/g')
    echo "$escaped_line\\n\\"
  done <"$input_file"
  echo "\";"
} >"$output_file"

echo "Conversion complete: $output_file"
