#include "WaveletArray.h"

const int RATE = 64; //Hertz
const int LEVELS = 7;
const int SCALE_FACTOR = 64; // raw input values can be scaled up by this factor for higher precision
const float LOW_CUTOFF = 0.4; // No frequencies below this
const float HIGH_CUTOFF = 8.0; // No frequencies above this
const int MAX_CHANGE = 5;

int data;
int lastRate;

// dwt variables, transformArray and temp used to be unsigned
WaveletArray rawInput;
int transformArray[WINDOWLENGTH];
int temp[WINDOWLENGTH];
float freqs[LEVELS];
float freq_widths[LEVELS];
float powers[LEVELS];

void setup() {
  //set timer1 interrupt at RATE Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for RATE hz increments
  // denominator is timer frequency
  OCR1A = 15625/RATE-1;// = (16*10^6) / (RATE*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  Serial.begin(9600);
  lastRate = 0;
}

// The data sampling is handled by the interrupt. Every time 
// enough samples have accumulated for one snapshot,
// copy to transformArray and call runTransform to find the heart rate
void loop() {
  if(rawInput.getLength() == WINDOWLENGTH) {
    noInterrupts();
    rawInput.getArray(transformArray);
    rawInput.newWindow();
    interrupts();
    int rate = runTransform();
    if(lastRate == 0 || abs(rate-lastRate) <= MAX_CHANGE) {
      lastRate = rate;
    } else if(rate > lastRate) {
      lastRate += MAX_CHANGE;
    } else {
      lastRate -= MAX_CHANGE;
    }
    Serial.println(lastRate); 
  }
}

// Interrupt routine, runs at a frequency of RATE
ISR(TIMER1_COMPA_vect) {
  data = analogRead(A0);
  rawInput.appendValue(data);
}

// call several functions to determine the heart rate

// returns the calculated heart rate
int runTransform() {
  wavedec();
  decodeArray();
  return getRate();
} 

// Perform a multilevel 1D discrete wavelet decomposition using the Haar wavelet
void wavedec() {
  
  int length = WINDOWLENGTH >> 1;
  for(int i=0; i<LEVELS; i++) {
    for(int j=0; j<length; j++) {
      int sum = transformArray[j * 2] + transformArray[j * 2 + 1];
      int diff = transformArray[j * 2] - transformArray[j * 2 + 1];
      temp[j] = sum;
      temp[j + length] = diff;
    }
    
    for(int j=0; j < length << 1; j++) {
      transformArray[j] = temp[j];
    }
    length >>= 1;
  } 
}

// Extract meaningful information from the wavelet transform. Each level of the
// decomposition is assigned an average frequency and a power, which get placed
// in separate arrays
void decodeArray() {
  float nyquist = RATE / 2;
  for(int i=1; i<= LEVELS; i++) {
    float lower_limit = nyquist / power(2, LEVELS-i+1);
    float upper_limit = nyquist / power(2, LEVELS-i);
    freqs[i-1] = (lower_limit + upper_limit) / 2.0;
    
    if(i != 1) {
      freq_widths[i-1] = freqs[i-1] - freqs[i-2];
    }
    if(freqs[i-1] <= HIGH_CUTOFF && freqs[i-1] >= LOW_CUTOFF) {
      powers[i-1] = transformMean(i);
    } else {
      powers[i-1] = 0;
    }
  }
}

// Take the geometric center of the frequency vs. power plot 
// (as if it's a solid shape)

// returns the core frequency
int getRate() {
  float rate = 0;
  float numerator = 0;
  float denominator = 0;
  
  for(int i=1; i<LEVELS; i++) {
    numerator += (power(freqs[i]-freqs[i-1], 2) / 6.0) * (2.0 * powers[i] + powers[i-1]) * freq_widths[i];
    delay(1); // Really shouldn't need this but Arduino throws a weird error
    denominator += ((freqs[i]-freqs[i-1])/2.0)*(powers[i] + powers[i-1]) * freq_widths[i];
  }
  
  rate = numerator / denominator;
  return 60*rate;
}
  
// perform a basic exponent operation given the base and exponent
// return base^(exponent)
float power(float base, int exponent) {
  float result = 1;
  for(int i=0; i<exponent; i++) {
    result *= base;
  }
  return result;
}  

// find the average value of a section of the wave decomposition
// use the fact that each section is as long as all the sections before it 
// combined, and starts at position 2^(LEVELS-sectionIndex+1)

// sectionIndex: number of the section who's mean is to be calculated
// returns the average of the specified section
float transformMean(int sectionIndex) {
  int first = WINDOWLENGTH / power(2, LEVELS-sectionIndex+1); 
  int length = first;
  unsigned int sum = 0;
  for(int i=first; i<first+length; i++) {
    sum += abs(transformArray[i]);
  }
  float mean = sum / float(length);
  return mean;
}

// calculate the raw mean of the transform array (should only be 
// called before wavedec, or results will be meaningless)

// returns the average value of the transform array
float rawMean() {
  long sum = 0;
  for(int i=0; i<WINDOWLENGTH; i++) {
    sum += transformArray[i];
  }
  float numSamples = WINDOWLENGTH;
  return sum/numSamples;
}
