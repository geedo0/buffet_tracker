#include <AltSoftSerial.h>

#define DEBUG

AltSoftSerial altSerial;

const int weight_sensor = A0;

void debug_println(String out) {
  #ifdef DEBUG
  Serial.println(out);
  #endif
}

int sample_weight()

void setup() {
  Serial.begin(9600);
  altSerial.begin(9600);
  if(Serial) {
    debug_println("Setup function complete");
  }
}

void loop() {
  String plate_id = "";
  debug_println("Loop function started");
  
  //Detect and fetch the RFID if available
  if(altSerial.available()) {
    debug_println("Serial event detected");
    boolean stringIncomplete = true;
    while (altSerial.available()) {
      char inChar = (char)altSerial.read();
      if((inChar != 0x02) && stringIncomplete) {
        plate_id += inChar;
        if (inChar == '\x0d') {
          stringIncomplete = false;
        }
      }
    }
  }
  debug_println("Plate id: "+plate_id);

  //Sample the initial weight
  int weight_accumulator = 0;
  for(int ii=0; ii<8; ii++) {
    weight_accumulator += analogRead(weight_sensor);
  }
  
  float initial_weight = weight_accumulator
  int initial_weight_raw = analogRead(weight_sensor);
  
  //TODO: Needs proper calibration
  int initial_weight = map(initial_weight_raw, 400, 1000, 0, 255);
  debug_println("
  delay(500);

  

  /*
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  //value fromlow from high to low to high
  outputValue = map(sensorValue, 400, 1000, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:
  Serial.print("sensor = " );
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(500);
  */
}
