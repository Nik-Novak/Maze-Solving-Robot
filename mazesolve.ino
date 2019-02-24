const int steppinR = 3;
int dirpinR = 4;

const int steppinL = 6;
int dirpinL = 7;


float left=200, right=200, front=200;

int trigPinL = 31;
int echoPinL = 33;
int trigPinR = 35;
int echoPinR = 37;
int trigPinF = 39;
int echoPinF = 41;

int Flimit = 30;
int Slimit = 30;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(steppinR,OUTPUT);
  pinMode(dirpinR,OUTPUT);
  pinMode(steppinL,OUTPUT);
  pinMode(dirpinL,OUTPUT);
  digitalWrite(dirpinL, HIGH);
  digitalWrite(dirpinR,LOW);
  //range finder init
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
}
int ft = 590;
boolean justturned = false;

void loop() { 

  //delay(1000);
  forward(ft*10000/4,steppinL, steppinR,true);
  
  //interrupted by change
  if(right > Slimit){

    if(!justturned){
      
    
    forward(200,steppinL,steppinR,false);
    //move slightly forward
    turn(ft/4, steppinR);
    justturned = true;
    //move 1 foot forward
    //forward(ft/2,steppinL,steppinR,false);
    return;

    }
  }

  if(front < Flimit){
    turn(ft/4, steppinL);
  }
  if(front < Flimit && front > 0)
    turn(ft/4, steppinL);
}

void turn(int deg, int steppin){
  delay(1000);
  switch(steppin){
    case steppinL:
      digitalWrite(dirpinL, HIGH);
      digitalWrite(dirpinR,HIGH);
      break;
    case steppinR:
      digitalWrite(dirpinL, LOW);
      digitalWrite(dirpinR,LOW);
      break;
  }
  int N = int(1.0*200*deg/360);
  for (int i=0; i!=N; i++){
    stepmot(steppinL, 6000);
    stepmot(steppinR, 6000);
  }
  delay(1000);
}

void forward(int deg, int steppinL, int steppinR,boolean doSense){
  //delay(1000);
  digitalWrite(dirpinL, HIGH);
  digitalWrite(dirpinR,LOW);
  int senseCount = 1;
  int N = int(1.0*200*deg/360);
  boolean senstog = true;
  for (int i=0; i!=N; i++){
    stepmot(steppinL, 6000);
    stepmot(steppinR, 6000);
    senstog = !senstog;
    if(!doSense)
      continue;
    if(senstog){
      if(sense(1))
       break;
    }
    else if(sense(2))
      break;
  }
}

void stepmot(int pin, int spd){
  int dly = constrain(spd,300,4000);
   digitalWrite(pin, HIGH);
   delayMicroseconds(dly);
   digitalWrite(pin, LOW);
   delayMicroseconds(dly);
}


//int senseinit = 100;
boolean sense(int dir){ //dir(0) = left, dir(1) = right, dir(2) = front
  int pin = 31 + 4*dir;
  float duration, distance;
  digitalWrite(pin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  
  duration = pulseIn(pin+2, HIGH, 15000);
  distance = (duration / 2) * 0.0344;
  if(duration == 0)
    distance = -1;
  Serial.println(distance);
  switch(dir){
    case 0: left = distance;
            return (distance > Slimit);
      break;
    case 1: right = distance;
            //Serial.println("RIGHT TRIP");
            Serial.println((distance > Slimit));
            if( distance < Slimit){
              justturned = false;
              return false;
            }
            return (distance > Slimit);
      break;
    case 2: front = distance;
            //Serial.println("FRONT TRIP");
            return (distance < Flimit);
      break;
  }
  
  
  
}

