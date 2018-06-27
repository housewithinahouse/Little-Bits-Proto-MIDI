// LittleBitsMIDISynth
/*
 * This is the code for Monroe County Public Library's Little Bits MIDI Synth. 
 * It was written by Edwin Fallwell in 2018. It is released with an MIT License. 
 * 
 * To use this, you'll need the Arduino MIDI Library (https://playground.arduino.cc/Main/MIDILibrary)
 * and the Hairless MIDI<->Serial Bridge (http://projectgus.github.io/hairless-midiserial/). I changed the 
 * Hairless Bridge to listen at 9600 Baud, and told the Arduino to broadcast at that, but if you had a reason, 
 * you could do it differently. You also need something to listen to the MIDI bus and use those instructions 
 * to make sound. I used Logic Pro X because we had it and I know how to use it. 
 * 
 * On the hardware side, I simply connected the center pin of the out side of a Proto LittleBit (https://shop.littlebits.com/products/proto)
 * to A0, and the GND (leftmost screw terminal) to GND. I left all the jumpers in place on the Little Bit. 
 * 
 * From here, there isn't much to it. Hook up littleBits the the input side of the littleBit and listen to the crazy. 
 * Control setups I've tried and liked included a slide pot -> 4 step sequencer -> synth, and light sensor -> button -> synth. 
 * The bargraph is fun to follow it up with. 
 * 
 * other tools that I used include: midi <-> ascii conversion (http://www.archduke.org/midi/) and 
 * feelyoursound's MIDI scales: https://www.feelyoursound.com/scale-chords/
 */

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

int littleBitsAnalogVoltagePin = A0;
int synthMode = 0;
int currentNote = 0;
int lastNote = 0;
int noteRange = 0;

//for case 0
bool returnedToZero = false;


// for case 1
int swingNote = 0;
int swingFlag = 0;
int count = 0;

// MIDI Scales
int cMaj[] = {48,50,52,53,55,57,59};
int chromatic[] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80};

void setup(){
  MIDI.begin(4);                                                                      // Launch MIDI and listen to channel 4
  Serial.begin(9600);
}

void loop(){
  // case 0 consists of reading the incoming voltage, mapping it to a range of MIDI notes,
  // sending out that note and stopping the last note sent. It waits for the voltage to fall back
  // to zero between each note. This should probally be implemented as one of several modes, because 
  // this only really works well with the synth. 

  //check the sensor
  int sensorValue = analogRead(littleBitsAnalogVoltagePin);


  //this switch is to make it so that we can switch between different MIDI modes with ease. 
  switch(synthMode%2){

  // case 0 consists of reading the incoming voltage, mapping it to a range of MIDI notes,
  // sending out that note and stopping the last note sent. It waits for the voltage to fall back
  // to zero between each note. This mode is for use with the sequencer
    case(0):
      singleNote();
      break;

    case(1):
      singleNoteWaitForReturnToZero();
      break;
  } 
}

void singleNote(){
  // Reads the 
  if(sensorValue != 0){
    int newCurrentNote = map(sensorValue, 1, 1000, 20, 120);                   //temp storage of new note
    if(newCurrentNote != currentNote+1 && newCurrentNote != currentNote-1){   //check to see if new note is too close to old note
      currentNote = newCurrentNote;                                            //if it's not, set it to new current note
    }
  }    
  
  if(currentNote != lastNote){
    MIDI.sendNoteOn(currentNote, 127, 1);    // Send a note
    MIDI.sendNoteOff(lastNote, 0, 1);        // Stop the last note  
    lastNote = currentNote;
  }
}

void singleNoteWaitForReturnToZero(){
  if(sensorValue != 0 && returnedToZero){
    int currentNote = returnMappedNote(sensorValue, chromatic);                   
  }
  
  if(!returnedToZero){
    if(sensorValue = 0){
      returnedToZero = true;
    }
  }
  
  if(currentNote != lastNote){
    MIDI.sendNoteOn(currentNote, 127, 1);    // Send a note
    MIDI.sendNoteOff(lastNote, 0, 1);        // Stop the last note  
    lastNote = currentNote;
    returnedToZero = false;
  }
}

int returnMappedNote(int currentSensorValue, int currentNoteArray[]){
  int upperBounds = sizeof(currentNoteArray)/sizeof(int);
  int noteIndex = constrain(map(currentSensorValue, 0, 1000, 0, upperBounds), 0, upperBounds);
  int mappedNote = currentNoteArray[noteIndex];
  return mappedNote;
}


