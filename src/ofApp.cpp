#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    ofSetBackgroundColor(0, 0, 0);

    shadertoy.load("shaders/sources.frag"); // change file name ========== TODO
    
    width = 1920;
    height = 1080;
    shadertoy.setDimensions(width, height); // **********change dimension after load shader file ************************ 
    
    
    //    width = 1280 * 2;
    //    height = 720;
    
    //    width = 1920;
    //    height = 1080;
    
    syphonFbo.allocate(width,height);
    
    syphonServer.setName("pSources");
    
    
    
    
    // sound settings
    soundStream.printDeviceList();

    ofSoundStreamSettings settings;
    
    // if you want to set the device id to be different than the default
    // auto devices = soundStream.getDeviceList();
    // settings.device = devices[4];
    
    // you can also get devices for an specific api
    // auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
    // settings.device = devices[0];
    
    // or get the default device for an specific api:
    // settings.api = ofSoundDevice::Api::PULSE;
    
    // or by name
//    auto devices = soundStream.getMatchingDevices("default");
//    if(!devices.empty()){
//        settings.setInDevice(devices[0]);
//    }
//
//    settings.setInListener(this);
//    settings.sampleRate = 44100;
//    settings.numOutputChannels = 0;
//    settings.numInputChannels = 2;
//    settings.bufferSize = bufferSize;
//    soundStream.setup(settings);
//
    
    int sampleRate = 44100;
    int bufferSize = 512;
    int outChannels = 0;
    int inChannels = 2;
    
    // setup the sound stream
    soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, 3);
    
    //setup ofxAudioAnalyzer with the SAME PARAMETERS
    audioAnalyzer.setup(sampleRate, bufferSize, inChannels);
    
    
    gui.setup();
    
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    soundStream.close();
    audioAnalyzer.exit();

}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
    audioAnalyzer.analyze(input);

}


//--------------------------------------------------------------
void ofApp::update(){
    
    float smoothing = 0.5;
    
    //-:ANALYZE SOUNDBUFFER:
    audioAnalyzer.analyze(soundBuffer);
    
    //-:get Values:
    rms     = audioAnalyzer.getValue(RMS, 0, smoothing);
    spectrum = audioAnalyzer.getValues(SPECTRUM, 0, smoothing);
    
    cout << "RMS : " << rms << endl;
    
    cout << spectrum.at(80) << endl;

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float r = (sin(ofGetElapsedTimef()) + 2) * 0.5;
    syphonFbo.begin();
    ofClear(0,0,0);
    
    shadertoy.begin();
    
    shadertoy.end();
    shadertoy.draw(0, 0, width, height);
    
    syphonFbo.end();
    
    syphonServer.publishTexture(&syphonFbo.getTexture());
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
