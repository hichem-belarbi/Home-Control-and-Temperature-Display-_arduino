#include <LiquidCrystal.h>
#include <Servo.h>

// Define the pin numbers
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
const int ledPin = 5;
const int servoPin = 6;
const int tempPin = A0;

const int uppin = 4;
const int downpin = 2;
const int okpin = 3;

// Booleans to record the state of the door and the lamp
int light_state = 0, door_state = 0;

int up_state = HIGH;
int down_state = HIGH;
int ok_state = HIGH;

// Initialize the objects
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myServo;
int index = 1; // Current position
int totalOptions = 7; // Total number of options

float temp;

// List of menu items
char menuItems[7][15] = {
    "Close the door",
    "Open the door",
    "Door state",
    "Temperature",
    "Light on",
    "Light off",
    "Light state"
};

void setup() {
    Serial.begin(9600);
    // define pins modes 
    pinMode(ledPin, OUTPUT);
    pinMode(uppin, INPUT_PULLUP);
    pinMode(downpin, INPUT_PULLUP);
    pinMode(okpin, INPUT_PULLUP);

    myServo.attach(servoPin);
    lcd.begin(16, 2);
    
    
    lcd.setCursor(0, 0);
    lcd.print("Home control");
    lcd.setCursor(0, 1);
    lcd.print("and Temp...");
    delay(2000);

    show_menu();
}

void loop() {
    up_state = digitalRead(uppin);
    down_state = digitalRead(downpin);
    ok_state = digitalRead(okpin);
    
    if (up_state == LOW) {
        up_btn_clicked();
        delay(200);
    } 
    else if (down_state == LOW) {
        down_btn_clicked();
        delay(200);
    } 
    else if (ok_state == LOW) {
        ok_btn_clicked();
        delay(200);
    }
}

void show_menu() {
    lcd.clear();

    int firstIndex = index - 1; // Selected item at the top
    int secondIndex = firstIndex + 1; // Next item

    // Display the first item with ">"
    lcd.setCursor(0, 0);
    lcd.print("> ");
    lcd.print(menuItems[firstIndex]);

    // Display the second item (if it exists)
    if (secondIndex < totalOptions) {
        lcd.setCursor(2, 1); // Offset to avoid displaying ">"
        lcd.print(menuItems[secondIndex]);
    }
}

void up_btn_clicked() {
    if (index > 1) {
        index--;
        show_menu();
    }
}

void down_btn_clicked() {
    if (index < totalOptions) {
        index++;
        show_menu();
    }
}

void ok_btn_clicked() {
    lcd.clear();
    switch (index) {
        // Close the door
        case 1:
            if (door_state == 1) {
                myServo.write(90);
                door_state = 0;
                lcd.print("door closed!");
            } else {
                lcd.print("already closed!");
            }
            delay(2000);
            break;

        // Open the door
        case 2:
            if (door_state == 0) {
                myServo.write(0);
                door_state = 1;
                lcd.print("Door is open.");
            } else {
                lcd.print("already open!");
            }
            delay(2000);
            break;

        // Check the door state
        case 3:
            lcd.print("the Door is:");
            lcd.setCursor(0, 1);
            lcd.print(door_state == 1 ? "    OPEN!" : "    CLOSED!");
            delay(2000);
            break;

        // Check the temperature
        case 4:
            temp = ((analogRead(tempPin) / 1024.0) * 5.0 - 0.5) * 100.0;
            lcd.print("temp: ");
            lcd.print(temp);
            lcd.print(" C");

            for (int i = 0; i < 11; i++) {
                lcd.setCursor(0, 1);
                lcd.print(10 - i); // Fixed loop iteration index
                lcd.print(" ");
                delay(500);
            }
            lcd.setCursor(0, 1);
            lcd.print("Returning...");
            delay(2000);
            break;

        // Switch the light on
        case 5:
            if (light_state == 0) {
                digitalWrite(ledPin, HIGH);
                lcd.print("switched ON.");
                light_state = 1;
            } else {
                lcd.print("already ON!");
            }
            delay(2000);
            break;

        // Switch the light off
        case 6:
            if (light_state == 1) {
                digitalWrite(ledPin, LOW);
                lcd.print("switched OFF.");
                light_state = 0;
            } else {
                lcd.print("already OFF!");
            }
            delay(2000);
            break;

        // Check the light state
        case 7:
            lcd.print("the light is:");
            lcd.setCursor(0, 1);
            lcd.print(light_state == 1 ? "    ON!" : "    OFF!");
            delay(2000);
            break;
    }

    delay(1000);
    show_menu();
}
