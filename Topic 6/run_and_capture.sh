#!/bin/bash

# Script to run the specular lighting demo and capture a screenshot
# Note: This script requires screencapture (macOS) or import (ImageMagick) for Linux

echo "Starting SpecularLightingDemo..."
echo "The program will run for 5 seconds, then attempt to capture a screenshot."
echo "Use W/A/S/D/Q/E keys to move the camera if needed."
echo ""

# Start the program in background
./specular_demo &
DEMO_PID=$!

# Wait for the window to appear and initialize
sleep 3

# Try to capture screenshot (macOS specific)
if command -v screencapture &> /dev/null; then
    echo "Capturing screenshot..."
    screencapture SpecularLightingDemo_Screenshot.png
    echo "Screenshot saved as SpecularLightingDemo_Screenshot.png"
elif command -v import &> /dev/null; then
    echo "Capturing screenshot..."
    import -window root SpecularLightingDemo_Screenshot.png
    echo "Screenshot saved as SpecularLightingDemo_Screenshot.png"
else
    echo "No screenshot tool available. Please manually capture a screenshot."
fi

# Stop the demo
kill $DEMO_PID 2>/dev/null

echo "Done!"
