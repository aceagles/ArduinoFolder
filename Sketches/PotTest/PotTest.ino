void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(modifiedMap(analogRead(A0), 0, 1023, -30, 30));
  Serial.println("=====");
  delay(500);
}

double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 //temp = (int) (4*temp + .5);
 return (double) temp;
}
