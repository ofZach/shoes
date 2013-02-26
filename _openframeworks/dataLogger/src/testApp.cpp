#include "testApp.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;


// todo: (a) pick right serial
// (b) send serial message





vector < string > getMatchedStrings (string stringToParse, string regex );
vector < string > getMatchedStrings (string contents, string regex, int & lastPos ){
    
    vector < string > results;
    RegularExpression regEx(regex);
    RegularExpression::Match match;
    
    lastPos = 0;
    while(regEx.match(contents, match) != 0) {
        
        // we get the sub string from the content
        // and then trim the content so that we
        // can continue to search
        string foundStr = contents.substr(match.offset, match.length);
        contents = contents.substr(match.offset + match.length);
        
        
        lastPos += match.offset + match.length;
        
        results.push_back(foundStr);
        
    }
    return results;
}




//--------------------------------------------------------------
void testApp::setup(){

    grabber.initGrabber(160,120);
    
    grabberImage.setUseTexture(false);
    grabberImage.allocate(160,120,OF_IMAGE_COLOR);
    
    recordingImageCount= 0;
    
    
    
    serial.listDevices();
    
    //std::exit(0);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    serial.setup(9, 115200);
    
    serial.writeByte('a');
    
    ofSetVerticalSync(true);
    message = "";
    
    message = "";
    
}

//--------------------------------------------------------------
void testApp::update(){

    unsigned char bytes[100];
    int howMany = serial.readBytes(bytes, 100);
    
    for (int i = 0; i < howMany; i++){
        message += (bytes[i]);
    }

    int pos = 0;
    vector < string > strs  = getMatchedStrings(message, "\\$.*?#", pos);
    
    if (message.size() > 0){
        message = message.substr(pos, message.size());
    }
    
    for (int i = 0; i < strs.size(); i++){
        string val = strs[i].substr(1, strs[i].size()-2);
        vector < string > valStrings = ofSplitString(val, ",");
        dataFrame data;
        
        //cout << endl;
        if (valStrings.size() == 7)
            for (int j = 0; j < 7; j++){
                data.data[j] = ofToFloat(valStrings[j]);
                
               // cout << data.data[j]  << " , ";
            }
        
        if (bRecording == true){
            float timeNow = ofGetElapsedTimef() - recordingStartTime;
            data.time = timeNow;
            writeFrameToXml(data, XML);
            
        }
        //cout << endl;
        //cout << "got good data " << ofGetFrameNum() << endl;
        //gotGoodData(data);
    }
    
    
    
    
    
    
    
    grabber.update();
    if (bRecording && grabber.isFrameNew()){
        
        
        
        float timeNow = ofGetElapsedTimef() - recordingStartTime;
        grabberImage.setFromPixels(grabber.getPixelsRef());
        
        string numeric = ofToString(recordingImageCount);
        
        for (int i = numeric.size(); i< 5; i++){
            numeric = "0" + numeric;
        }
        
        grabberImage.saveImage(folderName + "/" + numeric + "_" + ofToString(timeNow) + ".jpg");
        
        recordingImageCount++;
        
        // do you have new data?
        
        
//        dataFrame d;
//        for (int i = 0; i < 7; i++){
//            d.data[i] = ofRandom(0,1);
//        }
//        d.time = timeNow;
//        frame.push_back(d);
//        
//        writeFrameToXml(d, XML);
    }
}

void testApp::writeFrameToXml ( dataFrame & d, ofxXmlSettings & xml){
    
    
    xml.addTag("frame");
    
    int count = xml.getNumTags("frame");
    
    xml.pushTag("frame", count - 1);
    
    xml.addValue("time", d.time);
    for (int i = 0; i < 7; i++){
        xml.addValue("data" + ofToString(i),  d.data[i]);
    }
    
    xml.popTag();
    //xml.addValue(
    
}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(255);
    grabber.draw(20,20);
    
    if (bRecording){
        ofFill();
        ofSetColor(255,0,0);
        ofTriangle(0,0,10,0,0,10);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == ' '){
    
        
        if (!bRecording){
            folderName  = ofGetTimestampString();
            ofDirectory dir(folderName);
            dir.create();
            recordingStartTime = ofGetElapsedTimef();
            recordingImageCount = 0;
            XML.clear();
        } else {
            
            XML.saveFile(folderName + "/_data.xml");
        }
        
        bRecording =!bRecording;
    }
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

    cout << dragInfo.files[0] << endl;
    ofDirectory dir;
    dir.listDir(dragInfo.files[0]);
    dir.sort();
    
    for (int i = 0; i < dir.size(); i++){
        cout << dir.getName(i) << endl;
    }
    
    
}