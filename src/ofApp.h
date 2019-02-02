#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxShadertoy.h"

#include "ofxAudioAnalyzer.h"
#include "ofSoundPlayerExtended.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    
    void audioIn(ofSoundBuffer & input);
    
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    

    
    
    ofShader            shader;
    ofxShadertoy        shadertoy;
    ofxSyphonServer     syphonServer;
    ofFbo               syphonFbo;
    
    
    int                 width;
    int                 height;
    
    
    ofxPanel                         gui;
    ofParameter<float>               rmsThreshold;
    
    float brightness;
    float gamma;
    float ray_brightness;
    float curvature;

    ofSoundStream soundStream;

    ofxAudioAnalyzer                audioAnalyzer;
//    ofSoundPlayerExtended           player;
    ofSoundPlayer           player;

    int                             sampleRate;
    int                             bufferSize;
    ofSoundBuffer                   soundBuffer;
    
    float                           rms;
    float                           rmsSum;
    vector<float>                   spectrum;

    
    float                           colorChangerR;
    float                           colorChangerG;
    float                           colorChangerB;
    int                             ifColor = 1;
    
    int                             startBar = 0;
    int                             part = 0;
    static constexpr size_t nBandsToGet = 256;
    std::array<float, nBandsToGet> fftSmoothed{{0}};
    
    
    
};
