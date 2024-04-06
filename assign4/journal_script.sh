#!/bin/bash

# Diego Rodrigues
# CS 3377.501
# Professor Mohamed Amine Belkoura
# Assignment 4

# This script appends entries to a journal file located at $HOME/journal-file
# It checks for the existence and write permission of the journal-file
# It also adds entries with the user's name and the journal content.

journal_path="${HOME}/journal-file"
echo "Verifying presence of ${journal_path}..."

if [ -e "${journal_path}" ]; then
    echo "Found ${journal_path}"
    if [ ! -w "${journal_path}" ]; then
        echo "Write permission for ${journal_path} is missing."
        exit 2
    fi
else
    echo "The ${journal_path} does not exist."
    exit 1
fi

# Append the current date to the journal
echo "Date: $(date)" >> "${journal_path}"
echo -n "Please enter your name: "
read user_name
echo "Name: ${user_name}" >> "${journal_path}"

# Journal entry loop
echo "Type your journal entry (empty line to finish):"
while read line && [ -n "$line" ]; do
    echo "${line}" >> "${journal_path}"
done

# Append a separator
echo "${journal_path}"
echo "Journal entry recorded."
exit 0

