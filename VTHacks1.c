#include <Servo.h>

void Error(int);
int Configuring(int, int, int, int);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int FlexPin1 = 10;
int FlexPin2 = 11;
int FlexPin3 = 12;
int FlexPin4 = 13;

  enum { OpenConfig, CloseConfig, OpenSuccess, CloseSuccess, MAP } state;
  int initOp1, initOp2, initOp3, initOp4, initCl1, initCl2, initCl3, initCl4,
      TotValCl1, TotValCl2, TotValCl3, TotValCl4, TotValOp1, TotValOp2, TotValOp3, TotValOp4,
      AveValOp1, AveValOp2, AveValOp3, AveValOp4, AveValCl1, AveValCl2, AveValCl3, AveValCl4;
  int RED1 = 2;
  int RED2 = 4;
  int GREEN1 = 3;
  int GREEN2 = 5;
  
  int Flex1 = 0;
  int Flex2 = 0;
  int Flex3 = 0;
  int Flex4 = 0;
  

void setup()
{
 
 Serial.begin(9600); //baud

 servo1.attach(10);
 servo2.attach(11);
 servo3.attach(12);
 servo4.attach(13);
 
 servo1.write(0);
 servo2.write(0);
 servo3.write(0);
 servo4.write(0);
 
 pinMode(RED1, OUTPUT);
 pinMode(GREEN1, OUTPUT);
 pinMode(RED2, OUTPUT);
 pinMode(GREEN2, OUTPUT);
 
 Configuring(FlexPin1, FlexPin2, FlexPin3, FlexPin4);
 
}

void loop()
{
 
 /*testing*/
 int deg1 = analogRead(FlexPin2);
 Serial.println(deg1);
 
 Flex1 = analogRead(FlexPin1);
 Flex2 = analogRead(FlexPin2);
 Flex3 = analogRead(FlexPin3);
 Flex4 = analogRead(FlexPin4);
 
 int DegreeFlex1 = map(Flex1, AveValOp1, AveValCl1, 0, 180);
 int DegreeFlex2 = map(Flex2, AveValOp2, AveValCl2, 0, 180);
 int DegreeFlex3 = map(Flex3, AveValOp3, AveValCl3, 0, 180);
 int DegreeFlex4 = map(Flex4, AveValOp4, AveValCl4, 0, 180);
 
 servo1.write(DegreeFlex1);
 servo2.write(DegreeFlex2);
 servo3.write(DegreeFlex3);
 servo4.write(DegreeFlex4);
}


/*************************************************************************************************************/
/*FUNCTIONS*/

int Configuring(int FlexPin1, int FlexPin2, int FlexPin3, int FlexPin4){
  while(1){
    switch (state){
      
    case OpenConfig: // must take 30 consec. inputs with +-20 tolerance from initial reading/////////////////////
      
        digitalWrite(RED1, HIGH);
        delay(2000);//ms
        digitalWrite(RED2, HIGH);
        int Op1[30];
        int Op2[30];
        int Op3[30];
        int Op4[30];
        
        initOp1 = analogRead(FlexPin1);
        initOp2 = analogRead(FlexPin2);
        initOp3 = analogRead(FlexPin3);
        initOp4 = analogRead(FlexPin4);
        delay(100);
        digitalWrite(RED2, LOW);
        int i;
    for (i = 0; i<30; i++){
            Op1[i] = analogRead(FlexPin1);
            Op2[i] = analogRead(FlexPin2);
            Op3[i] = analogRead(FlexPin3);
            Op4[i] = analogRead(FlexPin4);
          if((abs(initOp1 - Op1[i]) < 20) && (abs(initOp2 - Op2[i]) < 20) && (abs(initOp3 - Op3[i]) < 20) && (abs(initOp4 - Op4[i]) < 20)){
                if(i == 29) state = OpenSuccess;
                }
        else Error(1); //1 for OpenConfig error; 0 for CloseConfig error
        }
        break;
  
    case CloseConfig: /////////////////////////////////////////////////////////
        digitalWrite(GREEN1, LOW);
    digitalWrite(RED2, HIGH);
    delay(2000);//ms
        digitalWrite(RED1, HIGH);
        initCl1 = analogRead(FlexPin1);
        initCl2 = analogRead(FlexPin2);
        initCl3 = analogRead(FlexPin3);
        initCl4 = analogRead(FlexPin4);
        delay(100);
        digitalWrite(RED1, LOW);
    int Cl1[30];
        int Cl2[30];
        int Cl3[30];
        int Cl4[30];
        int j;
    for (j = 0; j<30; j++){
            Cl1[j] = analogRead(FlexPin1);
            Cl2[j] = analogRead(FlexPin2);
            Cl3[j] = analogRead(FlexPin3);
            Cl4[j] = analogRead(FlexPin4);
      if((abs(initCl1 - Cl1[j]) < 20) && (abs(initCl2 - Cl2[j]) <20) && (abs(initCl3 - Cl3[j]) <20) && (abs(initCl4 - Cl4[j]) < 20)){
        if(j == 29) state = CloseSuccess;
      }
      else Error(0); //pass 0 as identifier for closedconfig error
      }
      break;
  
  
    case OpenSuccess:////////////////////////////////////////////////////////////////
    digitalWrite(RED1, LOW);
    digitalWrite(GREEN1, HIGH);
    delay(2000);
    state = CloseConfig;
    break;
  
    case CloseSuccess://///////////////////////////////////////////////////////////////
    digitalWrite(RED2, LOW);
    digitalWrite(GREEN2, HIGH);
    delay(2000);
    state = MAP;
    break;
  
    case MAP://///////////////////////////////////////////////////////////////////////////
    digitalWrite(GREEN2, LOW);
    delay(1000);
        int m;
    for(m = 0; m<2; m++){ // BLINK SUCCESS LIGHTS
              digitalWrite(GREEN1, HIGH);
          digitalWrite(GREEN2, HIGH);
          delay(200);
          digitalWrite(GREEN1, LOW);
          digitalWrite(GREEN2, LOW);
          delay(200);
          }
        int k = 0;
    for (k; k<30; k++){
          TotValOp1 += Op1[k];
              TotValOp2 += Op2[k];
              TotValOp3 += Op3[k];
              TotValOp4 += Op4[k];

          TotValCl1 += Cl1[k];
              TotValCl2 += Cl2[k];
              TotValCl3 += Cl3[k];
              TotValCl4 += Cl4[k];
    }
  
        AveValOp1 = TotValOp1 / 30;
        AveValOp2 = TotValOp2 / 30;
        AveValOp3 = TotValOp3 / 30;
        AveValOp3 = TotValOp4 / 30;
     
        AveValCl1 = TotValCl1 / 30;
        AveValCl2 = TotValCl2 / 30;
        AveValCl3 = TotValCl3 / 30;
        AveValCl4 = TotValCl4 / 30;
        digitalWrite(GREEN1, HIGH);
        digitalWrite(GREEN2, HIGH);
        digitalWrite(RED1, HIGH);
        digitalWrite(RED2, HIGH);
        return 0;
        break;
        
      }//end switch
      
    }//end while
    
  }//end function


  void Error (int x){
  int n = 0;
  for(n; n<2; n++){ // BLINK ERROR LIGHTS
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, HIGH);
    delay(200);
    digitalWrite(RED1, LOW);
    digitalWrite(RED2, LOW);
    delay(200);
    }
  }