const int inPin = A0;

const int pwrPinB = 2; 
const int outPin = 3; 
const int wrkLedPin = 11;
const int lokLedPin = 12;
const int pwrLedPin = 13; 

int    gap = 80;
int  reset = 2000;
int bell_long =500;
String myPass = "11211";

int  state = 6; //0:無鈴聲,1:按鈴,2:亞按鈴,3:開門
unsigned long timer = 0;
unsigned long keyTimer = 0;
unsigned long resetTimer = 0;

int mode = 1; //1: 工作模式(按鈴後, 3秒後開門) , 0:待命模式
int inVal = 0;
int keyTime=0;
int bellTime=0;
int bellType=0;
String empty = "";
String password="";
String output = "";
void setup() {
  Serial.begin(9600);
  Serial.println(empty+" "+mode+" Boot!");
  pinMode(outPin, OUTPUT);
  pinMode(pwrLedPin, OUTPUT);
  pinMode(wrkLedPin, OUTPUT);
  pinMode(lokLedPin, OUTPUT);

}

void loop() {
  // read the analog in value:
  if(millis()<100){
    digitalWrite(pwrLedPin, HIGH);
    digitalWrite(wrkLedPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    digitalWrite(pwrPinB, HIGH);
    return;
  }
  inVal = analogRead(inPin);
  if(state==0) { //Ｎo bell 0
    if(inVal>=300) {
      state=1;
      keyTimer=millis();
    }
    if(resetTimer>0 && millis()-resetTimer>reset){
      
      if(myPass==password){
        mode=!mode;
        Serial.println(empty+" "+mode+" Pass");
        state = (mode) ? 4 : 5;
      }else{
        Serial.println(empty+" "+mode+" Fail");
        if(mode){
          state=3;
        }
      }
      
      //Serial.println(password);
      resetTimer=0;
      password="";
    }
  }
  if(state==1) { //belling 1
    if(inVal<10) { //goto not sure bell 2
      state=2;
      timer=millis(); 
    }    
  }
  if(state==2) { //not sure bell...
    if(inVal>=10 ) { //belling continue
      state=1; 
    }
    if(inVal<10){
      bellTime=millis()-timer;
      if(timer>0 && bellTime>gap){ //no belling for gap millis, return no bell 0
        keyTime = millis()-keyTimer;
        bellType = (keyTime>bell_long) ? 2 : 1;
        password += bellType;
        Serial.print(bellType);  
        keyTimer=0;
        state=0;
        timer=0;
        resetTimer=millis();
      }
    }
  }
  if(state==3) { //開門...
    delay(2000);
    digitalWrite(outPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    delay(300);
    digitalWrite(outPin, LOW);
    digitalWrite(lokLedPin, LOW);
    state=7;
    return;
  }
  if(state==4) { //長開門 啟動工作模式...
    digitalWrite(wrkLedPin, HIGH);
    digitalWrite(outPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    delay(1500);
    digitalWrite(outPin, LOW);
    digitalWrite(lokLedPin, LOW);
    state=7;
    return;
  }
  if(state==5) { //開門*2 進入待命模式...
    digitalWrite(wrkLedPin, LOW);
    digitalWrite(outPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    delay(300);
    digitalWrite(lokLedPin, LOW);
    digitalWrite(outPin, LOW);
    delay(200);
    digitalWrite(lokLedPin, HIGH);
    digitalWrite(outPin, HIGH);
    delay(300);
    digitalWrite(lokLedPin, LOW);
    digitalWrite(outPin, LOW);
    state=0;
    return;
  }
  if(state==6) { //開機動畫...
    digitalWrite(wrkLedPin, HIGH);
    digitalWrite(pwrLedPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    delay(300);
    digitalWrite(wrkLedPin, LOW);
    digitalWrite(lokLedPin, LOW);
    delay(300);
    digitalWrite(wrkLedPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    delay(300);
    digitalWrite(wrkLedPin, LOW);
    digitalWrite(lokLedPin, LOW);
    delay(200);
    digitalWrite(wrkLedPin, HIGH);
    digitalWrite(lokLedPin, HIGH);
    delay(200);
    digitalWrite(wrkLedPin, LOW);
    digitalWrite(lokLedPin, LOW);
    delay(400);
    digitalWrite(wrkLedPin, HIGH);
    state=0;
    return;
  }
  if(state==7) { //休息5秒...
    digitalWrite(wrkLedPin, LOW);
    delay(5000);
    digitalWrite(wrkLedPin, HIGH);
    state=0;
    return;
  }
  output = empty + "i= " + (((unsigned long)millis())/100 % 50) + ", " + state;
  //Serial.println(output);      
}
