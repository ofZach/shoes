#include "testApp.h"



ofSoundPlayer snd;


float px, py, pz;

int nNonStepFrames = 0;
void detectStep(float x, float y, float z){
    
    float xx = x;
    float yy = y;
    float zz = z;


    float dx = x - px;
    float dy = y - py;
    float dz = z - pz;

    float size = sqrt(dx*dx + dy*dy + dz*dz);
    
   // cout << size << endl;
    if (size > 20) {
        
        //if (nNonStepFrames > 2){
            snd.play();
        //}
        
        nNonStepFrames = 0;
        
    } else {
        
        nNonStepFrames ++;
        
    }//cout << size << endl;
    //cout << size << endl;
    
    px = xx; py = yy; pz = zz;
//    float xx = x;
//    float yy = y;
//    float zz = z;
//    
//    float dot = (px * xx) + (py * yy) + (pz * zz);
//    float a = ABS(sqrt(px * px + py * py + pz * pz));
//    float b = ABS(sqrt(xx * xx + yy * yy + zz * zz));
//    
//    dot /= (a * b);
//
//    if (dot <= 0.75) {
//        snd.play();
//        //cout << "step " << ofGetFrameNum() << endl;
//    }
//    
//    px = xx; py = yy; pz = zz;
}


 float   mLimit = 10;
 float   mLastValues[3*2];
 float   mScale[2];
 float   mYOffset;

 float   mLastDirections[3*2];
float   mLastExtremes[3*2][3*2];
 float   mLastDiff[3*2];
 int     mLastMatch = -1;

void detectStep2(float values[3]){
    
    float h = 480;
    mYOffset = 480 * 0.5f;
    mScale[0] = - (h * 0.5f * (1.0f / ( 9.80665 * 2)));
    mScale[1] = - (h * 0.5f * (1.0f / 60.0));
    
    
    int j = 1;
    float vSum = 0;
    for (int i=0 ; i<3 ; i++) {
        float v = mYOffset + values[i] * mScale[j];
        vSum += v;
    }
    int k = 0;
    float v = vSum / 3;
    
    float direction = (v > mLastValues[k] ? 1 : (v < mLastValues[k] ? -1 : 0));
    if (direction == - mLastDirections[k]) {
        // Direction changed
        int extType = (direction > 0 ? 0 : 1); // minumum or maximum?
        mLastExtremes[extType][k] = mLastValues[k];
        float diff = fabs(mLastExtremes[extType][k] - mLastExtremes[1 - extType][k]);
        
        if (diff > mLimit) {
            
            bool isAlmostAsLargeAsPrevious = diff > (mLastDiff[k]*2/3);
            bool isPreviousLargeEnough = mLastDiff[k] > (diff/3);
            bool isNotContra = (mLastMatch != 1 - extType);
            
            if (isAlmostAsLargeAsPrevious && isPreviousLargeEnough && isNotContra) {
                
                snd.play();
                //cout << "step !! " << ofGetFrameNum() << endl;
                //Log.i(TAG, "step");
//                for (StepListener stepListener : mStepListeners) {
//                    stepListener.onStep();
//                }
                mLastMatch = extType;
            }
            else {
                mLastMatch = -1;
            }
        }
        mLastDiff[k] = diff;
    }
    mLastDirections[k] = direction;
    mLastValues[k] = v;
    
}





//--------------------------------------------------------------
void testApp::setup(){
    
    snd.loadSound("Rimhot.wav");
    snd.setVolume(0.4);
    snd.setMultiPlay(true);
    indexOfLoadedImage = -1;
    indexOfLastFrameData = -1;
    
    playback.allocate(160,120, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void testApp::update(){

    
    
    if (bHaveSomething == true){

        // slowdown? 
        float currentTime = (ofGetElapsedTimef() - startTime) * 0.2;
        
        //float currentTime = (ofGetElapsedTimef() - startTime);
        while (currentTime > maxTime) currentTime -= maxTime;
        
        
        
        // find the best image not greater then current time
        int index = -1;
        for (int i = 0; i < times.size(); i++){
            if (times[i] < currentTime) index = i;
            
            
        }
        
        if (index != -1 && indexOfLoadedImage != index){
            playback.loadImage(fileNames[index]);
            indexOfLoadedImage = index;
        }
        
        // find the best frame of data not great then the current frame
        
        
        index = -1;
        for (int i = 0; i < frames.size(); i++){
            if (frames[i].time < currentTime) index = i;
        }
        
        //cout << index;
        if (index != -1 && indexOfLastFrameData != index){
            //current =
            dataFrame temp = frames[index];
            for (int i = 0; i < 7; i++){
                current.data[i] = 0.9 * current.data[i] + 0.1 * temp.data[i];
            }
            //cout << index << endl;
            indexOfLastFrameData = index;
            
            
            
            float tempv[3];
            tempv[0] = current.data[1];
            tempv[1] = current.data[2];
            tempv[2] = current.data[3];
            
            //detectStep2(tempv);
            detectStep(current.data[1],current.data[2], current.data[3]);
            
            
            for (int i = 0; i < 7; i++){
                DD[i].addValue(current.data[i]);
            }
            
        }

        
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    
    ofSetColor(255);
    
    playback.draw(700,0, 640,480);
    
    
    ofDrawBitmapStringHighlight(ofToString(current.time), ofPoint(600,50));
    for (int i = 0; i < 7; i++){
        ofDrawBitmapStringHighlight(ofToString(current.data[i]), ofPoint(600,50 + 25 + 25 * i));
    }
    
    for (int i = 0; i < 7; i++){
        ofRectangle rect;
        rect.set(0,  i * 60, 400, 50);
        DD[i].draw(rect);
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
    
    //cout << dragInfo.files[0] << endl;
    
    frames.clear();
    fileNames.clear();
    times.clear();
    
    ofDirectory dir;
    dir.listDir(dragInfo.files[0]);
    dir.sort();
    
    // skip _xml !
    for (int i = 0; i < dir.size()-1; i++){
        string time = dir.getName(i).substr(6, dir.getName(i).size() - 4);
        times.push_back( ofToFloat(time) );
        fileNames.push_back(dir.getPath(i));
        
        //cout << times[i] << " " << fileNames[i] << endl;
    }
    
    maxTime = times[times.size()-1];
    startTime = ofGetElapsedTimef();
    
    bHaveSomething = true;
    
    
    XML.clear();
    XML.loadFile(dir.getPath(dir.size()-1));
    
    int count = XML.getNumTags("frame");
    
    for (int i = 0; i < count; i++){
    
        dataFrame data;
        
        XML.pushTag("frame", i);
        
        data.time = XML.getValue("time", 0.0);
        for (int i = 0; i < 7; i++){
            data.data[i] = XML.getValue("data" + ofToString(i), 0.0);
        }
        
        //cout << data.time << " " << data.data[0] << endl;
        
        XML.popTag();
        
        frames.push_back(data);
    }
    
    
   
    
    
    
}