void setup() {
  // 1. Hardware USB port for PC connection (115200, Parity Even)
  Serial.begin(115200, SERIAL_8E1);

  // 2. Hardware Serial2 port assigned to Pin 4 (D4) for the Mares dive computer
  Serial2.begin(115200, SERIAL_8E1, 4, 4, false);
  Serial2.setPins(4, 4, -1, -1); 
}

void loop() {
  // Data from PC (USB) -> to Mares (Pin 4)
  if (Serial.available()) {
    int bytesWritten = 0;
    
    // Forward packets from USB directly to the dive computer line
    while (Serial.available()) {
      Serial2.write(Serial.read());
      bytesWritten++;
    }
    
    // Wait for the hardware transmission to complete (flush the shift register)
    Serial2.flush(); 
    
    // PERFECT ECHO CANCELLATION:
    // We know exactly how many bytes were sent. We wait briefly and remove 
    // the identical number of bytes from the RX buffer so they do not return to Subsurface.
    delayMicroseconds(bytesWritten * 90); 
    while (bytesWritten > 0 && Serial2.available() > 0) {
      Serial2.read();
      bytesWritten--;
    }
  }

  // Data from Mares (Pin 4) -> to PC (USB)
  if (Serial2.available()) {
    while (Serial2.available()) {
      Serial.write(Serial2.read());
    }
    Serial.flush(); // Ensure Windows receives a clean, lag-free data stream
  }
}
