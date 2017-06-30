long duration;
int cm;
const int echo = 7;
const int trig = 8;
int distanceList[60]={};
bool doPing=true;

void setup()
{
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
    for (int i=0;i<15;i++){
      
      if(doPing){
        ping();
        doPing=false;
    }
    read_pulse();
    if(duration!=0){
        distanceList[i]=duration/29.1/2;
        Serial.print("Distance: ");
  for (int i=0;i<60;i++){
   Serial.print(distanceList[i]);
   Serial.print(" ");
  }
  
  Serial.println("finish");
        
        doPing=true;
    }
    }
    delayMicroseconds(2);
}
void ping(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
}

void read_pulse()
{
    static unsigned long rising_time;  // time of the rising edge
    static int last_state;             // previous pin state
    int state = digitalRead(echo);      // current pin state
    duration = 0; // default return value

    // On rising edge: record current time.
    if (last_state == LOW && state == HIGH) {
        rising_time = micros();
    }

    // On falling edge: report pulse length.
    if (last_state == HIGH && state == LOW) {
        unsigned long falling_time = micros();
        duration = falling_time - rising_time;
    }

    last_state = state;
}
