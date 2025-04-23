# USB-Communication
communication over USB with Arduino UNO board
The requirements for this program are a laptop and a microcontroller.
Both Arduino IDE and Visual Studio need to be installed on the computer.

The board must be connected to the PC, and the microcontroller must be uploaded with the code microcontroller_code in the Arduino IDE.
This code will enable communication between the PC and the microcontroller.

The C++ program in Visual Studio should be built and run under the name calpart1,3.

The program will search for the Arduino board by pinging every port and waiting for a reply from the microcontroller.

Once the port is detected, the program will prompt the user to input equations. The microcontroller will process the equations, output the results, and the PC application will store the results.

All communication between the microcontroller and the PC is logged in the Arduino_log, and the results are saved under Arduino_results.
If the Serial Monitor is open in the Arduino IDE, Visual Studio cannot communicate with the Arduino board.

If the port cannot be detected, increasing the sleep time in the PC application will increase the chance of the Arduino microcontroller responding to the query.


