#!/bin/bash

# Arduino PlatformIO Helper Script
# Usage: ./pio_helper.sh [command] [options]

# Default values
DEFAULT_BAUD=9600
PORT=""

# NEW: Default PlatformIO values
DEFAULT_BOARD="uno_r4_wifi"
DEFAULT_PLATFORM="renesas-ra"
DEFAULT_FRAMEWORK="arduino"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color


# Try to read first uncommented [env:...] from platformio.ini
extract_ini_env() {
    awk '
        /^[[:space:]]*;/ { next }                              # skip comments
        /^[[:space:]]*\[env:[^]]+\]/ {
            sub(/^[[:space:]]*\[env:/, "")
            sub(/\][[:space:]]*$/, "")
            print
            exit
        }
    ' platformio.ini
}

# Function to get PlatformIO environment
get_pio_env() {
    local ENV_NAME=""
    # Step 1: Extract from platformio.ini
    ENV_NAME=$(extract_ini_env)

    if [ -n "$ENV_NAME" ]; then
        echo "$ENV_NAME"
        return
    fi

    # Step 2: Load from .env file
    if [ -f .env ]; then
        ENV_NAME=$(grep -E '^PIO_ENV=' .env | cut -d '=' -f2 | tr -d '[:space:]')
        if [ -n "$ENV_NAME" ]; then
            echo "$ENV_NAME"
            return
        fi
    fi

    # Step 3: fallback to default
    echo "uno_wifi_rev2"
}


# Function to detect Arduino port automatically
detect_port() {
    echo -e "${BLUE}Detecting Arduino port...${NC}"
    
    # Look for common Arduino port patterns
    PORTS=($(ls /dev/cu.usbmodem* 2>/dev/null))
    
    if [ ${#PORTS[@]} -eq 0 ]; then
        echo -e "${RED}No Arduino ports found!${NC}"
        echo "Make sure your Arduino is connected."
        return 1
    elif [ ${#PORTS[@]} -eq 1 ]; then
        PORT=${PORTS[0]}
        echo -e "${GREEN}Found port: $PORT${NC}"
        return 0
    else
        echo -e "${YELLOW}Multiple ports found:${NC}"
        for i in "${!PORTS[@]}"; do
            echo "  $((i+1)). ${PORTS[i]}"
        done
        read -p "Select port (1-${#PORTS[@]}): " choice
        if [[ $choice =~ ^[0-9]+$ ]] && [ $choice -ge 1 ] && [ $choice -le ${#PORTS[@]} ]; then
            PORT=${PORTS[$((choice-1))]}
            echo -e "${GREEN}Selected port: $PORT${NC}"
            return 0
        else
            echo -e "${RED}Invalid selection${NC}"
            return 1
        fi
    fi
}

# Function to show usage
show_usage() {
    echo "Arduino PlatformIO Helper Script"
    echo ""
    echo "Usage: $0 [command] [options]"
    echo ""
    echo "Commands:"
    echo "  upload, u           - Build and upload code"
    echo "  monitor, m          - Start serial monitor"
    echo "  both, b             - Upload then monitor"
    echo "  clean, c            - Clean build files"
    echo "  build, d            - Build only (no upload)"
    echo "  list, l             - List available devices"
    echo "  kill, k             - Kill processes using serial ports"
    echo "  new <dir>           - Create a new Arduino project (with override options)"
    echo ""
    echo "Options:"
    echo "  -p, --port PORT     - Specify port (e.g., /dev/cu.usbmodemXXXX)"
    echo "  -b, --baud RATE     - Specify baud rate (default: 9600)"
    echo "  -h, --help          - Show this help"
    echo ""
    echo "Examples:"
    echo "  $0 upload"
    echo "  $0 monitor -p /dev/cu.usbmodemF412FA9F37082"
    echo "  $0 both -b 115200"
    echo "  $0 upload -p /dev/cu.usbmodemF412FA9F37082"
    echo "  $0 new MyProject"
    echo "  $0 new MyProject --board nano_every --platform atmelmegaavr --framework arduino"
}

# Parse command line arguments
COMMAND=""
ARGS=()
BAUD=$DEFAULT_BAUD

while [[ $# -gt 0 ]]; do
    case "$1" in
        upload|u|monitor|m|both|b|clean|c|build|list|l|kill|k|new|n)
            COMMAND="$1"
            shift
            ;;
        -p|--port)
            PORT="$2"
            shift 2
            ;;
        -b|--baud)
            BAUD="$2"
            shift 2
            ;;
        -h|--help)
            show_usage
            exit 0
            ;;
        *)
            ARGS+=("$1")
            shift
            ;;
    esac
done

# If no command provided, show usage
if [ -z "$COMMAND" ]; then
    show_usage
    exit 1
fi

# Execute commands
case $COMMAND in
    upload|u)
        PIO_ENV=$(get_pio_env)
        echo -e "${BLUE}Using environment: ${GREEN}$PIO_ENV${NC}"
        echo -e "${BLUE}Building and uploading...${NC}"
        pio run --target upload -e "$PIO_ENV"
        ;;
    
    monitor|m)
        if [ -z "$PORT" ]; then
            detect_port || exit 1
        fi
        PIO_ENV=$(get_pio_env)
        echo -e "${BLUE}Using environment: ${GREEN}$PIO_ENV${NC}"
        echo -e "${BLUE}Starting serial monitor on $PORT at ${BAUD} baud...${NC}"
        echo -e "${YELLOW}Press Ctrl+C to exit monitor${NC}"
        pio device monitor --port "$PORT" --baud "$BAUD"
        ;;
    
    both|b)
        PIO_ENV=$(get_pio_env)
        echo -e "${BLUE}Using environment: ${GREEN}$PIO_ENV${NC}"
        echo -e "${BLUE}Building and uploading...${NC}"
        pio run --target upload -e "$PIO_ENV"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}Upload successful! Starting monitor...${NC}"
            sleep 2  # Give the Arduino time to reset
            if [ -z "$PORT" ]; then
                detect_port || exit 1
            fi
            echo -e "${YELLOW}Press Ctrl+C to exit monitor${NC}"
            pio device monitor --port "$PORT" --baud "$BAUD"
        else
            echo -e "${RED}Upload failed!${NC}"
            exit 1
        fi
        ;;
    
    clean|c)
        PIO_ENV=$(get_pio_env)
        echo -e "${BLUE}Using environment: ${GREEN}$PIO_ENV${NC}"
        echo -e "${BLUE}Cleaning build files...${NC}"
        pio run --target clean -e "$PIO_ENV"
        ;;
    
    build|d)
        PIO_ENV=$(get_pio_env)
        echo -e "${BLUE}Using environment: ${GREEN}$PIO_ENV${NC}"
        echo -e "${BLUE}Building project...${NC}"
        pio run -e "$PIO_ENV"
        ;;
    
    list|l)
        echo -e "${BLUE}Available devices:${NC}"
        pio device list
        ;;
    
    kill|k)
        echo -e "${BLUE}Killing processes using serial ports...${NC}"
        pkill -f "pio device monitor" 2>/dev/null
        pkill -f "cu.usbmodem" 2>/dev/null
        pkill -f platformio 2>/dev/null
        echo -e "${GREEN}Done! Serial ports should be free now.${NC}"
        ;;
    new)
        if [ -z "${ARGS[0]}" ]; then
            echo -e "${RED}Please specify a project name.${NC}"
            echo "Example: $0 new my_project"
            exit 1
        fi

        PROJECT_NAME="${ARGS[0]}"
        shift

        BOARD="$DEFAULT_BOARD"
        PLATFORM="$DEFAULT_PLATFORM"
        FRAMEWORK="$DEFAULT_FRAMEWORK"

        # Parse overrides
        while [[ $# -gt 0 ]]; do
            case $1 in
                --board)
                    BOARD="$2"
                    shift 2
                    ;;
                --platform)
                    PLATFORM="$2"
                    shift 2
                    ;;
                --framework)
                    FRAMEWORK="$2"
                    shift 2
                    ;;
                *)
                    echo -e "${RED}Unknown option for 'new': $1${NC}"
                    exit 1
                    ;;
            esac
        done

        echo -e "${BLUE}Creating new PlatformIO project: ${GREEN}$PROJECT_NAME${NC}"
        echo -e "${BLUE}Board:${NC} $BOARD  ${BLUE}Platform:${NC} $PLATFORM  ${BLUE}Framework:${NC} $FRAMEWORK"

        # Create project directory and initialize PlatformIO
        mkdir -p "$PROJECT_NAME"
        cd "$PROJECT_NAME" || exit 1
        echo -e "${BLUE}Initializing PlatformIO project...${NC}"
        pio project init --board "$BOARD" --project-option="platform=$PLATFORM" --project-option="framework=$FRAMEWORK"

        # Create src dir and write main.cpp directly
        mkdir -p src
cat << 'EOF' > src/main.cpp
#include <Arduino.h>

void setup() {
    // Initialize serial communication at 9600 baud
    Serial.begin(9600);
    Serial.println("Hello, PlatformIO!");
}

void loop() {
    // Your code here
    delay(1000);
    Serial.println("Looping...");
}
EOF

        echo -e "${GREEN}Created src/main.cpp with skeleton Arduino code${NC}"

        # Create include dir and write config.h template
        mkdir -p include
cat << 'EOF' > include/config.h
// ============================================================================
// config.h - Project Configuration Header
// ============================================================================
//
// This file contains all environment-specific settings for your project.
// Update these values before compiling. Keep secrets (like Wi-Fi passwords)
// out of version control when possible (use environment variables or .gitignore).
//
// ============================================================================

#ifndef CONFIG_H
#define CONFIG_H

// -----------------------------------------------------------------------------
// Wi-Fi Configuration
// -----------------------------------------------------------------------------
#define WIFI_SSID   "your_wifi_ssid"   // Replace with your Wi-Fi SSID
#define WIFI_PASS   "your_wifi_pass"   // Replace with your Wi-Fi password

// Alternative Wi-Fi networks (uncomment as fallback)
// #define WIFI_SSID "BackupSSID"
// #define WIFI_PASS "BackupPassword"

// -----------------------------------------------------------------------------
// Serial Configuration
// -----------------------------------------------------------------------------
#define BAUD_RATE   9600               // Baud rate for Serial communication

// -----------------------------------------------------------------------------
// Network Configuration
// -----------------------------------------------------------------------------
#define LAN_IP      "192.168.1.100"    // Static LAN IP of this device (optional)
#define PORT        8000               // Port for server communication

// -----------------------------------------------------------------------------
// API Configuration
// -----------------------------------------------------------------------------
#define API_PATH    "/api/notion/log/" // API endpoint path
#define TARGET_API  "notion"           // Target API (e.g., "notion", "influx", etc.)
#define SOURCE      "device_name"      // Device identifier (e.g., "coffee_grinder")

// Database IDs (useful for Notion or other APIs)
#define DATABASE_ID "your_database_id_here"
#define DB_ID       "your_secondary_db_id_here"

// -----------------------------------------------------------------------------
// Debugging Configuration
// -----------------------------------------------------------------------------
#define DEBUG_LEVEL LOG_LEVEL_DEBUG    // Options: LOG_LEVEL_ERROR, LOG_LEVEL_WARN,
//                                      // LOG_LEVEL_INFO, LOG_LEVEL_DEBUG

// -----------------------------------------------------------------------------
// Notes:
// - Consider using a secrets.h file (gitignored) for sensitive values like Wi-Fi.
// - This template can be extended for different targets (MQTT, REST, etc.).
// -----------------------------------------------------------------------------

#endif // CONFIG_H
EOF

        echo -e "${GREEN}Created include/config.h with template settings${NC}"

        # Optional: show contents of src/
        echo -e "${BLUE}Contents of src/:${NC}"
        ls -l src

        echo -e "${GREEN}Project initialized in ./$PROJECT_NAME${NC}"
        echo -e "${BLUE}Contents of src/:${NC}"
        ls -l src


        # Ask about common.ini setup
        read -p "Would you like to create a common.ini for RumpusArduinoLibrary support? (y/n): " create_common
        if [[ "$create_common" =~ ^[Yy]$ ]]; then
            read -p "Enter full path to your RumpusArduinoLibrary/libraries directory: " rumpus_path
            read -p "Enter Arduino upload port (leave empty to auto-detect later): " upload_port

            # Write common.ini
            # Write common.ini
cat <<EOF > ../common.ini
[env]
lib_extra_dirs = $rumpus_path
EOF

            # Only add upload_port if user entered it
            if [ -n "$upload_port" ]; then
                echo "upload_port = $upload_port" >> ../common.ini
            fi

            # Inject extra_configs into platformio.ini
            echo "" >> platformio.ini
            echo "[platformio]" >> platformio.ini
            echo "extra_configs = ../common.ini" >> platformio.ini

            echo -e "${GREEN}common.ini created at ../common.ini${NC}"
            echo -e "${GREEN}platformio.ini updated to include extra_configs${NC}"
        fi


        echo -e "${YELLOW}Next: cd $PROJECT_NAME && edit src/main.cpp${NC}"
        exit 0
        ;;


    *)
        echo -e "${RED}Unknown command: $COMMAND${NC}"
        show_usage
        exit 1
        ;;
esac