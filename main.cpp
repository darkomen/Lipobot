#include <ooml/core.h>
#include <ooml/components.h>
#include <components/RoundedTablet.h> 
#include <parts/ArduinoUNO.h>
#include <parts/FutabaS3003.h>
#include <parts/BallWheelHolder.h>

#include "config.h"
#include <fstream>
#include <iostream>
 
using namespace std;
 
int main()
{
	IndentWriter body_writer;
	// El valor del ancho viene dado por el tamaño de los servos y la distancia entre ellos
	LX = ((LX_servo*2) + DX_servos);

	////////////
	// CUERPO //
	////////////
	Component body       = RoundedTablet(LX,LY,LZ,10,true,true,true,true,100,false);   // Estructura básica.
	Component body_hole  = RoundedTablet((LX-(TH*2)),(LY-(TH*2)),30,6,true,true,true,true,100,false).translate(TH,TH,3); // Vaciado interior.
	Component body_hole2 = RoundedTablet((LX-(TH*2)),LY_servo,5,6,true,true,true,true,100,false).translate(TH,LY-(LY_servo+20),-1); // Hueco para pasar cables.
	Component chasis     = body - body_hole - body_hole2;  // Creamos el chasis.

	chasis.addLink(RefSys((LX_servo),(LY-20),(LZ_servo+3.5)).relRotate(0,90,180));   // link 0 Servo Izquierda
	chasis.addLink(RefSys((LX_servo+DX_servos),(LY-20),3.5).relRotate(0,-90,180));	  // link 1 Servo Derecha
	chasis.addLink(RefSys((LX/2),(TH/2),0).relRotate(0,0,0));		  // link 2 Rueda loca
	
	//	Rueda loca
	 Component free_wheel = BallWheelHolder::create(34.5).translate(0,0,0);
	 Component wheel_hole = Cylinder::create(7,40,60,false).translate(0,0,6);
	 Component wheel_nut = Cylinder::create(3.6,3.1,6,false).translate(0,0,3);
	 Component rueda_loca =   free_wheel - wheel_nut - wheel_hole;

	// Arduino
	Component arduino = ArduinoUNO(true,10,false).translate(20,5,0);
	arduino.rotate(0,180,-90);


	Component main_body = chasis
						  + FutabaS3003(true,false).moveToLink(chasis, 0)
						  + FutabaS3003(true,false).moveToLink(chasis, 1)						  
						  + free_wheel.moveToLink(chasis,2)
						  - wheel_nut.moveToLink(chasis,2)
						  - wheel_hole.moveToLink(chasis,2)
						  ;	
    body_writer << LinksView(chasis);
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