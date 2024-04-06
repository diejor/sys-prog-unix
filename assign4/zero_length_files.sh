#!/bin/bash

# Diego Rodrigues
# CS 3377.501
# Professor Mohamed Amine Belkoura
# Assignment 4
#
# Script to find and optionally delete zero-length files. Pass the target directory and optionally -f for force delete without confirmation.

# Check for at least one argument
if [[ $# -lt 1 ]]; then
  echo "Usage: $0 directory [-f]"
  exit 1
fi

# Assign arguments to variables
directory=$1
force=false

# Check for '-f' option
if [[ "$2" == "-f" ]]; then
  force=true
fi

# Function to delete file with confirmation
delete_file() {
  if ! $force; then
    read -p "Delete '$directory'? [y/N]: " confirm
    if [[ $confirm == [yY] ]]; then
      rm "$1"
      echo "Deleted '$directory'."
    else
      echo "Skipped '$directory'."
    fi
  else
    rm "$1"
    echo "Force-deleted '$directory'."
  fi
}

# Find zero-length files and take action
find "$directory" -type f -size 0 -print0 | while IFS= read -r -d '' file; do
  echo "$file"
  delete_file "$file"
done

