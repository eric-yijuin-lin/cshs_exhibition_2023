// this sample code provided by www.programmingboss.com
// Define analog input
#define ANALOG_IN_PIN A0
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0;

void setup() {
  Serial.begin(9600);
}
void loop() {
  // Read the Analog Input
  adc_value = 0;
  for (int i=0; i<10; i++) {
    adc_value += analogRead(ANALOG_IN_PIN);
    delay(100);
  }
  
  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 10240.0; 
  
  // Calculate voltage at divider input
  in_voltage = adc_voltage / (R2/(R1+R2)) ; 
  
  // Print results to Serial Monitor to 2 decimal places
  String str = String(in_voltage, 2);
  Serial.println(str);
  // Serial.println("Hello Boss");
  delay(1500);
}