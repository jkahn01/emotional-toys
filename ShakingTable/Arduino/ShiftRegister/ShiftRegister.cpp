#include "ShiftRegister.h" //include the declaration for this class

int SER_Pin = 4;   // Input for next value shifted in
int RCLK_Pin = 6;  // Shifts register when pulled High
int SRCLK_Pin = 7; // Sets output when pulled High


ShiftRegister::ShiftRegister(){
  // Default constructor
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);

  setDefaults();
  initArrays();
  
  //reset all register pins
  clearRegisters();
  writeRegisters();
}

ShiftRegister::ShiftRegister(int registers){
  // Specify the number of shift registers wired together
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);

  _number_of_74hc595s = registers; 
  _numOfRegisterPins = _number_of_74hc595s * 8;
  initArrays();
  
  //reset all register pins
  clearRegisters();
  writeRegisters();
}               

ShiftRegister::~ShiftRegister(){
  //Nothing to destruct
}

void ShiftRegister::setDefaults(){
  _number_of_74hc595s = 4; 
  _numOfRegisterPins = _number_of_74hc595s * 8;
}

void ShiftRegister::initArrays(){
  int x = 0;
  for(int i = 0; i < 32; i++){
    if (i >= _numOfRegisterPins){
      x = -1;
    }
    registers[i] = x;
    regVals[i] = x;
  }
}

//set all register pins to LOW
void ShiftRegister::clearRegisters(){
  for(int i = _numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void ShiftRegister::writeRegisters(){
  
  digitalWrite(RCLK_Pin, LOW);

  for(int i = _numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void ShiftRegister::setRegisterPin(int index, int value){
  registers[index] = value;
}


void ShiftRegister::setRegisters(){
 //DEPRECATED??
 for(int i=0;i<_number_of_74hc595s;i++){
   for(int j=0;j<regVals[i];j++){
     registers[8*i+j] = HIGH;
   }
 }
}
