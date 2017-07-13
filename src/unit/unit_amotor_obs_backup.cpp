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


ros::ServiceServer service;
ros::Publisher chatter_pub;
std_msgs::Float32 msg; 






const int nSamples=30;
//physics simulation time step
float timeStep=1.0f/25.0f;


ControlPBP pbp;
int   nTimeSteps=15;		
const int nStateDimensions=2;
const int nControlDimensions=2;
float minControl=-1;	//lower sampling bound
float maxControl=1;		//upper sampling bound
float controlMean=0;	//we're using torque as the control, makes sense to have zero mean
//Square root of the diagonal elements of C_u in the paper, i.e., stdev of a Gaussian prior for control.
//Note that the optimizer interface does not have the C_u as a parameter, and instead uses meand and stdev arrays as parameters.
//The 3D character tests compute the C_u on the Unity side to reduce the number of effective parameters, and then compute the arrays based on it as described to correspond to the products \sigma_0 C_u etc.
float C=10;
float controlStd=0.8f*C;	//sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
float controlDiffStd=100.0f*C;	//sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
float controlDiffDiffStd=1000.0f*C; //sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
float mutationScale=0.25f;		//\sigma_m in the paper





bool robot(unit::for_double::Request &req, unit::for_double::Response &res)
{

	
	//ros::Time begin = ros::Time::now();
	  
	pos_robotx= req.positionx;
	pos_roboty= req.positiony;
	ang_robot = req.angle ;



	//check = true ;
	msg.data = conv;
	chatter_pub.publish(msg); 	
	



	setCurrentOdeContext(0);
	restoreOdeState(0);
	const dReal *pos_first = odeBodyGetPosition(body1);
	
	odeBodySetPosition(body1,pos_robotx,pos_roboty,pos_first[2]);
	//const dReal *pos_second = odeBodyGetPosition(body1);
	//printf(" Pos X %f  Pos Y %f\n", pos_second[0], pos_second[1]);


	
	saveOdeState(0);
	



		//init all trajectories to the master state
		for (int i=0; i<nSamples; i++)
		{
			//activate the context for this sample
			setCurrentOdeContext(i+1); 
			//load the state from the master context
			restoreOdeState(0);


			const dReal *pos = odeBodyGetPosition(body1);
		   odeBodySetPosition(body1,pos_robotx,pos_roboty,pos[2]);
			//const dReal *pos1 = odeBodyGetPosition(body1);

	
			saveOdeState(i+1,0); 


		}

	/*	for (int i=0; i<nSamples; i++)
		{
			setCurrentOdeContext(i);
			const dReal *pos = odeBodyGetPosition(body1);
	      //  printf(" position x :%f y:%f z:%f NUM: %d  \n",pos[0],pos[1],pos[2],i);
		}
		*/
		
		//signal the start of new C-PBP iteration
		setCurrentOdeContext(0); 
		restoreOdeState(0); 


		const dReal *pos2 = odeBodyGetPosition(body1);

		odeBodySetPosition(body1,pos_robotx,pos_roboty,pos2[2]);




		//const dReal *pos = odeBodyGetPosition(body1);
		//float angle=odeJointGetHingeAngle(joint2);

		//printf(" posX x :%f, aVel: %f  \n",pos[0],aVel);
		float stateVector[2]={pos2[0],pos2[1]};
		pbp.startIteration(true,stateVector);

		

		//simulate forward 
		for (int k=0; k<nTimeSteps; k++)

		{


			//signal the start of a planning step
			pbp.startPlanningStep(k);
						//NOTE: for multithreaded operation, one would typically run each iteration of the following loop in a separate thread. 
			//The getControl(), getPreviousSampleIdx(), and updateResults() methods of the optimizer are thread-safe.
			//The physics simulation is also thread-safe as we have full copies of the simulated system for each thread/trajectory

			for (int i=0; i<nSamples; i++)
			{
				//get control from C-PBP
				float control, control1;
				pbp.getControl(i,&control);
				pbp.getControl(i,&control1);

				//printf("k=%i",k);
				//printf("i=%d \n",i);
				//printf("control: %f  control 1 %f\n",control,control1);


				//get the mapping from this to previous state (affected by resampling operations)
				int previousStateIdx=pbp.getPreviousSampleIdx(i);
				//simulate to get next state.
				setCurrentOdeContext(i+1);
				//printf("previous state id: %d \n",previousStateIdx);
				restoreOdeState(previousStateIdx+1); //continue the a trajectory (selected in the resampling operation inside ControlPBP)

				//pos = odeBodyGetPosition(body1);
				//angle=odeJointGetHingeAngle(joint2);
		// printf("before Posx %1.3f,posz = %f  avel %1.3f, \n",pos[0],pos[2],(aVel*180/PI));
		 // printf("control %f \n",control);
				//dVector3 torque={0,control,0};
				//odeBodyAddTorque(body,torque);


 
    dReal MaxForce = dInfinity;
	odeJointSetAMotorParam(joint,dParamFMax1,dInfinity); 
	odeJointSetAMotorParam(joint,dParamVel1,control  );

	odeJointSetAMotorParam(joint,dParamFMax2,dInfinity); 
	odeJointSetAMotorParam(joint,dParamVel2,control1  );

				stepOde(1);

				pos = odeBodyGetPosition(body1);
				//angle=odeJointGetHingeAngle(joint2);
		//	printf("AFTER Posx %1.3f,posz = %f  avel %1.3f,\n",pos[0],pos[2],aVel);


				const dReal *pos = odeBodyGetPosition(body1);
				//float angle=odeJointGetHingeAngle(joint2);
				//float angle1=odeJointGetHingeAngle(joint1);
			float cost=squared((pos[0])*40.0f)+squared((pos[1])*10.0f); //+squared(control* 0.5f)+ squared(control1*0.5f);
			

		/*	if ( pos[0]<0.025 && pos[0]>0.025 )
			{
				
				if( pos[1]>-0.025 && pos[0]<0.025  )

			{
				cost = cost + 10000;
			}
			}
			*/


				//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as in						this simple case.
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
		//printf("Cost %f \n",cost);
		
	
		
		setCurrentOdeContext(0);
		restoreOdeState(0);

		//for(int j=0; j<20; j++)
		//{




	    if (cnt >3)
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

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining

	  	 pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
	   	//angle=odeJointGetHingeAngle(joint2);


        res.commandx = control[0];
        res.commandy = control[1];
		//conv = angle ;

		//last_angle = angle;

		cnt = cnt +1 ;
		last_position = pos[0] ;
		//printf("cnt %d\n",cnt );


	

		ros::Time end = ros::Time::now();
		double dt = (begin - end).toSec();

		//ROS_INFO("dt %f", dt);

		
		printf("FINAL Posx %1.3f,posy = %f   control %f,  control 1 %f \n",pos[0],pos[1],control[0],control[1]);
	  	// printf("angle %1.3f, avel %1.3f, cost=%1.3f\n",angle,aVel,cost);

		
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










