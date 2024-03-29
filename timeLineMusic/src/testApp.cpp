#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetDataPathRoot("../Resources/data/");
    
	ofSetFrameRate(60);
    ofBackground(255);
    
	midiFileName = "data/entertainer.mid";
	int retVal = cannamMainFunction();
	
	playing = false;
	
	screenWidth = ofGetWidth();
	screenHeight = ofGetHeight();
	midiEvents.screenWidth = &screenWidth;
	midiEvents.screenHeight = &screenHeight;
    
    gui.setup(parameters);
    gui.setPosition(10, 10);
    gui.loadFromFile("settings.xml");
    gui.add(frameView.setup("fr", ""));
    gui.add(bMidiNoteDraw.set("bMidiNoteDraw", false));
    gui.minimizeAll();
    
    triggerMovingFactor = 0;
    
    
    // Tonic
    ofSoundStreamSetup(2, 0, this, 44100, 256, 4);

    ControlGenerator midiNote = synth.addParameter("midiNumber");
    ControlGenerator noteFreq =  ControlMidiToFreq().input(midiNote);
    Generator tone = RectWave().freq( noteFreq );
    tone = LPF12().input(tone).Q(10).cutoff((noteFreq * 2) + SineWave().freq(10) * 0.5 * noteFreq);

    ControlGenerator envelopeTrigger = synth.addParameter("trigger");
    Generator toneWithEnvelope = tone * ADSR().attack(0).decay(1.5).sustain(0).release(0).trigger(envelopeTrigger).legato(true);
    Generator toneWithDelay = StereoDelay(0.5, 0.75).input(toneWithEnvelope).wetLevel(0.1).feedback(0.2);

    synth.setOutputGen( toneWithEnvelope );
    
    fullscreen = false;
}

//--------------------------------------------------------------
void testApp::update(){
	if (playing){
		midiEvents.updatePlayPosition();
    }
    
    triggerMovingFactor = triggerMovingFactor + 0.012;
    if (triggerMovingFactor>1) triggerMovingFactor = 0;
    
    frameView = ofToString(ofGetFrameRate());
}

//--------------------------------------------------------------
void testApp::draw(){

    gui.draw();
    
    if (bMidiNoteDraw) {
        midiEvents.drawFile();
    }
    
    drawPreviewLine();
    triggerLineDraw();
    
    for (int i=0; i<noteBlock.size(); i++) {
        noteBlock[i].drawing();
    }
}


void testApp::randomMidiInput(int _p, int _v, int _d){
    
    NoteBlock noteBlock_e;
    noteBlock_e.noteInput(_p, _v, _d);
    noteBlock.push_back(noteBlock_e);
    
}



void testApp::drawPreviewLine(){

    ofPushMatrix();
    ofPushStyle();
    ofSetColor( ofColor::fromHsb(0, 255, 255, 150) );
    ofSetLineWidth(3);
    
    ofLine( mousePressedPos, mouseReleassedPos );
    
    ofPopStyle();
    ofPopMatrix();
}


void testApp::triggerLineDraw(){

    ofPushMatrix();
    ofPushStyle();
    ofSetColor( ofColor::fromHsb(0, 255, 255, 150) );
    ofSetLineWidth(3);

    for (int i=0; i<triggerLine.size(); i++) {
        ofLine( triggerLine[i].start, triggerLine[i].stop );
        
        triggerPosOnLine = ( triggerLine[i].stop - triggerLine[i].start ) * triggerMovingFactor + triggerLine[i].start;
        
        for (int i=0; i<noteBlock.size(); i++) {
            noteBlock[i].contact(triggerPosOnLine);
            if (noteBlock[i].contactBlock) {
                trigger( noteBlock.at(i).midiPitch );
            }
        }
        
        ofPushStyle();
        ofEllipse( triggerPosOnLine.x, triggerPosOnLine.y, 10, 10 );
        
        ofPopStyle();
    }
    
    ofPopStyle();
    ofPopMatrix();
}


void testApp::trigger(int _pitch){
    synth.setParameter("midiNumber", _pitch);
    synth.setParameter("trigger", 1);
}

//--------------------------------------------------------------
void testApp::setScaleDegreeBasedOnMouseX(){
//    int newScaleDegree = ofGetMouseX() * NUMBER_OF_KEYS / ofGetWindowWidth();
//    if(ofGetMousePressed() && ( newScaleDegree != scaleDegree )){
//        scaleDegree = newScaleDegree;
//        trigger();
//    }else{
//        scaleDegree = newScaleDegree;
//    }
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
	
	if (key == ' '){
		startPlaying();
	}
	
	
	if (key == OF_KEY_RETURN)
		stopPlaying();
	
	if (key == OF_KEY_UP){
		if (midiEvents.ticksPerScreen >= 4000)
            midiEvents.ticksPerScreen += 2000;
		else
			midiEvents.ticksPerScreen += 500;
	}
	
	if (key == 'm'){
        //		midiEvents.findMatch(84, 0, 10000);
	}
	
	if (key == OF_KEY_DOWN){
		if (midiEvents.ticksPerScreen >= 4000)
            midiEvents.ticksPerScreen -= 2000;
        else if (midiEvents.ticksPerScreen > 500)
            midiEvents.ticksPerScreen -= 500;
	}
	
	if (key == 'w')
		midiEvents.printMatchMatrix();
	
	if (key == 'p'){
		midiEvents.printNotes();
	}
    
	if (key == 'o'){
		//open audio file
		string *filePtr;
		filePtr = &midiFileName;
		
		if (getFilenameFromDialogBox(filePtr)){
			printf("Midifile: Loaded name okay :\n'%s' \n", midiFileName.c_str());
			cannamMainFunction();
		}
        
	}
    
    
    if (key=='x') {
        triggerLine.clear();
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    if (key=='r') {
        reload = true;
        noteBlock.clear();
        int retVal = cannamMainFunction();
    }
    
    if (key=='f') {
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    mouseReleassedPos.x = x;
    mouseReleassedPos.y = y;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    mouseReleassedPos.x = x;
    mouseReleassedPos.y = y;
    mousePressedPos.x = x;
    mousePressedPos.y = y;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    TriggerLine triggerLine_e;
    triggerLine_e.start.x = mousePressedPos.x;
    triggerLine_e.start.y = mousePressedPos.y;
    triggerLine_e.stop.x = mouseReleassedPos.x;
    triggerLine_e.stop.y = mouseReleassedPos.y;
    triggerLine.push_back(triggerLine_e);
    
    if (triggerLine.size()>9) {
        triggerLine.erase(triggerLine.begin());
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	screenWidth = w;
	screenHeight = h;
	midiEvents.noteHeight = screenHeight / (float)(midiEvents.noteMaximum - midiEvents.noteMinimum);
	
}



void testApp::startPlaying(){
	playing = !playing;
	midiEvents.reset();
	midiEvents.setStartPlayingTimes();
	//this is where we stop and start playing
}

void testApp::stopPlaying(){
	playing = false;
}

bool testApp::getFilenameFromDialogBox(string* fileNameToSave){
	//this uses a pointer structure within the loader and returns true if the dialogue box was used successfully
	// first, create a string that will hold the URL
	string URL;
	
	// openFile(string& URL) returns 1 if a file was picked
	// returns 0 when something went wrong or the user pressed 'cancel'
    //	int response = ofxFileDialogOSX::openFile(URL);
    //	if(response){
    //		// now you can use the URL
    //		*fileNameToSave = URL;
    //		//printf("\n filename is %s \n", soundFileName.c_str());
    //		return true;
    //	}
    //	else {
    //		//	soundFileName = "OPEN canceled. ";
    //		printf("\n open file cancelled \n");
    //		return false;
    //	}
	
	
	
}




int testApp::cannamMainFunction(){
    
    
	midiEvents.recordedNoteOnMatrix.clear();//where we store the note ons
	
	//int main(int argc, char **argv)
	//{
	//	if (argc != 2) {
	//		cerr << "Usage: midifile <file.mid>" << endl;
	//		return 1;
	//	}
    
	std::string filename = midiFileName;//argv[1];
    
    MIDIFileReader fr(filename);
    if (!fr.isOK()) {
        std::cerr << "Error: " << fr.getError().c_str() << std::endl;
        return 1;
    }
    
    MIDIComposition c = fr.load();
    
    switch (fr.getFormat()) {
        case MIDI_SINGLE_TRACK_FILE: cout << "Format: MIDI Single Track File" << endl; break;
        case MIDI_SIMULTANEOUS_TRACK_FILE: cout << "Format: MIDI Simultaneous Track File" << endl; break;
        case MIDI_SEQUENTIAL_TRACK_FILE: cout << "Format: MIDI Sequential Track File" << endl; break;
        default: cout << "Format: Unknown MIDI file format?" << endl; break;
    }
    
    cout << "Tracks: " << c.size() << endl;
    
    int td = fr.getTimingDivision();
    if (td < 32768) {
        cout << "Timing division: " << fr.getTimingDivision() << " ppq" << endl;
        
        midiEvents.pulsesPerQuarternote = fr.getTimingDivision();
    } else {
        int frames = 256 - (td >> 8);
        int subframes = td & 0xff;
        cout << "SMPTE timing: " << frames << " fps, " << subframes << " subframes" << endl;
    }
    
    for (MIDIComposition::const_iterator i = c.begin(); i != c.end(); ++i) {
        
        cout << "Start of track: " << i->first+1 << endl;
        
        for (MIDITrack::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
            
            unsigned int t = j->getTime();
            int ch = j->getChannelNumber();
            
            if (j->isMeta()) {
                int code = j->getMetaEventCode();
                string name;
                bool printable = true;
                switch (code) {
                        
                    case MIDI_END_OF_TRACK:
                        cout << t << ": End of track" << endl;
                        break;
                        
                    case MIDI_TEXT_EVENT: name = "Text"; break;
                    case MIDI_COPYRIGHT_NOTICE: name = "Copyright"; break;
                    case MIDI_TRACK_NAME: name = "Track name"; break;
                    case MIDI_INSTRUMENT_NAME: name = "Instrument name"; break;
                    case MIDI_LYRIC: name = "Lyric"; break;
                    case MIDI_TEXT_MARKER: name = "Text marker"; break;
                    case MIDI_SEQUENCE_NUMBER: name = "Sequence number"; printable = false; break;
                    case MIDI_CHANNEL_PREFIX_OR_PORT: name = "Channel prefix or port"; printable = false; break;
                    case MIDI_CUE_POINT: name = "Cue point"; break;
                    case MIDI_CHANNEL_PREFIX: name = "Channel prefix"; printable = false; break;
                    case MIDI_SEQUENCER_SPECIFIC: name = "Sequencer specific"; printable = false; break;
                    case MIDI_SMPTE_OFFSET: name = "SMPTE offset"; printable = false; break;
                        
                    case MIDI_SET_TEMPO:
                    {
                        int m0 = j->getMetaMessage()[0];
                        int m1 = j->getMetaMessage()[1];
                        int m2 = j->getMetaMessage()[2];
                        long tempo = (((m0 << 8) + m1) << 8) + m2;
                        
                        cout << t << ": Tempo: " << 60000000.0 / double(tempo) << endl;
                        midiEvents.tempo = 60000000.0 / double(tempo);
                        midiEvents.period = double(tempo)/1000.0;
                        
                        printf("period double is %f\n", midiEvents.period);
                    }
                        break;
                        
                    case MIDI_TIME_SIGNATURE:
                    {
                        int numerator = j->getMetaMessage()[0];
                        int denominator = 1 << (int)j->getMetaMessage()[1];
                        
                        cout << t << ": Time signature: " << numerator << "/" << denominator << endl;
                    }
                        
                    case MIDI_KEY_SIGNATURE:
                    {
                        int accidentals = j->getMetaMessage()[0];
                        int isMinor = j->getMetaMessage()[1];
                        bool isSharp = accidentals < 0 ? false : true;
                        accidentals = accidentals < 0 ? -accidentals : accidentals;
                        cout << t << ": Key signature: " << accidentals << " "
                        << (isSharp ?
                            (accidentals > 1 ? "sharps" : "sharp") :
                            (accidentals > 1 ? "flats" : "flat"))
                        << (isMinor ? ", minor" : ", major") << endl;
                    }
                        
                }
                
                
                if (name != "") {
                    if (printable) {
                        cout << t << ": File meta event: code " << code
                        << ": " << name << ": \"" << j->getMetaMessage()
                        << "\"" << endl;
                    } else {
                        cout << t << ": File meta event: code " << code
                        << ": " << name << ": ";
                        for (int k = 0; k < j->getMetaMessage().length(); ++k) {
                            cout << (int)j->getMetaMessage()[k] << " ";
                        }
                    }
                }
                continue;
            }
            
            switch (j->getMessageType()) {
                    
                case MIDI_NOTE_ON:
                    cout << t << ": Note: channel " << ch
                    << " duration " << j->getDuration()
                    << " pitch " << j->getPitch()
                    << " velocity " << j->getVelocity()
                    << "event time " << midiEvents.getEventTimeMillis(t) << endl;
                    v.clear();
                    v.push_back(t);
                    v.push_back(j->getPitch());
                    v.push_back(j->getVelocity());
                    v.push_back(j->getDuration());
                    midiEvents.recordedNoteOnMatrix.push_back(v);
                    midiEvents.recordedEventTimes.push_back(midiEvents.getEventTimeMillis(t));
                    
                    randomMidiInput( j->getPitch(), j->getVelocity(), j->getDuration() );
                    
                    break;
                    
                case MIDI_POLY_AFTERTOUCH:
                    cout << t << ": Polyphonic aftertouch: channel " << ch
                    << " pitch " << j->getPitch()
                    << " pressure " << j->getData2() << endl;
                    break;
                    
                case MIDI_CTRL_CHANGE:
                {
                    int controller = j->getData1();
                    string name;
                    switch (controller) {
                        case MIDI_CONTROLLER_BANK_MSB: name = "Bank select MSB"; break;
                        case MIDI_CONTROLLER_VOLUME: name = "Volume"; break;
                        case MIDI_CONTROLLER_BANK_LSB: name = "Bank select LSB"; break;
                        case MIDI_CONTROLLER_MODULATION: name = "Modulation wheel"; break;
                        case MIDI_CONTROLLER_PAN: name = "Pan"; break;
                        case MIDI_CONTROLLER_SUSTAIN: name = "Sustain"; break;
                        case MIDI_CONTROLLER_RESONANCE: name = "Resonance"; break;
                        case MIDI_CONTROLLER_RELEASE: name = "Release"; break;
                        case MIDI_CONTROLLER_ATTACK: name = "Attack"; break;
                        case MIDI_CONTROLLER_FILTER: name = "Filter"; break;
                        case MIDI_CONTROLLER_REVERB: name = "Reverb"; break;
                        case MIDI_CONTROLLER_CHORUS: name = "Chorus"; break;
                        case MIDI_CONTROLLER_NRPN_1: name = "NRPN 1"; break;
                        case MIDI_CONTROLLER_NRPN_2: name = "NRPN 2"; break;
                        case MIDI_CONTROLLER_RPN_1: name = "RPN 1"; break;
                        case MIDI_CONTROLLER_RPN_2: name = "RPN 2"; break;
                        case MIDI_CONTROLLER_SOUNDS_OFF: name = "All sounds off"; break;
                        case MIDI_CONTROLLER_RESET: name = "Reset"; break;
                        case MIDI_CONTROLLER_LOCAL: name = "Local"; break;
                        case MIDI_CONTROLLER_ALL_NOTES_OFF: name = "All notes off"; break;
                    }
                    cout << t << ": Controller change: channel " << ch
                    << " controller " << j->getData1();
                    if (name != "") cout << " (" << name << ")";
                    cout << " value " << j->getData2() << endl;
                }
                    break;
                    
                case MIDI_PROG_CHANGE:
                    cout << t << ": Program change: channel " << ch
                    << " program " << j->getData1() << endl;
                    break;
                    
                case MIDI_CHNL_AFTERTOUCH:
                    cout << t << ": Channel aftertouch: channel " << ch
                    << " pressure " << j->getData1() << endl;
                    break;
                    
                case MIDI_PITCH_BEND:
                    cout << t << ": Pitch bend: channel " << ch
                    << " value " << (int)j->getData2() * 128 + (int)j->getData1() << endl;
                    break;
                    
                case MIDI_SYSTEM_EXCLUSIVE:
                    cout << t << ": System exclusive: code "
                    << (int)j->getMessageType() << " message length " <<
                    j->getMetaMessage().length() << endl;
                    break;
            }
        }
    }
    
	//}
}//end cannam midi main


void testApp::audioRequested (float * output, int bufferSize, int nChannels){
    synth.fillBufferOfFloats(output, bufferSize, nChannels);
}
