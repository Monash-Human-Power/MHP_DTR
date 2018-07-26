
// =============================== INFORMATION FOR USE ==============================================================
// Description: 
// -) This code was set up to help Chasis team measure current and voltage produced by a generator
//    attached to the Drive Train Testing Rig (DTR).  
// -) A rider rides to spin the wheel on the testing rig. This wheel is linked through a roller to a motor
//    which acts as a generator to produce electricity. 
// -) The output voltage of the generator is measured using a resistor divider of 7.5k and 30k ohms. 
//    This divides the output voltage of the generator from 0-25V down to 0-5V. Please ensure that the generator
//    does not produce more than 25V, or the arduino will be irreperably damaged. It is advised to always
//    can check the output voltage produced by the generator using a multimeter across the generator terminals.
// -) The output currernt of the generator is measured using a ACS712 current sensor. This chip is on a
//    breakout board for convenient hookup to an arduino. The sensor measures the input current and produces
//    an output voltage relative to the input current. The readings from the sensor can be understood as follows:
//      1) An output voltage of 0.5V means the input current is -30A
//      2) An output voltage of 2.5V means the input current is 0A
//      3) An output voltage of 4.5V means the input current is 30A
//    Please ensure the input current does not exceed -30A or 30A.

// Connections:
//  -) Please connect the current sensor to 5V and GND. Connect Vout to Arduino input pin A0.
//  -) Please connect the voltage sensor to GND. Connect the "S" output to Arduino input pin A1.


// ======================================== VARIABLES =============================================================

int collected_samples = 5;    // Defines the number of samples average on each reading of the current and voltage
float R1 = 30000.0;           // Value of resistor R1 in voltage measurement sensor
float R2 = 7500.0;            // Value of resistor R2 in voltage measurement sensor
int I_Reading = 0;            // Variable to store the ADC reading of the current
int V_Reading = 0;            // Variable to store the ADC reading of the voltage
float I_ADC = 0;            // Variable to hold Current ADC Measurements
float V_ADC = 0;            // Variable to hold Voltage ADC Measurements
float I_Ave;              // Variable to hold Average of Current Measurements
float V_Ave;              // Variable to hold Average of Voltage Measurements
unsigned long start;      // Variable to record the start time
float time_elapsed;    // Variable to record the time elapsed

// User defined variable. Change this to change the print out.
int plx_daq = 0;

// 1) Define plx_daq = 1 if you want to print arduino data to excel.
// 2) Define plx_daq = 0 if you would like to see a nice output in the arduino serial terminal
 
// ======================================= SET UP CODE ==============================================================

void setup() 
{
  // Set up the serial link between the Arduino and the Computer
  Serial.begin(9600);

  if (plx_daq == 1)
  { 
    // Command required by PLX-DAQ to clear columns in Excel
    Serial.println("CLEARDATA");

    // Label columns in the excel file
    Serial.println("LABEL, Computer Time, Time Elapsed (s), Current (A), Voltage (V)");
  }
  else
  {
    // Print Column Headings on Serial Monitor
    Serial.println("TIME\t\tCURRENT (A)\t\tVOLTAGE (V)");// set the titles for the columns
  }
  
  // Record the start time before doing the main loop
  start = millis();
}

// ===================================== START THE MAIN CODE ===================================

void loop() 
{
  // Collect "collected_samples" number of samples
  for (int x = 0; x < collected_samples; x++)
  {
    I_Reading = analogRead(A0);                                 // Read the value of the Analog to Digital Converter (ADC)
    I_ADC += ((I_Reading / 1024.0 * 5.0) - 2.5) * 60.0 / 4.0;   // Convert the ADC reading to a current measurement.

    V_Reading = analogRead(A1);                                 // Read the value of the ADC
    V_ADC += V_Reading / 1024.0 * 5.0 * (R1 + R2) / R2;         // Convert the output of the ADC to a voltage measurement.
   
    delay(1);   // Delay a few milliseconds to allow the ADCs to settle
  }

  // Average the collected_samples number of readings
  I_Ave = I_ADC / collected_samples;      // Average all collected current readings 
  V_Ave = V_ADC / collected_samples;      // Average all collected voltage readings
  
  // Calculate the elapsed time in seconds
  time_elapsed = (millis()-start) / 1000.0;

  // Print data in excel with 4 decimal places
  if (plx_daq == 1)
  {
    Serial.print("DATA,TIME,");           // Needed for the excel software
    Serial.print(time_elapsed, 4);                // Print time elapsed
    Serial.print(", ");                   // Add some spacing on the line
    Serial.print(I_Ave, 4);                  // Print current reading
    Serial.print(", ");                   // Add some spacing on the line
    Serial.print(V_Ave, 4);                  // Print voltage reading
    Serial.println();
  }
  // Print data in serial monitor to 4 decimal places
  else
  {
    Serial.print(time_elapsed, 4);
    Serial.print("\t\t");
    Serial.print(I_Ave, 4);
    Serial.print("\t\t");
    Serial.println(V_Ave, 4);
  }
  
  // Reset the variables for the next loop
  I_ADC = 0;
  V_ADC = 0;
  I_Ave = 0;
  V_Ave = 0;

  // Optional 1 second delay to slow the program down for debugging. Uncomment this in if you would like it enabled.
  //delay(1000);
}

// ================================================== END OF ARDUINO SCRIPT ============================

