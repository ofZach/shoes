#include "dataDisplay.h"



dataDisplay::dataDisplay(){
    minVal = 100000;
    maxVal = -1000000;
}
    
void dataDisplay::draw(ofRectangle rect){
    ofNoFill();
    ofSetColor(255);
    ofRect(rect.x, rect.y, rect.width, rect.height);
    
    ofFill();
    
    if (values.size() > 0){
    
    ofBeginShape();
    ofVertex(rect.x, rect.y + rect.height);
    for (int i = 0; i < values.size(); i++){
        ofPoint pt;
        pt.x = ofMap(i, 0, values.size()-1, rect.x, rect.x + rect.width);
        pt.y = ofMap(values[i], minVal - 0.0000001, maxVal + 0.000001, rect.y + rect.height,  rect.y);
        //cout << pt << endl;
        ofVertex(pt.x, pt.y);
    }
    ofVertex(rect.x + rect.width, rect.y + rect.height);
    ofEndShape(true);
    
    ofSetColor(100,100,100);
    
    ofPoint pt;
    pt.y = ofMap(mean, minVal - 0.0000001, maxVal + 0.000001, rect.y + rect.height,  rect.y);
    ofLine(rect.x, pt.y, rect.x + rect.width, pt.y);
    }
    //ofDrawBitmapStringHighlight(rect.x + rect.width, <#const ofPoint &position#>)
    
}

    
void dataDisplay::addValue(float val){
        
        // cout << val << endl;
        values.push_back(val);
        if (values.size() > 150){
            values.erase(values.begin());
        }
        
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
    
        computeStats(values.begin( ), values.end( ), sum, mean, var, dev, skew, kurt);
}