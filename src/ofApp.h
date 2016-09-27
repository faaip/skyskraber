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
    }
    
    // GUI
    bool hideGui = false;
    ofxPanel gui;
    ofParameter<bool> screenRender;
    
    // Syphon
    ofFbo render;
    ofxSyphonServer syphon;
    
    // GPU Registration
    ofTexture colorTex0;
    ofTexture depthTex0;
    ofTexture irTex0;
    GpuRegistration gr;
    bool process_occlusion = true;
    
    
};


#endif /* ofApp_h */
