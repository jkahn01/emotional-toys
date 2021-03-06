bool sample = false;
bool toggle1 = false;
int pulse = 0;
int data;
String output;
int counter;

void setup() {
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  // denominator is timer frequency
  OCR1A = 15625/30;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, HIGH);
  digitalWrite(12,HIGH);
  counter = 0;
  Serial.begin(9600);
}

void loop() {
  if(sample) {// && counter < 200) {
    data = map(analogRead(A2),0,1023,0,1000);
    output = int2str(data);
    Serial.println(output);
    sample = false;
  }
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  //pulse++;
  if(pulse == 20) {
    if (toggle1){
      digitalWrite(13,HIGH);
      digitalWrite(12,HIGH);
      toggle1 = 0;
    } else{
      digitalWrite(13,LOW);
      digitalWrite(12,LOW);
      toggle1 = 1;
    }
    pulse = 0;
  }
  sample = true;
  counter++;
}

String int2str(int data)
{
  String out = "S";
  if(data < 10) {
    out += "00";
    out += String(data);
  } else if(data < 100) {
    out += "0";
    out += String(data);
  } else {
    out += String(data);
  }
  return out;
}

