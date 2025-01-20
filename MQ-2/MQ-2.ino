// Define the pin connections
const int analogPin = A0; // Analog output from MQ-2
const int digitalPin = 2; // Digital output from MQ-2
const int ledPin = 4;    // Optional: An LED to indicate gas presence (onboard LED)

// Threshold for gas detection (adjust based on your needs)
const int gasThreshold = 600;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize digital input for the MQ-2 digital pin
  pinMode(digitalPin, INPUT);

  // Initialize LED
  pinMode(ledPin, OUTPUT);

  Serial.println("MQ-2 Gas Sensor Initialized...");
}

void loop() {

  digitalWrite(7,HIGH);
  // Read the analog value from the MQ-2 sensor
  int analogValue = analogRead(analogPin);
  
  // Read the digital signal from the MQ-2 sensor (0 or 1)
  int digitalValue = digitalRead(digitalPin);

  // Display readings on the Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Digital Value: ");
  Serial.println(digitalValue);

  // Trigger LED if gas concentration exceeds threshold
  if (analogValue > gasThreshold || digitalValue == HIGH) {
    Serial.println("Warning: Gas concentration detected!");
    digitalWrite(ledPin, HIGH); // Turn LED ON
  } else {
    digitalWrite(ledPin, LOW); // Turn LED OFF
  }

  // Delay to make readings more stable
  delay(1000);
}
