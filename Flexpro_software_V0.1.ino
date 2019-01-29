/*certain sensors are missing, code currently loops indefinitely without allowing the user to take out the cartridge, vibration time may be subjected to changes in the future
*/

const int silicaBottom = 12; //pin connected to solenoid at the bottom of the silica measuring compartment
const int silicaTop1 = 14; //pin connected to Hbridge in1
const int silicaTop2 = 15; //pin connected to Hbridge in2
const int silicaTopClosed = A7;
const int silicaTopOpen = ??; //this button needs to be added to the design
const int cartridgeLoaded = ??;  //sensor needs to be connected
const int Step = 2;  //pin connected to STEP, written "Step" because "step" is already defined in a library somewhere
const int dir = 1; //pin connected to DIRECTION, may not be needed at all
const int barricade = 13; //pin connected to slide barricade
const int wheel = 18; //pin connected to wheel actuator, might need to be 19 instead
const int vibration = 9;  //pin connected to vibrating mechanism
const int lift = 39;  //pin connected to motor lifting the cartridge
const int liftUpSensor = A9;  //pin connected to sensor detecting cartridge has been lifted
const int liftDownSensor = ??;  //pin connected to sensor detecting cartridge has been lowered
const int silicaFull = A4;  //pin connected to sensor reading silica compartment contents

attachInterrupt(digitalPinToInterrupt(53), countUp, FALLING);  //interrupt when rod passes counting sensor, calls "countUp" function


volatile int rods = 0; //int to keep track of rods count
volatile int steps = 0;  //int to keep track of step count

void countUp (){  //countUp function, called by interrupt
  rods +=1;
  if (rods % 4 == 0){       //slowly tilt cartridge back as rod count increases
    digitalWrite(Step, HIGH);
    wait(1ms);
    digitalWrite(Step, LOW);
    steps += 1;
  }
}

void setup() {
  pinMode(silicaBottom, OUTPUT);
  pinMode (silicaTop1, OUTPUT);
  pinMode (silicaTop2, OUTPUT);
  pinMode (silicaTopClosed, INPUT);
  pinMode (silicaTopOpen, INPUT);
  pinMode (cartridgeLoaded, INPUT);
  pinMode (Step, OUTPUT);
  pinMode (dir, OUTPUT);
  pinMode (barricade, OUTPUT);
  pinMode (wheel, OUTPUT);
  pinMode (vibration, OUTPUT);
  pinMode(lift, OUTPUT);
  pinMode(liftUpSensor, INPUT);
  pinMode(liftDownSensor, INPUT);
  pinMode(silicaFull, INPUT);

  int temp = 0;
  digitalWrite (silicaTop1, HIGH);  //start opening top of silica compartment
  while (temp == LOW){ //wait until top of silica compartment is fully open
    temp = digitalRead(silicaTopOpen);
  }
  digitalWrite (silicaTop1, LOW); //stop opening silica compartment
}

void loop() {
  int reading = 0;  //declare reading int for reading sensors, set to LOW
  while (reading == LOW){ //wait until cartridge is loaded
    reading = digitalRead(cartridgeLoaded);
  }
  for (int i = 0; i < 100; i++){  //tilt cartridge
    digitalWrite(Step, HIGH);
    wait(1ms);
    digitalWrite(Step, LOW);
    wait(1ms);
  }
  rods = 0;
  steps = 0;
  digitalWrite(barricade, HIGH);  //raise barricade
  digitalWrite(wheel, HIGH);      //start wheel
  while (rods < 375){ //wait until 375 rods have passed
  }
  digitalWrite(barricade, LOW); //lower barricade
  digitalWrite(wheel, LOW);     //stop wheel
  for(int i = 0; i < 100-steps; i++){ //correct inaccuracies in tilt
    digitalWrite(Step, HIGH);
    wait(1ms);
    digitalWrite(Step, LOW);
    wait(1ms);
  }
  digitalWrite(vibration, HIGH);  //vibrate cartridge for 1 second
  wait(1000ms);
  digitalWrite(vibration, LOW);
  reading = 0;  //set reading to LOW
  digitalWrite(lift, HIGH); //start lifting cartridge
  while (reading == LOW) {  //wait unitl cartridge has arrived
    reading = digitalRead(liftUpSensor);
  }
  digitalWrite(lift, LOW);  //stop lifting cartridge
  reading = 0;  //set reading to LOW
  while (reading == LOW){
    reading = digitalRead(silicaFull);
  }
  reading = 0;  //set reading to LOW
  digitalWrite (silicaTop1, HIGH);  //start closing top of silica compartment
  while (reading == LOW){ //wait until top of silica compartment is fully closed
    reading = digitalRead(silicaTopClosed);
  }
  digitalWrite (silicaTop1, LOW); //stop closing silica compartment
  digitalWrite (silicaBottom, HIGH);  //open bottom of silica compartment
  digitalWrite(vibration, HIGH);  //vibrate mechanism
  wait(20000ms);  //waiting time may be subject to change
  digitalWrite(vibration, LOW); //stop vibrating mechanism
  digitalWrite(silicaBottom, LOW);  //close bottom of silica compartment
  reading = 0;  //set reading to LOW
  digitalWrite (silicaTop1, HIGH);  //start opening top of silica compartment
  while (reading == LOW){ //wait until top of silica compartment is fully open
    reading = digitalRead(silicaTopOpen);
  }
  digitalWrite (silicaTop1, LOW); //stop opening silica compartment
  reading = 0;  //set reading to LOW
  digitalWrite(lift, HIGH); //start lowering cartridge
  while (reading == LOW){ //wait until cartridge is fully lowered
    reading = digitalRead(liftDownSensor);
  }
  digitalWrite(lift, LOW);  //stop lowering cartridge
}
