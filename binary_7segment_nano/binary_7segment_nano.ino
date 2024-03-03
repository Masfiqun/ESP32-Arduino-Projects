//Declaring the pins for seven segments
// int a = 2;
// int b = 3;
// int c = 5;
// int d = 6;
// int e = 7;
// int f = 10;
// int g = 9;

//Declaring input pins of switch

char inputPin[]={A1,A2,A3,A4};
int state[4];
int outputPin[] = {2,3,5,6,7,10,9};
// digitalWrite(a,HIGH);
// digitalWrite(a,LOW);
//     digitalWrite(b,LOW);
//     digitalWrite(c,HIGH);
//     digitalWrite(d,LOW);
//     digitalWrite(e,LOW);
//     digitalWrite(f,HIGH);
//     digitalWrite(g,LOW);
int signalVal[10][7]={
  {0, 0, 0, 0, 0, 0, 1}, // 0
  {1, 0, 0, 1, 1, 1, 1}, // 1
  {0, 0, 1, 0, 0, 1, 0}, // 2
  {0, 0, 0, 0, 1, 1, 0}, // 3
  {1, 0, 0, 1, 1, 0, 0}, // 4
  {0, 1, 0, 0, 1, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 1, 1, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 1, 0, 0}
  };



void setup() {
  Serial.begin(9600);

  //setting pins for output
  pinMode(outputPin[0],OUTPUT);
  pinMode(outputPin[1],OUTPUT);
  pinMode(outputPin[2],OUTPUT);
  pinMode(outputPin[3],OUTPUT);
  pinMode(outputPin[4],OUTPUT);
  pinMode(outputPin[5],OUTPUT);
  pinMode(outputPin[6],OUTPUT);
  pinMode(8,OUTPUT);

  //setting the pins for switches input
  pinMode(inputPin[0],INPUT);
  pinMode(inputPin[1],INPUT);
  pinMode(inputPin[2],INPUT);
  pinMode(inputPin[3],INPUT);

}
void readInput(){
  for(int i=0; i<4; i++)
  {
    state[i] = digitalRead(inputPin[i]);
  //   Serial.println(digitalRead(inputPin[i]));
  // // if(digitalRead(inputPin[i])>550 && digitalRead(inputPin[i])<580){
  //     if(digitalRead(inputPin[i])){
  //   state[i]=1;
  // }
  // else{
  //   state[i]=0;
  // }
  }
}
void showDigits(int i){
  
    // Serial.println(i);
      for(int j=0; j<7; j++)
    {
      digitalWrite(outputPin[j],signalVal[i][j]);
    }
  
  
}
void hideDigits(){
    for(int i=0; i<7; i++)
  {
    digitalWrite(outputPin[i],1);
  }
}

int binToDec(){
  return ((state[0]*1)+(state[1]*2)+(state[2]*4)+(state[3]*8));
}
void loop() {
  digitalWrite(8,HIGH);
  // showDigits();
  // delay(1000);
  // hideDigits();
  // delay(1000);
  readInput();
  binToDec();
  Serial.println(binToDec());
  switch (binToDec()) {
  case 0:
    showDigits(0);
    break;
  case 1:
    showDigits(1);
    break;
  case 2:
    showDigits(2);
    break;
  case 3:
    showDigits(3);
    break;
  case 4:
    showDigits(4);
    break;
  case 5:
    showDigits(5);
    break;
  case 6:
    showDigits(6);
    break;
  case 7:
    showDigits(7);
    break;
  case 8:
    showDigits(8);
    break;
  case 9:
    showDigits(9);
    break;
  // case 10:
  //   showDigits(0);
  //   break;
  // case 11:
  //   showDigits(0);
  //   break;
  // case 12:
  //   showDigits(0);
  //   break;
  // case 13:
  //   showDigits(0);
  //   break;
  // case 14:
  //   showDigits(0);
  //   break;
  // case 15:
  //   showDigits(0);
  //   break;
  default:
    Serial.println(binToDec());
}

}
