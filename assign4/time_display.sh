#!/bin/bash

# Diego Rodrigues
# CS 3377.501
# Professor Mohamed Amine Belkoura
# Assignment 4

# This script will display the current time every 15 seconds
# until the user stops the script.
while true; do
    clear
    echo "Current Time: $(date +"%r")"
    sleep 15
done

