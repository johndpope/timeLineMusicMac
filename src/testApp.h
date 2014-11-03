#ifndef _TEST_APP
#define _TEST_APP

//midieventholder - newMidiEvents() sent there
//create cannamMidiLoader for cannamMainfunction
//check new prior offset function - how is used?
//check the widening function - adding decay noise
//ticksperscreen could be better as millis per screen


#include "ofMain.h"

#include "MIDIFileReader.h"
//#include "ofxFileDialogOSX.h"
#include "drawMidiNotes.h"

#include <iostream>
#include "midiEventHolder.h"

#include "ofxGui.h"

#include "NoteBlock.h"

#include "ofxTonic.h"
using namespace Tonic;

//#include "ofxOsc.h"
//#define PORT 12121

using namespace std;
using namespace MIDIConstants;

struct MidiNoteRandomDraw {
    int pitch;
    int velocity;
    int duration;
    float onPosX;
    float onPosY;
    float offPosX;
    float offPosY;
    int rotationZ;
};


struct TriggerLine {
    ofVec2f start;
    ofVec2f stop;
    
};

class testApp : public ofBaseApp{

    ofxTonicSynth synth;
    int scaleDegree;
    void trigger(int _pitch);
    void setScaleDegreeBasedOnMouseX();

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

    void audioRequested (float * output, int bufferSize, int nChannels);

    
	void startPlaying();
	void stopPlaying();
	bool getFilenameFromDialogBox(string* fileNameToSave);

	typedef std::vector<int> IntVector;
	typedef std::vector<double> DoubleVector;

	IntVector v;

	midiEventHolder midiEvents;
	
	int cannamMainFunction();
	string midiFileName;

	bool playing;

	int screenWidth, screenHeight;
    
    // ---- GUI
    ofxPanel gui;
    bool bGuiHide;
    ofParameterGroup parameters;
    ofXml settings;
    ofxLabel frameView;
    ofParameter<bool> bMidiNoteDraw;
    
    
    // NoteBlock
    vector<NoteBlock> noteBlock;
    
    // MIDI Note new Drawing
    MidiNoteRandomDraw midiNoteRandomDraw;
    void randomMidiDrawing();
    void randomMidiInput(int _p, int _v, int _d);
    
    // Trigger Line
    vector<TriggerLine> triggerLine;
    ofVec2f mouseReleassedPos;
    ofVec2f mousePressedPos;
    void drawPreviewLine();
    void triggerLineDraw();
    
    
    // Trigger Point;
    float triggerMovingFactor;
    ofVec2f triggerPosOnLine;
    bool triggerHoverX, triggerHoverY;
    
    bool reload;
    
};

#endif
