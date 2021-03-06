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

// these two vars control what mode the synth is in. I should add a couple of buttons to the synth to switch between these. 
int synthMode = 0;
int noteRange = 0;

//these vars are used to hold the last note played and the note that is currently being played. 
int currentNote = 0;
int lastNote = 0;

int sensorValue = 0;

//for case 1
bool returnedToZero = false;


// MIDI Scales
int cMaj[] = {48,50,52,53,55,57,59};
int chromatic[] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120};

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
  sensorValue = analogRead(littleBitsAnalogVoltagePin);
  


  //this switch is to make it so that we can switch between different MIDI modes with ease. 
  switch(synthMode%2){


    case(0):
      singleNote();
      break;
      
    // case 1 consists of reading the incoming voltage, mapping it to a range of MIDI notes,
    // sending out that note and stopping the last note sent. It waits for the voltage to fall back
    // to zero between each note. This mode is for use with the sequencer
    case(1):
      singleNoteWaitForReturnToZero();
      break;
  } 
}

void singleNote(){
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
    setCurrentNote();
  }
  
  if(!returnedToZero){
    if(sensorValue < 10){
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

int setCurrentNote(){
  int sizeOfNoteArray;
  switch(noteRange){
    case 0: //chromatic
      sizeOfNoteArray = sizeof(chromatic)/sizeof(int);
      currentNote = returnMappedNote(sensorValue, chromatic, sizeOfNoteArray);
      break;
      
    case 1: //cMaj
      sizeOfNoteArray = sizeof(cMaj)/sizeof(int);
      currentNote = returnMappedNote(sensorValue, cMaj, sizeOfNoteArray);
      break;
  }
}

int returnMappedNote(int currentSensorValue, int currentNoteArray[], int sizeOfCurrentNoteArray){
  int noteIndex = constrain(map(currentSensorValue, 0, 1000, 0, sizeOfCurrentNoteArray), 0, sizeOfCurrentNoteArray);
  int mappedNote = currentNoteArray[noteIndex];
  return mappedNote;
}


