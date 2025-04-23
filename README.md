Arduino Communication with Visual Studio (C++) 
Overview
This project involves using Visual Studio to communicate between a PC and an Arduino microcontroller. The program allows you to send mathematical equations (eg, 34 *74) from the PC to the microcontroller, which performs calculations. The results are then saved to a text file.

Requirements
Arduino IDE installed

Visual Studio installed

Arduino microcontroller

Instructions
1. Connect the Arduino board to the PC.
   
2. Upload the Arduino Program to the Microcontroller
Compile and upload the Arduino program to the microcontroller using the Arduino IDE.
The program must be installed on the microcontroller before it can communicate with the PC.

3. Build the Visual Studio Program under the name Calpart1,3.
Once the program in Visual Studio is built, it will automatically detect the port to which the Arduino is connected.

4. Ping the Microcontroller
The program will send a "hello" message to the microcontroller and wait for a response.

If the microcontroller responds, the laptop will detect and capture the corresponding port.

5. Increase Sleep Time if No Response
If the program doesn't receive a response from the microcontroller, increase the sleep time to give more time for the communication to be established.

6. Establish Connection and Input Equations
Once the connection is successfully established, the program will prompt the user to input a mathematical equation.

The program supports basic arithmetic operations: addition, subtraction, multiplication, and division.

Only natural numbers (integers) are allowed as input (no decimals), but the result may be a decimal.

7. Enter Two Equations
The program will ask the user to input two equations.

Both equations will be calculated on the microcontroller.

8. Save Results
The results of both calculations will be saved in a text file.

An Arduino log file will also be generated, containing the full sequence of communication between the microcontroller and the PC.

File Structure
ArduinoProgram.ino - Arduino code to upload to the microcontroller.

VisualStudioProgram - Source code for the Visual Studio application.

output.txt - File where calculation results are saved.

ArduinoLog.txt - Log file capturing the communication sequence.
