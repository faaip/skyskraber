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
    gui.add(backgroundColor.set("backgroundColor", 0,0,255));
    
    
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
    ecam.setDistance(200);
}

void ofApp::update() {
    ofSetBackgroundColor(backgroundColor);
    
    kinect0.update();
    if (kinect0.isFrameNew()) {
        // Mesh
        mesh.clear();
        {
            int step = 2;
            int h = kinect0.getDepthPixelsRef().getHeight();
            int w = kinect0.getDepthPixelsRef().getWidth();
            for(int y = 0; y < h; y += step) {
                for(int x = 0; x < w; x += step) {
                    float dist = kinect0.getDistanceAt(x, y);
                    if(dist > 50 && dist < 500) {
                        ofVec3f pt = kinect0.getWorldCoordinateAt(x, y, dist);
                        ofColor c;
                        float h = ofMap(dist, 50, 200, 0, 255, true);
                        c.setHsb(h, 255, 255);
                        mesh.addColor(c);
                        mesh.addVertex(pt);
                    }
                }
            }
            
        }
    }
    
}

void ofApp::draw()
{
    render.begin();
//    
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ofClear(0);
    
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

