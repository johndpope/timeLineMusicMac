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
#include "ofxFft.h"

#include "ofxGui.h"

//#include "ofxOsc.h"
//#define PORT 12121

using namespace std;
using namespace MIDIConstants;

class testApp : public ofBaseApp{

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
    ofxLabel theSongName;
    ofxLabel audioBar;
    ofxLabel midiBar;
    ofParameter<int> sceneChange;
    ofParameter<float> bgImageBright_G;
    ofParameter<float> speedSmoothVolume;
    ofParameter<bool> bDrawPointCloud;
    ofParameter<bool> AllSceneOnOff;
    ofParameter<bool> numberOnOff;
    ofParameter<float> inputVolume;
    
};

#endif
