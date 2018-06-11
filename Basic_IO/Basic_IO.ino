#include <MIDI.h>

// Simple tutorial on how to receive and send MIDI messages.
// Here, when receiving any message on channel 4, the Arduino
// will blink a led and play back a note for 1 second.

MIDI_CREATE_DEFAULT_INSTANCE();

int currentNote = 0;
int lastNote = 0;
int numberOfNotes = 0;


void setup()
{
   
    MIDI.begin(4);                      // Launch MIDI and listen to channel 4
    Serial.begin(9600);
}

void loop()
{
        int sensorValue = analogRead(A0);
        
        if(sensorValue != 0){
          currentNote = map(sensorValue, 1, 1000, 20, 70);
        }
        if(currentNote != lastNote){
          if(numberOfNotes%2 != 0){
            MIDI.sendNoteOn(currentNote, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
            MIDI.sendNoteOff(lastNote, 0, 1);     // Stop the note  
            lastNote = currentNote;
          }
          numberOfNotes++;
            
         }

}
