
#define IRpin_PIN      PIND
#define IRpin          2

#define MAXPULSE 65000
 

#define RESOLUTION 20 
 
uint16_t pulses[100][2];  
uint8_t currentpulse = 0; 
 
void setup(void) {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
}
 
void loop(void) {
  uint16_t highpulse, lowpulse; 
  highpulse = lowpulse = 0; 
  
  

    while (IRpin_PIN & (1 << IRpin)) {

     highpulse++;
     delayMicroseconds(RESOLUTION);
 
     if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
       printpulses();
       currentpulse=0;
       return;
     }
  }
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse;
  
  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
     // pin is still LOW
     lowpulse++;
     delayMicroseconds(RESOLUTION);
     if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
       printpulses();
       currentpulse=0;
       return;
     }
  }
  pulses[currentpulse][1] = lowpulse;
 
  currentpulse++;
}
 
void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }
  
  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF ");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    //Serial.print("\t"); // tab
    Serial.print("pulseIR(");
    Serial.print(pulses[i][1] * RESOLUTION , DEC);
    Serial.print(");");
    Serial.println("");
    //Serial.print("\t");
    Serial.print("delayMicroseconds(");
    Serial.print(pulses[i+1][0] * RESOLUTION , DEC);
    Serial.println(");");
 
  }
  //Serial.print("\t"); // tab
  Serial.print("pulseIR(");
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION, DEC);
  Serial.print(");");
 
}
