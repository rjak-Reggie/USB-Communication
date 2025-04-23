#include <Windows.h>     // Required for serial communication on Windows
#include <iostream>      // Standard input/output stream
#include <string>        // For string manipulation
#include <fstream>       // For file operations (log/result saving)


using namespace std;

// Function to send a ping to the Arduino and check if it responds correctly
bool isArduinoResponding(HANDLE hSerial) {
    DWORD bytesWritten, bytesRead;
    char buffer[128] = { 0 };

    // Message to send to Arduino
    string ping = "hello\n";

    // Open the log file in append mode
    ofstream logFile("arduino_log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << "[transmit - Ping] Sent: " << ping;
    }

    // Send the ping message
    WriteFile(hSerial, ping.c_str(), ping.length(), &bytesWritten, NULL);

    // Give Arduino some time to respond
	Sleep(100); // Reduced for quicker response

    // Read the response from Arduino
    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';  // Null-terminate the buffer
        string response(buffer);   // Convert to string

        if (logFile.is_open()) {
            logFile << "[receive - Ping] Received: " << response << "\n";
            logFile.close();
        }

        return response.find("Arduino here!") != string::npos;
    }

    if (logFile.is_open()) {
        logFile << "[receive - Ping] No response\n";
        logFile.close();
    }

    return false;// If nothing or wrong message received
}

// Function to scan COM1 to COM30 and return handle to the correct Arduino port
HANDLE findArduinoPort() {
    for (int i = 1; i <= 30; ++i) {  // Loop through COM1 to COM30
        wstring comPort = L"\\\\.\\COM" + to_wstring(i);  // Format as Windows COM port path

        // Try opening the COM port
        HANDLE hSerial = CreateFile(
            comPort.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
        );

        // If port is successfully opened
        if (hSerial != INVALID_HANDLE_VALUE) {
            // Set up serial parameters
            DCB dcbSerialParams = { 0 };
            dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

            // Get current serial port settings
            if (GetCommState(hSerial, &dcbSerialParams)) {
                dcbSerialParams.BaudRate = CBR_9600;      // Set baud rate to 9600
                dcbSerialParams.ByteSize = 8;             // 8 data bits
                dcbSerialParams.StopBits = ONESTOPBIT;    // 1 stop bit
                dcbSerialParams.Parity = NOPARITY;        // No parity
                SetCommState(hSerial, &dcbSerialParams);  // Apply settings

                // Configure timeouts
                COMMTIMEOUTS timeouts = { 0 };
				timeouts.ReadIntervalTimeout = 10; // Reduced for quicker response
				timeouts.ReadTotalTimeoutConstant = 10; // Reduced for quicker response
                timeouts.ReadTotalTimeoutMultiplier = 5; // Reduced for quicker response
                SetCommTimeouts(hSerial, &timeouts);

                // Allow Arduino to reset
                Sleep(2000); // Reduced for quicker response

                // Check if this port is connected to an Arduino
                if (isArduinoResponding(hSerial)) {
                    wcout << L"Arduino detected on COM" << i << endl;
                    return hSerial;  // Return valid handle
                }
            }

            // If not Arduino, close handle and continue
            CloseHandle(hSerial);
        }
    }

    // If no port responded correctly
    cerr << "Arduino not found on any COM port." << endl;
    return INVALID_HANDLE_VALUE;
}

// Main program entry point
int main() {
	
    // Automatically find the Arduino port
    HANDLE hSerial = findArduinoPort();
    if (hSerial == INVALID_HANDLE_VALUE) return 1;  // Exit if not found

    // Open files for logging
    ofstream logFile("arduino_log.txt", ios::app);
    ofstream resultFile("arduino_result.txt", ios::app);

    // Loop for sending two expressions
    for (int i = 1; i <= 2; ++i) {
        float a, b;     // Operands
        char op;        // Operator (+, -, *, /)

        // Prompt user to enter an expression
        cout << "\nOperator can only be (+, -, *, /) ";
        cout << "\nEnter expression #" << i << " (e.g. 34 * 72): ";
       
        cin >> a >> op >> b;

        // Format the data string to send to Arduino
        string dataToSend = "a = " + to_string((int)a) + ", b = " + to_string((int)b) + " and op = " + op + "\n";
        DWORD bytesWritten;

        // Send the data to the Arduino
        WriteFile(hSerial, dataToSend.c_str(), dataToSend.length(), &bytesWritten, NULL);
        cout << "Sent: " << dataToSend;

        // Log the sent message
        if (logFile.is_open()) logFile << "[transmit  #" << i << "] " << dataToSend;

        // Allow Arduino time to calculate
        Sleep(2000); // Reduced for quicker response

        // Prepare to receive response
        char buffer[256];
        DWORD bytesRead;
        string response;

        while (true) {
            // Read data from Arduino
            if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';  // Null-terminate
                response += buffer;        // Append to full response
                cout << buffer;            // Display output to user

                // Log received data
                if (logFile.is_open()) logFile << "[receive #" << i << "] " << buffer;

                // If full result or error has been received
                if (response.find("Result: ") != string::npos ||
                    response.find("Error: ") != string::npos) {

                    // Log results to separate file
                    if (resultFile.is_open()) {
                        resultFile << "[Calculation " << i << "]\n";
                        resultFile << "Sent to Arduino: " << dataToSend;
                        resultFile << "Arduino response: " << response << "\n\n";
                    }
                    break;  // Exit reading loop
                }
            }

            Sleep(10);  // Prevent CPU overuse
        }
    }

    // Close file and serial handles
    if (logFile.is_open()) logFile.close();
    if (resultFile.is_open()) resultFile.close();
    CloseHandle(hSerial);

    // done
    cout << "\nData saved to arduino_log.txt and arduino_result.txt\n";
    return 0;
}
