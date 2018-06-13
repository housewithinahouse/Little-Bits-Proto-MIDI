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

int currentNote = 0;
int lastNote = 0;
int swingNote = 0;
int swingFlag = 0;
int count = 0;

void setup(){
  MIDI.begin(4);                      // Launch MIDI and listen to channel 4
  Serial.begin(9600);
}

void loop(){
  // Main loop consists of reading the incoming voltage, mapping it to a range of MIDI notes,
  // sending out that note and stopping the last note sent.
  int sensorValue = analogRead(A0);
      
    if(sensorValue != 0){
      int newCurrentNote = map(sensorValue, 1, 1000, 20, 70);                   //temp storage of new note
      if(newCurrentNote != currentNote+1 || newCurrentNote != currentNote-1){   //check to see if new note is too close to old note
        currentNote = newCurrentNote                                            //if it's not, set it to new current note
      }
    }
    
    if(currentNote != lastNote){
        MIDI.sendNoteOn(currentNote, 127, 1);    // Send a note
        MIDI.sendNoteOff(lastNote, 0, 1);        // Stop the last note  
        lastNote = currentNote;
        count = 0;
    }
}
