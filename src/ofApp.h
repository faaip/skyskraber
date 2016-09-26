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
#include "ofxMultiKinectV2.h"
#include "GpuRegistration.h"


class ofApp : public ofBaseApp{
public:
    ofxMultiKinectV2 kinect0;
    ofEasyCam ecam;
    ofVboMesh mesh;
    
    void setup();
    void update();
    void draw();
    
    // GPU Registration
    ofTexture colorTex0;
    ofTexture depthTex0;
    ofTexture irTex0;
    GpuRegistration gr;
    bool process_occlusion = true;
    
    
};


#endif /* ofApp_h */
