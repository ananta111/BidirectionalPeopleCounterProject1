const int triggerDistance = 20;
const int triggerThreshold = 10;

bool eventStart1 = false;
bool eventStart2 = false;

// ultrasound 1
const int trigPin1 = 7;
const int echoPin1 = 6;
const int ledPin1 = 2;
int d1;

// ultrasound 2
const int trigPin2 = 11;
const int echoPin2 = 10;
const int ledPin2 = 3;
int d2;

int counter = 0;

bool decreasing = false;
int lowPnts1 = 0;
int lowPnts2 = 0;

void setup() {
 
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);


  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
 
  Serial.begin(9600);
  resetEcho(trigPin1, echoPin1);
  resetEcho(trigPin2, echoPin2);
}

void loop() {
 
  d1 = measureDistance(trigPin1, echoPin1);
  lowPnts1 = detectSpike(d1, lowPnts1);
  if(lowPnts1 >= triggerThreshold){
    eventStart1 = true;
  }
  
  if(eventStart1){
    digitalWrite(ledPin1, HIGH);
  }else{
    digitalWrite(ledPin1, LOW);
  }
  
  if (eventStart2 && eventStart1){
    Serial.println("EXIT");
    eventStart1 = false;
    eventStart2 = false;
    counter = counter - 1;
    Serial.print("Number of people: ");
    Serial.println(counter);
    resetEcho(trigPin1, echoPin1);
  }
  //------------------------------------------
  d2 = measureDistance(trigPin2, echoPin2);
  lowPnts2 = detectSpike(d2, lowPnts2);
  if(lowPnts2 >= triggerThreshold){
    eventStart2 = true;
  }
  if(eventStart2){
    digitalWrite(ledPin2, HIGH);
  }else{
    digitalWrite(ledPin2, LOW);
  }
  
  if (eventStart1 && eventStart2){
    Serial.println("ENTER");
    eventStart1 = false;
    eventStart2 = false;
    counter = counter + 1;
    Serial.print("Number of people: ");
    Serial.println(counter);
    resetEcho(trigPin2, echoPin2);
  }

  Serial.print(d1);
  Serial.print("\t");
  Serial.println(d2);
}

float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  float dur=pulseIn(echoPin,HIGH);

  return dur*0.034/2;
}

int detectSpike(int distance, int lowPnts){
  if (distance > 0){  
    if(distance < triggerDistance){
      lowPnts = lowPnts + 1;    
    }else{
      lowPnts = 0;
    }
  }
  return lowPnts;
}

void resetEcho(const int trig, const int echo){  
  int distance = 0; 
  bool reset = false;
  int highPnts = 0;
  Serial.println("Resetting");
  while(!reset){
    distance = measureDistance(trig, echo);
    Serial.println(distance);
    if(distance > triggerDistance){
      highPnts = highPnts + 1;    
    }else{
      highPnts = 0;
    }

    if(highPnts >= triggerThreshold){
      reset = true;
    }
  }
}
