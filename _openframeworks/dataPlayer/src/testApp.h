#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "dataDisplay.h"


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

        bool bHaveSomething;
        vector < dataFrame > frames;
        vector < string > fileNames;
        vector < float > times;
        ofxXmlSettings XML;
    
        float maxTime;
        float startTime;
        ofImage playback;
        int indexOfLoadedImage;
        int indexOfLastFrameData;
    
    
        dataFrame current;
    
        dataDisplay DD[7];
    
        
    
    
        //vector < ofImage > images;
    
    
};

