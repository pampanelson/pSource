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
    //soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, 3);
    
    //setup ofxAudioAnalyzer with the SAME PARAMETERS
    audioAnalyzer.setup(sampleRate, bufferSize, inChannels);
    
    player.load("sources_20190202_44k_16bit.wav");

    gui.setup();
    
    
    // start status
    brightness = 0.001;
    ray_brightness = 7.0;
    gamma = 7.0;
    curvature = 10.0;
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    //soundStream.close();
    audioAnalyzer.exit();
    
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
    //audioAnalyzer.analyze(input);
    
}


//--------------------------------------------------------------
void ofApp::update(){

    ofSoundUpdate();
    float fftSum = 0;

    float * val = ofSoundGetSpectrum(nBandsToGet);        // request 128 values for fft
    for (int i = 0;i < nBandsToGet; i++){
        
        // let the smoothed value sink to zero:
        fftSmoothed[i] *= 0.96f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
        
        //cout << i << ": " << fftSmoothed[i] << endl;
        
        fftSum += fftSmoothed[i];
        
    }
    
    rms = fftSum / nBandsToGet;
    rmsSum += rms;
    
    if(part == 0){
        //ifColor = 0;
        if(brightness <= 1.0){
            brightness += rms * 0.1;
        }else if(brightness < 5.0){
            brightness += rms * 0.2;
        }else{
            brightness = 5.0;
        }
    }
    
    if(part == 1){
        //ifColor = 0;
        brightness = 5.0;
        if(curvature < 180){
            curvature += rms;
        }else{
            curvature = 180;
        }
    }
    
    if(part == 2){
        //ifColor = 1;
        
//        colorChangerR = rms * 50;

    }
    
    if(part == 5){
        brightness = 5.0;
        if(curvature >= 10){
            curvature -= rms * 1.2;
        }else{
            curvature = 10;
        }
    }


    
//    float brightness_small_step = 0.001;
//    float brightness_middle_step = 0.01;
//    float brightness_big_step = 0.1;
//
//    float curvature_step = 0.05;
//
//    if (brightness <= 1.0){
//        brightness += brightness_small_step;
//    }else if(brightness > 1.0 && brightness < 2.0){
//        brightness += brightness_middle_step;
//    }else{
//        brightness += brightness_big_step;
//    }
//
//
//    if(curvature >= 180){
//        curvature = 180.0;
//    };
//
//    if(brightness > 5.0){
//        brightness = 5.0;
//        curvature += curvature_step;
//    };
//
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    syphonFbo.begin();
    ofClear(0,0,0);
    shadertoy.begin();
    shadertoy.setUniform1f("brightness", brightness);
    shadertoy.setUniform1f("ray_brightness",ray_brightness);
    shadertoy.setUniform1f("gamma", gamma);
    shadertoy.setUniform1f("curvature", curvature);
//    shadertoy.setUniform1f("colorChangerR", colorChangerR);
//    shadertoy.setUniform1f("colorChangerG", colorChangerR);
//    shadertoy.setUniform1f("colorChangerB", colorChangerR);
//    shadertoy.setUniform1i("ifColor", ifColor);
    shadertoy.setUniform1i("rms", rms * 5.0);

    shadertoy.end();
    shadertoy.draw(0, 0, width, height);
    syphonFbo.end();
    syphonServer.publishTexture(&syphonFbo.getTexture());
    
    ofSetColor(255, 0, 0);
    
    float barW = 3;

    for (int i = startBar; i < fftSmoothed.size(); i++) {
        
        float x = 10 + i * barW;
        float y = 300;
        float barH = fftSmoothed[i] * 100;
        
        ofDrawRectangle(x, y, barW, barH);
    }
    
    ofDrawBitmapString("rms  : " + ofToString(rms), 10, 10);
    ofDrawBitmapString("rms sum : " + ofToString(rmsSum), 10, 20);
    ofDrawBitmapString("part : " + ofToString(part), 10, 30);
    ofDrawBitmapString("brightness : " + ofToString(brightness), 10, 40);
    ofDrawBitmapString("curvature : " + ofToString(curvature), 10, 50);
    ofDrawBitmapString("start bar : " + ofToString(startBar), 10, 60);

    int numToShowFft = 20;
    for (int i = 0; i < numToShowFft; i++) {
        ofDrawBitmapString("fft " + ofToString(i) + ": " + ofToString(fftSmoothed.at(i)), 10, 70 + 10 * i);
    }



    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'p'){
        
        player.play();
        
    }
    if(key == 'a'){
        part = 0;
    }
    
    if(key == 'b'){
        part = 1;
    }
    
    if(key == 'c'){
        part = 2;
    }
    
    if(key == 'd'){
        part = 3;
    }
    
    if(key == 'e'){
        part = 4;
    }
    
    if(key == 'f'){
        part = 5;
    }
    if(key == 'g'){
        part = 6;
    }
    if(key == 'h'){
        part = 7;
    }
    
    if(key == OF_KEY_UP){
        startBar ++;
        
    }
    if(key == OF_KEY_DOWN){
        startBar --;
    }
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
