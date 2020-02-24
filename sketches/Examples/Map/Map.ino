#include <Map.h>
Map maps = Map(5, 2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  maps.update(0.5, 90);
  maps.update(0.5, 0);
  for (int i = 0; i < 5; i++)
  {
    Serial.print(maps.read(i));
    delay(10);
  }
  Serial.println();
  Serial.println();
  delay(100);
  vector3D v(0, 0.4, 0);
  bool g = maps.read(v);
  Serial.println(g);
  Serial.println();
  delay(100);
  //Serial.println(maps.read(0));
  maps.flush();
  //bool f = maps.read(v);
  //Serial.println(f);
  //Serial.println(maps.read(v));

}
