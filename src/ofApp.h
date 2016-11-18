//
//  ofApp.h
//  Kinect_point_cloud
//
//  Created by Frederik Tollund Juutilainen on 09/09/16.
//
//

#ifndef ofApp_h
#define ofApp_h

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxMultiKinectV2.h"
#include "GpuRegistration.h"
#include "ofxSyphon.h"

#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080


class ofApp : public ofBaseApp{
public:
    ofxMultiKinectV2 kinect0;
    ofEasyCam ecam;
    ofVboMesh mesh;
    
    void setup();
    void update();
    void draw();
    void keyReleased(int key){
        if(key == 'h'){
            hideGui = !hideGui;
        }
        if(key == 'f'){
            freezeFrame = !freezeFrame;
        }
        if(key == ' '){
            ofPixels pixels;
            render.readToPixels(pixels);
            string name = ofGetTimestampString();
            ofSaveImage(pixels, name + ".png", OF_IMAGE_QUALITY_BEST);
            cout << " filed saved " << endl;
        }
    }
    
    // GUI
    bool hideGui = false;
    ofxPanel gui;
    ofParameter<bool> screenRender;
    ofParameter<bool> showAxis;
    ofParameter<bool> freezeFrame;
    ofParameter<int> backgroundColor;
    ofParameter<bool> setBackground;
    ofParameter<int> colorMode;
    ofParameter<int> colorChange;
    ofParameter<int> bottomThres;
    ofParameter<int> topThres;
    ofParameter<int> backgroundThreshold;



    // Syphon
    ofFbo render;
    ofxSyphonServer syphon;
    
};


#endif /* ofApp_h */
