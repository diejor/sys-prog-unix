#!/bin/bash

# Diego Rodrigues
# CS 3377.501
# Professor Mohamed Amine Belkoura
# Assignment 4

# Perform multiple file searching operations using 'find'.

# Task a: List all files modified within the last day.
find . -type f -mtime -1

# Task b: List files larger than 1MB with read access.
find / -type f -size +1M -readable 2>/dev/null

# Task c: Remove all 'core' files from home directory.
find ~/ -type f -name 'core' -exec echo "Removing: {}" \; -exec rm {} \;

# Task d: List inode numbers of .c files.
find . -type f -name '*.c' -printf 'Inode: %i File: %p\n'

# Task e: List files with read access modified in the last 30 days.
find / -type f -readable -mtime -30 2>/dev/null

echo "All tasks completed."
