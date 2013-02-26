

#pragma once

#include "ofMain.h"
#include "statistics.h"




//---------------------------------------------------------------
class dataDisplay {
    
public:
    
    
    float minVal, maxVal;
    vector < float > values;
    
    dataDisplay();
    void draw(ofRectangle rect);
    void addValue (float val);
    
    double sum, mean, var, dev, skew, kurt;
    //computeStats(v.begin( ), v.end( ), sum, mean, var, dev, skew, kurt);

};
//---------------------------------------------------------------
