#!/bin/bash
# run_unit_tests.sh
# Interactive PlatformIO test runner for Arduino libraries
# Place this script in the project root, next to platformio.ini

set -e

PROJECT_DIR="$(pwd)"  # assumes running from project root
TEST_DIR="$PROJECT_DIR/test"

# Discover all unit test environments (ending with _unit)
ALL_UNIT_ENVS=()
for env_dir in "$TEST_DIR"/*_unit; do
    [ -d "$env_dir" ] && ALL_UNIT_ENVS+=("$(basename "$env_dir")")
done

# Discover all environments from platformio.ini (simplified example)
ALL_ENVS=("RumpshiftLogger_unit" "WiFiNetworkManager_unit") # update as needed

echo -e "\nSelect test mode:"
echo "1) Run all unit tests (no upload)"
echo "2) Run specific test environment"
read -rp "Enter choice [1/2]: " MODE

if [[ "$MODE" == "1" ]]; then
    echo -e "\n=== Running all unit tests (without uploading) ===\n"
    for env in "${ALL_UNIT_ENVS[@]}"; do
        echo -e "\n--- Running $env ---"
        pio test -e "$env" -d "$PROJECT_DIR" -v --without-uploading
        if [ $? -ne 0 ]; then
            echo -e "\033[0;31mTests failed for $env\033[0m"
        else
            echo -e "\033[0;32mTests passed for $env\033[0m"
        fi
    done
else
    echo -e "\nAvailable environments:"
    select ENV in "${ALL_ENVS[@]}"; do
        if [ -n "$ENV" ]; then
            echo "Selected: $ENV"
            break
        else
            echo "Invalid selection. Try again."
        fi
    done

    read -rp "Do you want to upload to device after testing? [y/N]: " UPLOAD
    UPLOAD_FLAG=""
    if [[ "$UPLOAD" =~ ^[Yy]$ ]]; then
        UPLOAD_FLAG=""
    else
        UPLOAD_FLAG="--without-uploading"
    fi

    echo -e "\n--- Running $ENV ---"
    pio test -e "$ENV" -d "$PROJECT_DIR" -v $UPLOAD_FLAG
    if [ $? -ne 0 ]; then
        echo -e "\033[0;31mTests failed for $ENV\033[0m"
    else
        echo -e "\033[0;32mTests passed for $ENV\033[0m"
    fi
fi

echo -e "\nAll requested tests completed.\n"
