#include <AltSoftSerial.h>

#define DEBUG

AltSoftSerial altSerial;

const int weight_sensor = A0;
const int rfid_reset = 13;

void debug_println(String out) {
  #ifdef DEBUG
  Serial.println(out);
  #endif
}

int sample_weight() {
  int weight_accumulator = 0;
  //Sample needs to occur quick enough that weight isn't affected by the triggering event
  //Accumulate 8 times to avoid floating point divide
  for(int ii=0; ii<8; ii++) {
    weight_accumulator += analogRead(weight_sensor);
    delay(62);
  }
  //Don't care about units for now calibrate later
  int mean_weight = weight_accumulator >> 3;
  debug_println("Sampled weight: " + String(mean_weight));
  return mean_weight;
}

String get_id_code() {
  boolean stringIncomplete = true;
  String id_code;
  while (altSerial.available()) {
    char inChar = (char)altSerial.read();
    if((inChar != 0x02) && stringIncomplete) {
      id_code += inChar;
      if (inChar == '\x0d') {
        stringIncomplete = false;
      }
    }
  }
  return id_code;
}

void setup() {
  pinMode(rfid_reset, OUTPUT);
  digitalWrite(rfid_reset, HIGH);
  delay(10);
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
    plate_id = get_id_code();
    debug_println("Plate id: "+plate_id);
    int initial_weight = sample_weight();
    //Figure out when the RFID is lifted
    delay(1000);
    while(true) {
      debug_println("Toggling RFID reset");
      digitalWrite(rfid_reset, LOW);
      delay(10);
      digitalWrite(rfid_reset, HIGH);
      delay(1000);
      //Remove junk byte on bus
      altSerial.read();
      String new_plate = get_id_code();
      debug_println("New plate id: " + new_plate);
      if(new_plate != plate_id) {
        debug_println("Plate removed");
        break;
      }
      else {
        debug_println("Plate is still present");
      }
    }
    
  }





  
  delay(500);
}

