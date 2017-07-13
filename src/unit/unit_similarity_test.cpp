//#include <drawstuff/drawstuff.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include <math.h>
#include "MathUtils.h"
#include "ControlPBP.h"
#include "UnityOde.h"
#include <ode/ode.h>
#include <stdio.h>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <unit/from_robot.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>



using namespace std;
using namespace AaltoGames;

static const int ANGLE=0;  //state variable index
static const int AVEL=1;	//state variable index
const dReal *pos,*R1,*pos2,*R2,*pos3,*R3;





float conv = 0.0;
bool  check;
float pos_robot;
float ang_robot;
int   body, hinge,body1;
dReal geom,geom1;
float last_angle ;
float change;


int cnt = 0 ;



ros::ServiceServer service;
ros::Publisher chatter_pub;
std_msgs::Float32 msg; 

ofstream sim_test;

   


bool robot(unit::from_robot::Request &req, unit::from_robot::Response &res)
{
  
ros::Time begin = ros::Time::now();

pos_robot= req.position;
ang_robot = req.angle ;

	
 	
  
   


		odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
		odeJointSetHingeParam(hinge,dParamVel,0.1);




		stepOde(0);
	


		

		
  	
  		//myfile.close();

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining

	    pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
	   float angle=odeJointGetHingeAngle(hinge);


       res.command = 0.1;
	
	  
		//printf("cnt %d\n",cnt );
		//final << " best control" <<std::setw(12) << control << std::setw(12) <<"best cost" <<std::setw(12)<< std::setw(12)<<cost <<std::setw(12)<< "position"<< std::setw(12)<<last_position <<endl ;
		sim_test << std::setw(20)<<"position sim " <<std::setw(20)<<pos[0] << std::setw(12)<<" angle" <<std::setw(20)<< angle*180/3.1416 << std::setw(25)<< "original  ball " << std::setw(20)<< pos_robot<<std::setw(12)<<"org angle"<<ang_robot*180/3.1416 <<endl ;

	   ros::Time end = ros::Time::now();
	   double dt = (begin - end).toSec();

	   ROS_INFO("dt %f", dt);
	 /*  float testing= 1e-1 ;
	   printf(" testing %f\n", testing);*/


		//printf("FINAL Posx %1.3f,posz = %f  angle %1.3f, cost=%1.3f, control %f \n",pos[0],pos[2],angle*180/3.1416,cost,control);
	    printf("position sim %f  angle %1.3f \n",pos[0],angle*180/3.1416);
	  	printf("original  ball %f  org angle %f \n", pos_robot, ang_robot*180/3.1416);


		
		   
	/*	int i = 0;
		loop : cin >> i;
		if ( i != 1)
		goto loop;*/
		
	
}







int main(int argc, char **argv)
{

	ros::init(argc,argv, "talker");
	ros::NodeHandle n ;


	 chatter_pub = n.advertise<std_msgs::Float32>("from_ode", 1);
	 service = n.advertiseService("from_robot", robot);
	


    ROS_INFO("%f", msg.data);
//myfile.open ("example.txt");

    sim_test.open ("sim_step_100_cfm_e-1_erp_0.2.txt");
 
   

	//Allocate one simulation context for each sample, plus one additional "master" context

    initOde(1);
	odeWorldSetGravity(0, 0, -9.81f);


		//Build the model:
	//create the stage
    geom = odeCreateBox(.4,.4,0.05);
	dQuaternion q;
	//dQFromAxisAndAngle(q,0,0,1,0);
	//odeGeomSetQuaternion(geom,q);

	//Create a body and attach it to the geom
    body=odeBodyCreate();
   //odeBodySetMass (body, 0.94f);

    odeMassSetBoxTotal(body, 0.94f, 0.4, 0.4 , 0.05) ;
    odeBodySetPosition(body,0,0,2.5);

	odeGeomSetBody(geom,body);

		


	hinge=odeJointCreateHinge();
	//printf("joint ID : %d",hinge);

	odeJointAttach(hinge,body,0);
	odeJointSetHingeAnchor(hinge,0,0,2.55);
	odeJointSetHingeAxis(hinge,0,1,0);
	//odeJointSetHingeParam(hinge,dParamFMax,0); //this hinge has no motor, we'll control it directly using torques
	//odeJointSetHingeParam(hinge,dParamVel,1.0f);



	///////CREATING BALL//////////

	 geom1 = odeCreateSphere(.03f);
	//Create a body and attach it to the geom
	body1=odeBodyCreate();


	//odeMassSetSphereTotal(body1,.05f,1.0f);
	 //odeMassSetSphere()


    
    odeMassSetSphereTotal(body1,0.04,0.03);
    odeBodySetPosition(body1,0.00428,0,2.554f);
	//odeBodySetMass (body1, 0.04);
	odeGeomSetBody(geom1,body1);

	//Set position of the ball
	







	while (ros::ok())
	{

       
      ros::spinOnce();
  
	//printf("spinning");
		

	}
		printf("Done, press enter.");
		getchar();

		return 0;
	}










