#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

typedef struct {
    float time;
    float data[7];    
} dataFrame;

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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        //-------------------------------------
        ofVideoGrabber grabber;
        vector < dataFrame > frame;
        ofImage grabberImage;
        string folderName;
        ofxXmlSettings XML;
        int recordingImageCount;
    
        void writeFrameToXml ( dataFrame & d, ofxXmlSettings & xml);
    
        //-------------------------------------
        bool bRecording;
        float recordingStartTime;
        int lastRecordedFrame;
    
    
        //--------------------------------------
        ofSerial serial;
        string message;
    
    
    
    
};
