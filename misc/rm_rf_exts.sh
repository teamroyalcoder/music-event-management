#!/bin/bash

read -p "Enter root directory [./]: " root_directory
root_directory=${root_directory:-"./"}

read -p "Enter file extensions (comma-separated) [.save]: " file_extensions
file_extensions=${file_extensions:-".save"}

IFS=',' read -ra extensions <<< "$file_extensions"

echo -e "\nPerforming recursive file deletion in '$root_directory' with file extensions: ${extensions[*]}\n"

delete_files() {
    local current_directory="$1"

    for entry in "$current_directory"/*; do
        if [[ -f "$entry" ]]; then
            for extension in "${extensions[@]}"; do
                if [[ "$entry" == *"$extension" ]]; then
                    echo "Deleting file: ${entry#"$root_directory"/}"
                    rm "$entry"
                    break
                fi
            done
        elif [[ -d "$entry" ]]; then
            delete_files "$entry"
        fi
    done
}

delete_files "$root_directory"
