# Password-Based Security Door Lock on ATmega16

## Overview
This project implements a password-based security door lock system using two ATmega32 microcontrollers operating at 8MHz. The system is designed with a layered architecture to separate user interaction from control logic.

## System Architecture
- **HMI_ECU (Human Machine Interface)**
  - Microcontroller: ATmega32
  - Peripherals: 2x16 LCD, 4x4 Keypad
  - Role: Handles user interaction, taking inputs via the keypad and displaying messages on the LCD.
  
- **Control_ECU**
  - Microcontroller: ATmega32
  - Peripherals: EEPROM, Buzzer, DC Motor
  - Role: Processes inputs, validates passwords, controls door mechanisms, and manages system security.

## System Workflow

### Step 1: Create a System Password
1. LCD prompts: “Please Enter Password.”
2. User enters a 5-digit password, displayed as `*` on the screen.
3. Press the enter button.
4. Re-enter the password for confirmation.
5. If passwords match, they are saved in EEPROM; if not, repeat this step.

### Step 2: Main Options
The LCD displays the following options:
- `+`: Open Door
- `-`: Change Password

### Step 3: Open Door (`+`)
1. LCD prompts: “Please Enter Password.”
2. User enters the password.
3. Password is sent to Control_ECU for validation.
4. If matched:
   - Motor rotates CW for 15 seconds, unlocking the door.
   - Motor holds for 3 seconds.
   - Motor rotates A-CW for 15 seconds, locking the door.

### Step 4: Change Password (`-`)
1. LCD prompts: “Please Enter Password.”
2. User enters the current password.
3. If validated, repeat Step 1 to set a new password.

### Step 5: Invalid Password Handling
1. If the entered password is incorrect:
   - User gets two additional attempts to enter the correct password.
   - On the third incorrect attempt:
     - Buzzer activates for 1 minute.
     - Error message displayed on the LCD for 1 minute.
     - System locks out further inputs during this period.
   - After the timeout, the system returns to the main options menu.

## Components
- **ATmega32 Microcontrollers** (x2)
- **16x2 LCD Display**
- **4x4 Keypad**
- **EEPROM**
- **Buzzer**
- **DC Motor**
- **UART Communication** between HMI_ECU and Control_ECU

