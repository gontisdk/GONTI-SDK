#!/bin/bash

# List directory structure
tree /f /a > struct.txt

# Clear the terminal screen
clear

echo "What do you want to do?"
echo "1 or b - Build engine"
echo "2 or t - Build testbed"
echo "3 or d - Debug testbed"
read -p "Enter command (Build/Testbed/Debug): " userInput

# Convert user input to lowercase
userInput=$(echo "$userInput" | tr '[:upper:]' '[:lower:]')

case "$userInput" in
  "build" | "b" | "1")
    # Build engine
    cd _Build/_Scripts/linux/ || exit
    ./main.sh
    ;;
  "testbed" | "t" | "2")
    # Build testbed
    cd Tests/BuildScripts/linux/ || exit
    ./makeTestbed.sh
    ;;
  "debug" | "d" | "3")
    # Debug testbed
    cd Tests/BuildScripts/linux/ || exit
    ./Debug.sh
    ;;
  *)
    echo "Invalid value, exiting..."
    exit 1
    ;;
esac

exit 0
