/*
  Melody
 Plays a melody 
 
 circuit:
 * 8-ohm speaker on digital pin 1
 
 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe 

 http://arduino.cc/en/Tutorial/Tone

Modified by KrlsNdrs  Jan 1, 2016 for a melody Created in Cali, Colombia (Ras Tas Tas by Cali Flow Latino)
P0 - P1: Piezo disk
P2-P3-P4-P5: Charlieplexing leds
Works with Reset fuse disabled (but isn't necessary)

 */
#include "pitches.h"
#include "Charlieplex.h"

#define ras  NOTE_CS5
#define tas  NOTE_D5
#define taz  NOTE_E5

#define como_asi  NOTE_CS7
#define como_asi_que  NOTE_D7
#define como_fue  NOTE_E7

#define aja  NOTE_CS4
#define jodedera  NOTE_D4
#define full_hd  NOTE_E4

#define tempo_real 50


float n = 4 ; // negra
float c= 8 ; // corchea
float cp= 5.33; //16/3 ; // corchea con puntillo
float cpmsf= 5.81; //64/11 ; // corchea con puntillo menos semifusa
float s= 16 ; // semicorchea
float f =32 ; // fusa
float sf= 64 ; // semifusa

float nmsf= 4.26; //64/15 ; // restar una semifusa a una negra
float cmsf= 9.14; //64/7 ; // restar una semifusa a una corchea
float smsf= 21.33; //64/3 ; // restar una semifusa a una semicorchea
float cmds= 10.66; //32/3 ; // restar dos semifusas a una corchea
float nmds= 4.57; //32/7 ; // restar dos semifusas a una negra


// notes in the melody:
int melody[] = { 
aja,ras,  como_asi,ras,aja,como_asi,ras, ras,ras,  aja,como_asi,tas,
jodedera,tas,  como_asi_que,tas,jodedera,como_asi_que,tas, tas,tas,  jodedera,como_asi_que,taz,
full_hd,taz,  como_fue,taz,full_hd,como_fue,taz, taz,taz,  full_hd,como_fue,tas,
jodedera,tas,  como_asi_que,tas,jodedera,como_asi_que,tas, tas,tas,  jodedera,como_asi_que,ras
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
float noteDurations[] = {
sf,nmsf, sf,cmsf,sf,sf,cmds, c,c, sf,sf,nmds,
sf,nmsf, sf,cmsf,sf,sf,cmds, c,c, sf,sf,nmds,
sf,nmsf, sf,cmsf,sf,sf,cmds, c,c, sf,sf,nmds,
sf,nmsf, sf,cmsf,sf,sf,cmds, c,c, sf,sf,nmds
};
  
int tempo;
int repeticiones = 0;


#define NUMBER_OF_PINS 4

//define pins in the order you want to adress them
byte pins[] = {2,3,4,5};

//initialize object
Charlieplex charlieplex = Charlieplex(pins,NUMBER_OF_PINS);

//individual 'pins' , adress charlieplex pins as you would adress an array
charliePin led1 = { 0 , 1 }; 
charliePin led2 = { 1 , 0 };
charliePin led3 = { 1 , 2 };
charliePin led4 = { 2 , 1 };
charliePin led5 = { 2 , 3 };
charliePin led6 = { 3 , 2 };
boolean singleOn = false;


void setup() 
{
    tempo = (int) (1000 / (tempo_real/60.0) ) ;
    pinMode(0,OUTPUT);
    digitalWrite(0,LOW);
    
        pinMode(2,OUTPUT);
        pinMode(3,OUTPUT);
        pinMode(4,OUTPUT);
        pinMode(5,OUTPUT);
}

void loop()  {
    int total_notas;
    int nota;
    int noteDuration;


      total_notas = sizeof(melody)/sizeof(int);
        
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < total_notas; thisNote++) 
  {

    // Parte A
    if (repeticiones < 8 )
    {
      nota = melody[thisNote];
      //if (repeticiones < 2 && nota > NOTE_C6) // Sin altos
      if (repeticiones < 3 && nota < NOTE_C5) // Sin bajos
        nota = 0;
      if (repeticiones < 2 && (nota < NOTE_C5 || nota > NOTE_C6))
        nota = 0;

      noteDuration = tempo / noteDurations[thisNote];
    }
    else
    // Parte B
    {
    
      nota = melody[thisNote];
      
      if (repeticiones >= 12 && repeticiones <= 13 && nota > NOTE_C5)
        nota = 0;
      noteDuration = tempo / noteDurations[thisNote];
      
    }

    // Ultimos dos compases de la melodia cada 3 repeticiones
    if ( (repeticiones%4 == 3 && thisNote >= (total_notas*2.1/4) && !(repeticiones >= 14) ))
        nota = 0;

    // Parte B final (sin melodia principal)
     if (repeticiones >= 12 /*&& repeticiones <= 14*/ && ( NOTE_C5 < nota && nota < NOTE_C6))
        nota = 0;
        
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    // Cada 4 repeticiones cambia de octava
    tone(1, nota *1/(1+(repeticiones&4)/4), 1000/8);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration ;
    delay(pauseBetweenNotes);
    // stop the tone playing:
      
    noTone(1);
      
  }
  repeticiones = (repeticiones + 1) % 16;
  prender_led(1* (1-(repeticiones%4 + 2)&1));
  prender_led(2* (1-(repeticiones%4 + 2)&2 /2));
  prender_led(3* (1-(repeticiones%4 + 2)&4 /4));
  prender_led(4* (1-(repeticiones%4 + 2)&8 /8));
}

void prender_led(int led)
{
  if (singleOn){ charlieplex.clear(); }
  charlieplex.charlieWrite(led1,HIGH);
  charlieplex.charlieWrite(led2,HIGH);
  charlieplex.charlieWrite(led3,HIGH);
  singleOn=!singleOn;

}
