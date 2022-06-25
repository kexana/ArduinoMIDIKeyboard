#include <PCF8574.h>
#include <Wire.h>

const int  rowPins[]= {2,3,4,5};
//2-purple; 3-blue; 4-green; 5-yellow;

const int noteON = 144;//144 = 10010000 in binary, note on command
const int noteOFF = 128;//128 = 10000000 in binary, note off command
const int velocity = 100;

int midiNotes[4][8]; 

void NoteSetup(){ 
  midiNotes[1][0]=47;
  midiNotes[1][1]=46;
  midiNotes[1][2]=45;
  midiNotes[1][3]=44;
  midiNotes[1][4]=43;
  midiNotes[1][5]=42;
  midiNotes[1][6]=41;
  midiNotes[1][7]=72;
  
  midiNotes[2][0]=55;
  midiNotes[2][1]=54;
  midiNotes[2][2]=53;
  midiNotes[2][3]=52;
  midiNotes[2][4]=51;
  midiNotes[2][5]=50;
  midiNotes[2][6]=49;
  midiNotes[2][7]=48;
  
  midiNotes[3][0]=63;
  midiNotes[3][1]=62;
  midiNotes[3][2]=61;
  midiNotes[3][3]=60;
  midiNotes[3][4]=59;
  midiNotes[3][5]=58;
  midiNotes[3][6]=57;
  midiNotes[3][7]=56;
  
  midiNotes[0][0]=71;
  midiNotes[0][1]=70;
  midiNotes[0][2]=69;
  midiNotes[0][3]=68;
  midiNotes[0][4]=67;
  midiNotes[0][5]=66;
  midiNotes[0][6]=65;
  midiNotes[0][7]=64;
}

PCF8574 pcf(0x38);

int lastBtnState[4][8];

void setup()
{
  Serial.begin(115200);

  pcf.begin();
  pcf.setButtonMask(00000000);

  NoteSetup();

  for(int i=0;i<4;i++){
    pinMode(rowPins[i], INPUT);
    digitalWrite(rowPins[i], HIGH);
    for(int j=0;j<8;j++){ 
      lastBtnState[i][j] = 1;
    }
  }
  delay(1000);
}

void loop()
{
  for(int i=0;i<4;i++){
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i],LOW);
    
    int btnStates[8];
    CrackByte(pcf.readButton8(),btnStates);
    
    for(int j=0;j<8;j++){
      //Serial.print(btnStates[j]);
      if (btnStates[j] != lastBtnState[i][j]) {
        if(btnStates[j] == 0){
          MIDImessage(noteON,midiNotes[i][j],velocity);
        }else{
          MIDImessage(noteON,midiNotes[i][j],0);
          }
        }
        lastBtnState[i][j]=btnStates[j];    
     }
     //Serial.println();
     delay(5);
     pinMode(rowPins[i], INPUT);
  }
}
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}
void CrackByte( byte b, int variable[8] ){
  byte i;
  
  for ( i=0; i < 8; ++i )
  {
    variable[i] = b & 1;
    b = b >> 1;
  }
}
