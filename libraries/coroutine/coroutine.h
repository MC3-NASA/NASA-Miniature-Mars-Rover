#include <Arduino.h>

class coroutine{
	public:
    bool readyState = false;
    int elaspedTime = 0;
    int waitTime = 0;
    int startTime = 0;

	void loop(){
        if(elaspedTime >= waitTime){
            readyState = true;
        }else{
        }
        elaspedTime =  millis() - startTime;
    }
	void setup(int HowLong){
        waitTime = HowLong;
        startTime = millis();   
    }

    void reset(){
        
        if(readyState == true){
            readyState = false;
            startTime = millis();
            elaspedTime =  millis() - startTime;
        }

    }
};