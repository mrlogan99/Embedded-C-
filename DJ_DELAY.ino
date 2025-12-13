int redRelay = 7;    
int greenRelay = 8; 
int againred = 9;   

void setup() {
  pinMode(redRelay, OUTPUT); 
  pinMode(greenRelay, OUTPUT); 
  pinMode(againred, OUTPUT); 
}

void loop() {

  digitalWrite(redRelay, HIGH);   

  delay(100);                    
  digitalWrite(redRelay, LOW);    
  delay(100);                    

  digitalWrite(greenRelay, HIGH); 
  delay(100);                    
  digitalWrite(greenRelay, LOW);  
  delay(100);                     

  digitalWrite(againred, HIGH);   
  delay(100);
  digitalWrite(againred, LOW);  
  delay(100);
}
