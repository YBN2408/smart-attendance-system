

#include <SoftwareSerial.h>

// Bluetooth Module
SoftwareSerial bluetooth(10, 11);

// Keypad Connections
const int R1 = 9;
const int R2 = 8;
const int R3 = 7;
const int R4 = 6;

const int C1 = 5;
const int C2 = 4;
const int C3 = 3;
const int C4 = 2;

// Keypad Mapping
char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Valid IDs
String validIDs[] = {
  "1234",
  "5678",
  "1111"
};

String enteredID = "";

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Row Pins as OUTPUT
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  // Column Pins as INPUT_PULLUP
  pinMode(C1, INPUT_PULLUP);
  pinMode(C2, INPUT_PULLUP);
  pinMode(C3, INPUT_PULLUP);
  pinMode(C4, INPUT_PULLUP);

  Serial.println("Smart Attendance System Started");
  bluetooth.println("Bluetooth Attendance System Started");
}

void loop()
{
  char key = scanKeypad();

  if (key != '\0')
  {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    delay(300);

    // Submit Attendance
    if (key == '#')
    {
      bool valid = false;

      for (int i = 0; i < 3; i++)
      {
        if (enteredID == validIDs[i])
        {
          valid = true;
          break;
        }
      }

      if (valid)
      {
        Serial.println("Attendance Marked");

        bluetooth.print("ID: ");
        bluetooth.println(enteredID);
        bluetooth.println("Attendance Marked");
      }
      else
      {
        Serial.println("Invalid User");

        bluetooth.print("ID: ");
        bluetooth.println(enteredID);
        bluetooth.println("Invalid User");
      }

      enteredID = "";
    }

    // Clear Input
    else if (key == '*')
    {
      enteredID = "";

      Serial.println("Input Cleared");
      bluetooth.println("Input Cleared");
    }

    // Store Digits
    else
    {
      enteredID += key;

      Serial.print("Entered ID: ");
      Serial.println(enteredID);

      bluetooth.print("Typing ID: ");
      bluetooth.println(enteredID);
    }
  }
}

// Function to Scan Keypad Manually
char scanKeypad()
{
  int rowPins[4] = {R1, R2, R3, R4};
  int colPins[4] = {C1, C2, C3, C4};

  for (int row = 0; row < 4; row++)
  {
    // Set all rows HIGH
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, HIGH);
    digitalWrite(R4, HIGH);

    // Pull one row LOW
    digitalWrite(rowPins[row], LOW);

    for (int col = 0; col < 4; col++)
    {
      if (digitalRead(colPins[col]) == LOW)
      {
        return keys[row][col];
      }
    }
  }

  return '\0';
}