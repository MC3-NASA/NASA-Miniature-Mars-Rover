#include <save.h>
#include <SerialCommand.h>
SerialCommand sCmd;

int* getIndex(char* command);
save save;
String sentence;
int miniMap[64][64];
int indexMiniMap[2]; //Static variable that holds the current index of the matrix. 
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    //save.setup();
    while (!Serial);
    sCmd.addCommand("M#M", miniMapHandler);
}

void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available() > 0)
   {
      sCmd.readSerial();
   }

   //char* command = "#0000016001000100111002";
   //int* nums = getIndex(command);
   //storeIntoMap(command+7);
//   Serial.print(nums[0]);
  // Serial.print(" , ");
  // Serial.println(nums[1]);
   delay(100);
}

void miniMapHandler () {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    //Serial.println(arg);
    getIndex(arg);
    if(indexMiniMap[0] < 64){

      if(indexMiniMap[1] == 4){
        indexMiniMap[0] += 1;
        indexMiniMap[1] = 0;
        Serial.print("M_M");
        Serial.print(indexMiniMap[0]);
        Serial.print(",");
        Serial.println(indexMiniMap[1]);
      }else{
        indexMiniMap[1] += 1;
        Serial.print("M_M");
        Serial.print(indexMiniMap[0]);
        Serial.print(",");
        Serial.println(indexMiniMap[1]);
      }

    }else{
      Serial.println("M_D");
    }

  }
  else
    Serial.println("no message");


  delay(10);
}

//Gets the index of the map to be stored. Max value is (MXN)/16.
int* getIndex(char* command){
  if(*command == '#'){ //#Character represents that this is map data.
    command++;
    char num[3];
    num[0] = *command;
    num[1] = *(++command);
    num[2] = *(++command);
    command++;
    indexMiniMap[0] = atoi(num);
    num[0] = *command;
    num[1] = *(++command);
    num[2] = *(++command);
    indexMiniMap[1] = atoi(num);
    command++;
    return indexMiniMap;
  }
  indexMiniMap[0] = -1;
  indexMiniMap[1] = -1;
  return indexMiniMap;
}

//Stores the command into the miniMap.
void storeIntoMap(char* command){
  char c[1];
  for(int i = 0; i < 16; i++){
    c[0] = *(command+i);
    miniMap[indexMiniMap[0]][indexMiniMap[1]+i] = atoi(c);
    //Serial.print(miniMap[indexMiniMap[0]][indexMiniMap[1]+i]);
  }
  //Serial.println(" ");
}
