#include "ofApp.h"


//========================================================================
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    // GUI
    gui.setup();
    gui.add(screenRender.set("screenRender", true));
    gui.add(showAxis.set("showAxis", false));
    gui.add(freezeFrame.set("freeze frame", false));
    gui.add(backgroundColor.set("backgroundColor", 0,0,255));
    gui.add(setBackground.set("set background", true));
    gui.add(colorMode.set("color mode: ", 0, 0, 2));
    gui.add(colorChange.set("color change: ", 255,0,255));
    gui.add(bottomThres.set("btm thres: ", 50,0,200));
    gui.add(topThres.set("top thres: ", 200,0,800));
    gui.add(backgroundThreshold.set("background dist ", 1000,50,1000));
    
    // Syphon stuff
    render.allocate(OUTPUT_WIDTH, OUTPUT_HEIGHT, GL_RGBA);
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render.end();
    syphon.setName("kinect_point_cloud");
    
    // Kinect
    kinect0.open(false, true, 0);
    // Note :
    // Default OpenCL device might not be optimal.
    // e.g. Intel HD Graphics will be chosen instead of GeForce.
    // To avoid it, specify OpenCL device index manually like following.
    // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
    
    kinect0.start();
    
    mesh.setUsage(GL_DYNAMIC_DRAW);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    ecam.setAutoDistance(false);
    ecam.setDistance(62);
}

void ofApp::update() {
    if(setBackground)ofSetBackgroundColor(backgroundColor,ofGetMouseX());
    
    if(!freezeFrame) kinect0.update();
//    if (kinect0.isFrameNew()) {
        // Mesh
        mesh.clear();
        {
            int step = 2;
            int h = kinect0.getDepthPixelsRef().getHeight();
            int w = kinect0.getDepthPixelsRef().getWidth();
            for(int y = 0; y < h; y += step) {
                for(int x = 0; x < w; x += step) {
                    float dist = kinect0.getDistanceAt(x, y);
                    if(dist > 50 && dist < backgroundThreshold) {
                        ofVec3f pt = kinect0.getWorldCoordinateAt(x, y, dist);
                        ofColor c;
                        float h = ofMap(dist, bottomThres, topThres, 0, 255, true);
                        if(colorMode == 0){
                            c.setHsb(h, colorChange, 255); // hue
                        }else if(colorMode == 1){
                            c.setHsb(colorChange, h, 255); // saturation
                        }else if(colorMode == 2){
                            c.set(255-(h*colorChange));
                        }
                        mesh.addColor(c);
                        mesh.addVertex(pt);
                    }
                }
//            }
            
        }
    }
}

void ofApp::draw()
{
    render.begin();
    if(setBackground){
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ofClear(0);
    }
    
    if (mesh.getVertices().size()) {
        ofPushStyle();
        glPointSize(2);
        ecam.begin();
        if(showAxis){
            ofDrawAxis(100);
        }
        ofPushMatrix();
        ofTranslate(0, 0, -100);
        mesh.draw();
        ofPopMatrix();
        ecam.end();
        ofPopStyle();
    }
    
    render.end();  // End off frame buffer
    syphon.publishTexture(&render.getTexture());
    
    if(screenRender) render.draw(0, 0,ofGetWidth(),ofGetHeight());
    if(!hideGui)gui.draw();
    
    ofSetWindowTitle("fps: " + ofToString(ofGetFrameRate()));
}

