#include <ooml/core.h>
#include <ooml/components.h>
#include <components/RoundedTablet.h> 
#include <parts/ArduinoUNO.h>
#include <parts/FutabaS3003.h>
#include <parts/BallWheelHolder.h>

#include <fstream>
#include <iostream>
 
using namespace std;
 
int main()
{

	// Base inicial
	Component body = RoundedTablet(80,100,23,10,true,true,true,true,100,false);
	// Hueco completo
	Component body_hole = RoundedTablet(70,90,30,6,true,true,true,true,100,false).translate(5,5,3);
	// Hueco en medio
	Component body_hole2 = RoundedTablet(70,40,5,6,true,true,true,true,100,false).translate(5,42,0);
	
	//	Rueda loca
	 Component free_wheel = BallWheelHolder::create(38.5);
	 free_wheel.translate(40,3,0);
	 Component wheel_hole = Cylinder::create(7,40,65,false).translate(40,3,6);
	 Component wheel_nut = Cylinder::create(3.6,3.1,6,false).translate(40,3,3);


	// Arduino
	IndentWriter body_writer;
	Component arduino = ArduinoUNO(true,10,false).translate(20,5,0);
	arduino.rotate(0,180,-90);



	// Servos
	Component l_servo = FutabaS3003(true,false);
	l_servo.rotate(0,-90,0);
	Component r_servo = l_servo.mirroredCopy(27,0,0);
	r_servo.translate(30,0,0);

	Component servos = l_servo + r_servo;
	servos.translate(25,42,3.5);



	Component main_body = body
						  - body_hole
						  - body_hole2
						  - arduino
						  - servos
						  + free_wheel
						  - wheel_nut
						  - wheel_hole
						  ;	
    
    body_writer << main_body;
 
	ofstream file("robot.scad");
	if (file){
		//generate OpenScad code of the translated cube and save it into file;
		file << body_writer;
		file.close();
	}else{
		cerr << "Error, cannot open the file" << endl;
	}
 
	cout << "Done" << endl;
 
	return 0;
}