// Initialize an empty string to store incoming serial data
String inputString = "";    

// Flag to indicate when a full line has been received
bool stringComplete = false;

void setup() {
  Serial.begin(9600);             // Start serial communication at 9600 baud
  inputString.reserve(200);       // Pre-allocate 200 bytes for the inputString to avoid memory fragmentation
}

void loop() {
  // Check if a full message has been received
  if (stringComplete) {
    inputString.trim();           // Remove any leading/trailing whitespace or newline

    // Check if the message is a ping (used to identify Arduino connection)
    if (inputString.equalsIgnoreCase("hello")) {
      Serial.println("Arduino here!");  // Respond to ping from PC
    } 

    // Check if the message starts with expression format
    else if (inputString.startsWith("a = ")) {
      // Extract positions of parts in the string
      int aIndex = inputString.indexOf("a = ") + 4;                   // Position after 'a = '
      int bIndex = inputString.indexOf(", b = ");                     // Position of ', b = '
      int opIndex = inputString.indexOf("and op = ");                 // Position of 'and op = '

      // Ensure the positions are valid and in order
      if (aIndex >= 0 && bIndex > aIndex && opIndex > bIndex) {
        // Extract and convert operands
        int a = inputString.substring(aIndex, bIndex).toInt();        // Get integer value of a
        int b = inputString.substring(bIndex + 6, opIndex).toInt();   // Get integer value of b
        char op = inputString.charAt(opIndex + 9);                    // Get operation symbol

        float result = 0.0;         // Variable to store result
        bool valid = true;          // Flag to indicate if operation is valid

        // Perform calculation based on operator
        switch (op) {
          case '+': result = a + b; break;
          case '-': result = a - b; break;
          case '*': result = a * b; break;
          case '/': 
            if (b == 0) {
              Serial.println("Error: Division by zero"); // Handle division by zero
              valid = false;
            } else {
              result = (float)a / b; // Cast to float for accurate division
            }
            break;
          default:
            Serial.println("Error: Invalid operator"); // Handle unknown operator
            valid = false;
        }

        // If calculation was successful, send result
        if (valid) {
          Serial.print("Result: ");
          Serial.println(result, 2);  // Print result with 2 decimal places
        }
      } else {
        Serial.println("Error: Malformed expression");  // Inform of incorrect input format
      }
    } 
    else {
      Serial.println("Unknown command");  // For unrecognized input
    }

    // Clear the input buffer for the next command
    inputString = "";
    stringComplete = false;
  }
}

// This function is called automatically when serial data is available
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();  // Read each character from serial buffer
    inputString += inChar;              // Append to input string

    // When newline is detected, set flag to process the message
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
