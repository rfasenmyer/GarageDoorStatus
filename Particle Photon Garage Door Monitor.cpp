//2-24-2018 
//Author: Ryan Fasenmyer
//Purpuse: This program monitors the state of garage door and sends a push notification when the door opens and closes.


int led = D7; //Built in LED on Particle Photon.  Will illuminate to indicate when the door is open.  Will turn off when the door is closed.
int door = D0; //The pin the door switch will be connected to on the Particle Photon.

void setup() {

  pinMode(led, OUTPUT); //indicate that the pin connected to an LED is an output
  pinMode(door, INPUT_PULLUP); //indicate that the pin connected to the door switch is an input with a pullup resistor

//Turn on serial interface for testing    
  Serial.begin(9600);
  //Serial.println("Program Started"); 
  
//send push notification that the system is online
Spark.publish("GarageEvent", "Garage Door Monitor is Online", 60, PRIVATE);

}

void loop() {

    //get current door status
    int currentDoorStatus = digitalRead(door);


    //Call moniterDoor function and pass it the current door status.
     moniterDoor(currentDoorStatus);
    
}



//Garage door moniter function
void moniterDoor(int startingDoorState){
   //Serial.println("Currently Monitoring Closed Door");
   
   //declare currentDoorStatus variable
   int currentDoorStatus; 
    
    //run infinite loop until change is detected in the door state
    while (true) {
        //Keep Cloud Connection Open.  When using a while statement you must initiate the following function to keep the cloud connection active.
        Particle.process();
        
        //Get current state and if 0 turn the LED light off, if 1 turn the LED light on.
        if (startingDoorState == 0) {
            //turn off LED
            digitalWrite(led, LOW);
        } else {
            //turn on LED
            digitalWrite(led, HIGH);
            
        }
        
        //Check to see if door status has changed
        
        //get current door status
        currentDoorStatus = digitalRead(door);
        
        //see if status of door has changed.
        if (currentDoorStatus != startingDoorState){
            //Door reads open, wait one second and check again just to make sure the door is not in transition.
            delay(1000);
        
	        //get current door status again
            currentDoorStatus = digitalRead(door);
            
            //check door status again to see if it is still changed.
	        if (currentDoorStatus != startingDoorState){
	            
	            //Door status has definately changed
	            //Send apropriate notification based on what the starting state was.
                if (startingDoorState ==0) {
                    
                    //send pushover notification
                    Spark.publish("GarageEvent", "The Garage Door Is Open", 60, PRIVATE);
                    Serial.println("Changing The Garage Door From Closed To Open");
                
                    //turn off LED
                    digitalWrite(led, HIGH);
                    //exit out of while loop
                
                } else {
                    
                    //send pushover notification
                    Spark.publish("GarageEvent", "The Garage Door Is Closed", 60, PRIVATE);
                    Serial.println("Changing The Garage Door From Open To Closed");
                
                    //turn off LED
                    digitalWrite(led, LOW);
                    //exit out of while loop
                    
                    
                }
                
                //Since change has occurred we break out of the while loop and return to the main loop
                break;
   
	        }//end if
	        
        }//end if
        
    } //end while
    
} //end monitorOpenDoor function
























