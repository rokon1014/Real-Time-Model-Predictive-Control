//#include <drawstuff/drawstuff.h>
#include "ros/ros.h"
#include"std_msgs/String.h"
#include"std_msgs/Float32.h"
#include <math.h>
#include "MathUtils.h"
#include "ControlPBP.h"
#include "UnityOde.h"
#include <ode/ode.h>
#include <stdio.h>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <unit/for_double.h>



using namespace std;
using namespace AaltoGames;

static const int ANGLE=0;  //state variable index
static const int AVEL=1;	//state variable index
const dReal *pos,*R1,*pos2,*R2,*pos3,*R3;




//vairable initilization for simulation
float last_position =0.0588, current_position=0.0588, ball_vel;
float conv = 0.0;
bool  check;
float pos_robotx;
float pos_roboty;
float ang_robot;
float last_angle ;
float change;
int   cnt = 0 ;
int body, hinge, hinge1,hinge2, body1, body_pole1,body_pole2,body_pole3,body_pole4,body2,body_obs;
int geom,geom1,geom_pole1,geom_pole2,geom_pole3,geom_pole4,geom2,geom_obs;
int joint,joint1,joint2,joint3,joint4,joint_obs;



//variable initilization for optimization
const int nSamples=60;
float timeStep=1.0f/25.0f;
ControlPBP pbp;
int   nTimeSteps=15;		
const int nStateDimensions=2;
const int nControlDimensions=2;
float minControl=-1;	
float maxControl=1;		//upper sampling bound
float controlMean=0; 
float C=10;
float controlStd=0.8f*C;	
float controlDiffStd=100.0f*C;	
float controlDiffDiffStd=1000.0f*C; 
float mutationScale=0.25f;		


//initializing ROS services
ros::ServiceServer service;
ros::Publisher chatter_pub;
std_msgs::Float32 msg; 




bool robot(unit::for_double::Request &req, unit::for_double::Response &res)
{

	ros::Time begin = ros::Time::now(); //for measuring simulation and optimization time

	//ROS action service
	pos_robotx= req.positionx;   
	pos_roboty= req.positiony;
	ang_robot = req.angle ;


	//NOT necessary
	check = true ;
	msg.data = conv;
	chatter_pub.publish(msg); 	
	



	setCurrentOdeContext(0); 
	restoreOdeState(0);
	const dReal *pos_first = odeBodyGetPosition(body1);
	odeBodySetPosition(body1,pos_robotx,pos_roboty,pos_first[2]);  // set the position of the ball in simulator using feedback
	saveOdeState(0);
	



		//init all trajectories to the master state
		for (int i=0; i<nSamples; i++)
		{
			//activate the context for this sample
			setCurrentOdeContext(i+1); 
			//load the state from the master context
			restoreOdeState(0);

			const dReal *pos = odeBodyGetPosition(body1);
		    odeBodySetPosition(body1,pos_robotx,pos_roboty,pos[2]);  // set position of the ball using feedback for all the states
			//const dReal *pos1 = odeBodyGetPosition(body1);
			saveOdeState(i+1,0); 


		}

		setCurrentOdeContext(0); 
		restoreOdeState(0); 


		const dReal *pos2 = odeBodyGetPosition(body1);
		odeBodySetPosition(body1,pos_robotx,pos_roboty,pos2[2]);
		float stateVector[2]={pos2[0],pos2[1]};
		pbp.startIteration(true,stateVector);

		

		//simulate forward 
		for (int k=0; k<nTimeSteps; k++)

		{


			//signal the start of a planning step
			pbp.startPlanningStep(k);
						//TODO: for multithreaded operation, one would typically run each iteration of the following loop in a separate thread. 
			//The getControl(), getPreviousSampleIdx(), and updateResults() methods of the optimizer are thread-safe.
			//The physics simulation is also thread-safe as we have full copies of the simulated system for each thread/trajectory

			for (int i=0; i<nSamples; i++)
			{
				//get control from C-PBP
				float control, control1;
				pbp.getControl(i,&control);
				pbp.getControl(i,&control1);

				//get the mapping from this to previous state (affected by resampling operations)
				int previousStateIdx=pbp.getPreviousSampleIdx(i);
				//simulate to get next state.
				setCurrentOdeContext(i+1);
				//printf("previous state id: %d \n",previousStateIdx);
				restoreOdeState(previousStateIdx+1); //continue the a trajectory (selected in the resampling operation inside ControlPBP)

			    dReal MaxForce = dInfinity;
				odeJointSetAMotorParam(joint,dParamFMax1,dInfinity); 
				odeJointSetAMotorParam(joint,dParamVel1,control  );

				odeJointSetAMotorParam(joint,dParamFMax2,dInfinity); 
				odeJointSetAMotorParam(joint,dParamVel2,control1  );

				//STEPPING SIMULATION FORWAARD
				stepOde(1);
				const dReal *pos = odeBodyGetPosition(body1);
				float cost=squared((0.03-pos[0])*40.0f)+squared((pos[1])*10.0f); //+squared(control* 0.5f)+ squared(control1*0.5f);


				//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as in this simple case.
				//instead, one may use arbitrary state features
				float stateVector[2]={pos[0],pos[1]};
				float cont[2]= {control,control1};

				//float stateVector[2]={angle,aVel};
				pbp.updateResults(i,cont,stateVector,cost);

				//printf("cost in the loop end: %f posz: %f \n",cost,pos[2]);
			}
				//save all states, will be used at next step (the restoreOdeState() call above)
				for (int i=0; i<nSamples; i++)
			{
				saveOdeState(i+1,i+1);
			}

			//signal the end of the planning step. this normalizes the state costs etc. for the next step
			pbp.endPlanningStep(k);
			}
			//signal the end of an iteration. this also executes the backwards smoothing pass
		pbp.endIteration();
		
		//deploy the best control found using the master context
		float control[2];
		pbp.getBestControl(0,control);
		float cost=(float)pbp.getSquaredCost();
		setCurrentOdeContext(0);
		restoreOdeState(0);

		


	    if (cnt >3)   //Ignoring first 3 results 
	    {
	    	
           
    	dReal MaxForce = dInfinity;
		odeJointSetAMotorParam(joint,dParamFMax1,5);
		odeJointSetAMotorParam(joint,dParamVel1,control[0]);

		odeJointSetAMotorParam(joint,dParamFMax2,5);
		odeJointSetAMotorParam(joint,dParamVel2,control[1]);

		}

		else 
		{
			
		odeJointSetAMotorParam(joint,dParamFMax1,5);
		odeJointSetAMotorParam(joint,dParamVel1,0.0);

		odeJointSetAMotorParam(joint,dParamFMax2,5);
		odeJointSetAMotorParam(joint,dParamVel2,0.0);


		}

		stepOde(0);
		saveOdeState(0);


	  	pos = odeBodyGetPosition(body1);


	  	 //send the control signal as response
        res.commandx = control[0];
        res.commandy = control[1];
		cnt = cnt +1 ;
		last_position = pos[0] ;
		


		ros::Time end = ros::Time::now();
		double dt = (begin - end).toSec();  //measuring simulaiton time
		ROS_INFO("dt %f", dt);
		
/*		int i = 0;
		loop : cin >> i;
		if ( i != 1)
		goto loop;*/
	
	
}







int main(int argc, char **argv)
{



	ros::init(argc,argv, "talker");
	ros::NodeHandle n ;


	chatter_pub = n.advertise<std_msgs::Float32>("from_ode", 1);
	service = n.advertiseService("for_double", robot);
    ROS_INFO("%f", msg.data);

   

	//Allocate one simulation context for each sample, plus one additional "master" context
	initOde(nSamples+1);
	setCurrentOdeContext(ALLTHREADS);
	odeRandSetSeed(0);
	odeSetContactSoftCFM(0);
	odeWorldSetGravity(0, 0, -9.81f);


	

	//Build the model:
	//create the stage
    geom = odeCreateBox(.4,.4,0.05);
    body=odeBodyCreate();
    odeMassSetBoxTotal(body, 0.94f, 0.4, 0.4 , 0.05) ;
    odeBodySetPosition(body,0,0,2.5);
	odeGeomSetBody(geom,body);

	//create reference object
	 geom2 = odeCreateBox(.1,.1,0.05);
    body2=odeBodyCreate();
    odeMassSetBoxTotal(body2, 0.94f, 0.1, 0.1 , 0.05) ;
    odeBodySetPosition(body2,0,0,2.0);
	odeGeomSetBody(geom2,body2);


	/// create obstacle ///////

	 geom_obs = odeCreateSphere(.025f);
	//Create a body and attach it to the geom
	body_obs=odeBodyCreate() ;
    odeMassSetSphereTotal(body_obs,0.04,0.025);
    odeBodySetPosition(body_obs,0,0,2.554f);
	odeGeomSetBody(geom_obs,body_obs);


	/////////////////


	//create joints
	
	joint = odeJointCreateAMotor();
	odeJointAttach(joint,body,body2);

	joint1 = odeJointCreateBall();
	odeJointAttach(joint1,body,body2);
	odeJointSetBallAnchor(joint1, 0,0,2.5);

	joint3 = odeJointCreateFixed();
	odeJointAttach(joint3,body2,0);
	//odeJointSetHingeAnchor(joint3,0,0,2.2);
	//odeJointSetHingeAxis(joint3,0,0,1);
	odeJointSetFixed (joint3);


	odeJointSetAMotorMode(joint,dAMotorUser);
	odeJointSetAMotorNumAxes(joint,2);
	odeJointSetAMotorAxis(joint,0,1,0,1,0);
	odeJointSetAMotorAxis(joint,1,1,1,0,0);


	joint_obs = odeJointCreateFixed();
	odeJointAttach(joint_obs,body,body_obs);
	odeJointSetFixed (joint_obs);



	///////CREATING BALL//////////

	 geom1 = odeCreateSphere(.03f);
	//Create a body and attach it to the geom
	body1=odeBodyCreate() ;
    odeMassSetSphereTotal(body1,0.04,0.03);
    odeBodySetPosition(body1,0.1,0,2.554f);
	odeGeomSetBody(geom1,body1);






	setCurrentOdeContext(0);
	saveOdeState(0);

    //initialize the optimizer
	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,&minControl,&maxControl,&controlMean,&controlStd,&controlDiffStd,&controlDiffDiffStd,mutationScale,NULL);

	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,false,0.001f);



	while (ros::ok())
	{


        ros::spinOnce();
	//printf("spinning");
		

	}
		printf("Done, press enter.");
		getchar();

		return 0;
	}










