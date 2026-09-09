void setup() {
  // 1. Sprzętowy port USB do połączenia z komputerem PC (115200, Parity Even)
  Serial.begin(115200, SERIAL_8E1);

  // 2. Sprzętowy port Serial2 przypisany do pinu 4 (D4) dla zegarka Mares
  Serial2.begin(115200, SERIAL_8E1, 4, 4, false);
  Serial2.setPins(4, 4, -1, -1); 
}

void loop() {
  // Dane z PC (USB) -> do Maresa (Pin 4)
  if (Serial.available()) {
    int bytesWritten = 0;
    
    // Przesyłamy pakiety z USB bezpośrednio na linię zegarka
    while (Serial.available()) {
      Serial2.write(Serial.read());
      bytesWritten++;
    }
    
    // Czekamy na sprzętowe zakończenie nadawania (opróżnienie rejestru przesuwnego)
    Serial2.flush(); 
    
    // PERFEKCYJNE WYCINANIE ECHA:
    // Wiemy dokładnie, ile bajtów wysłaliśmy. Czekamy chwilę i usuwamy z bufora wejściowego
    // dokładnie taką samą liczbę bajtów, aby nie trafiły do programu Subsurface jako echo.
    delayMicroseconds(bytesWritten * 90); 
    while (bytesWritten > 0 && Serial2.available() > 0) {
      Serial2.read();
      bytesWritten--;
    }
  }

  // Dane z Maresa (Pin 4) -> do PC (USB)
  if (Serial2.available()) {
    while (Serial2.available()) {
      Serial.write(Serial2.read());
    }
    Serial.flush(); // Upewniamy się, że Windows dostał czysty strumień bez opóźnień
  }
}
