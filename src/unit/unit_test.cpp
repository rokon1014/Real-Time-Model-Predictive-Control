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

float last_position =0.0588, current_position=0.0588, ball_vel;




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


	const int nSamples=50;
	//physics simulation time step
	float timeStep=1.0f/30.0f;


    ControlPBP pbp;
	int nTimeSteps=15;		
	const int nStateDimensions=2;
	const int nControlDimensions=1;
	float minControl=-2;	//lower sampling bound
	float maxControl=2;		//upper sampling bound
	float controlMean=0;	//we're using torque as the control, makes sense to have zero mean
	//Square root of the diagonal elements of C_u in the paper, i.e., stdev of a Gaussian prior for control.
	//Note that the optimizer interface does not have the C_u as a parameter, and instead uses meand and stdev arrays as parameters.
	//The 3D character tests compute the C_u on the Unity side to reduce the number of effective parameters, and then compute the arrays based on it as described to correspond to the products \sigma_0 C_u etc.
	float C=10;
	float controlStd=0.8f*C;	//sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
	float controlDiffStd=100.0f*C;	//sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
	float controlDiffDiffStd=1000.0f*C; //sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
	float mutationScale=0.25f;		//\sigma_m in the paper
	
	ofstream myfile;
	ofstream final;
   


bool robot(unit::from_robot::Request &req, unit::from_robot::Response &res)
{
  
ros::Time begin = ros::Time::now();

pos_robot= req.position;
ang_robot = req.angle ;



check = true ;
msg.data = conv;
chatter_pub.publish(msg); 	
	
 	
  
   


setCurrentOdeContext(0);
restoreOdeState(0);
const dReal *pos_first = odeBodyGetPosition(body1);

odeBodySetPosition(body1,pos_robot,pos_first[1],pos_first[2]);
const dReal *pos_second = odeBodyGetPosition(body1);


	
saveOdeState(0);



//init all trajectories to the master state
		for (int i=0; i<nSamples; i++)
		{
			//activate the context for this sample
			setCurrentOdeContext(i+1);
			//load the state from the master context
			restoreOdeState(0);

			const dReal *pos = odeBodyGetPosition(body1);

			


			odeBodySetPosition(body1,pos_robot,pos[1],pos[2]);
			const dReal *pos1 = odeBodyGetPosition(body1);

			//printf(" checking position %f\n", pos1[0] );
			//printf("sample no: %d \n" ,i);
			//save the state to the sample context (needed later in resampling)
			saveOdeState(i+1,0);


		}


		for (int i=0; i<nSamples; i++)
		{
			setCurrentOdeContext(i);
			const dReal *pos = odeBodyGetPosition(body1);
	      //  printf(" position x :%f y:%f z:%f NUM: %d  \n",pos[0],pos[1],pos[2],i);


			
		}
	


		//signal the start of new C-PBP iteration
		setCurrentOdeContext(0);
		restoreOdeState(0);

		const dReal *pos2 = odeBodyGetPosition(body1);

		odeBodySetPosition(body1,pos_robot,pos2[1],pos2[2]);

		//float angle=odeJointGetHingeAngle(hinge);
		//float aVel=odeJointGetHingeAngleRate(hinge);
		//float stateVector[2]={angle,aVel};

		
		const dReal *pos = odeBodyGetPosition(body1);
		float angle=odeJointGetHingeAngle(hinge);

		//printf(" posX x :%f\n",pos[0]);
		float stateVector[2]={pos[0],angle};
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
				float control;
				pbp.getControl(i,&control);
				//printf("k=%i",k);
				//printf("i=%d \n",i);
				//printf("control: %f \n",control);


				//get the mapping from this to previous state (affected by resampling operations)
				int previousStateIdx=pbp.getPreviousSampleIdx(i);
				//simulate to get next state.
				setCurrentOdeContext(i+1);
				//printf("previous state id: %d \n",previousStateIdx);
				restoreOdeState(previousStateIdx+1); //continue the a trajectory (selected in the resampling operation inside ControlPBP)

		

				dReal Gain = 1;

			
   				dReal MaxForce = dInfinity;
				odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
				odeJointSetHingeParam(hinge,dParamVel,control  );

             
				stepOde(0);
				
				
                

				const dReal *pos = odeBodyGetPosition(body1);
				float angle=odeJointGetHingeAngle(hinge);


				current_position = pos[0] ;
				ball_vel = (current_position - last_position); ///timeStep ;

				 change = angle -last_angle ;


				 
			 // float cost=squared((0.03-pos[0])*800.0f) +squared(ball_vel*0.3)+squared(control * 0.5) ; //+ squared(ball_vel*0.2)+ squared(angle*0.3f)  ; // last working

			 float cost=squared((0.01-pos[0])*1050.0f) +squared(ball_vel*0.3)+squared(control * 0.3) ; //+ squared(ball_vel*0.2)+ squared(angle*0.3f)  ;

		

			//myfile  <<"number" <<std::setw(5)  << i <<std::setw(12)<<"control"<<std::setw(15) << control <<std::setw(10)<<"position" <<std::setw(10)<< pos[0] <<std::setw(10) <<"cost"<<std::setw(10)<< cost <<std::setw(15) <<"timestep"<< std::setw(5)<<k <<std::setw(10) <<"org step "<< std::setw(5)<<cnt <<endl;
			/*
					///////////////////  need to remodel .//////////
    			 if (pos[2]<2.3 || pos[2]>3.2)
			{
					cost=cost+1000;
				//printf("I am in\n");
			}
			float angle1 =abs( angle*180/PI);
			if (angle1 >24)
			{
			
				cost = cost +1000;
				//printf("I am into degree cost:%f \n",cost);
			}
				
		*/	//////////////////

				//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as 					in this simple case.
				//instead, one may use arbitrary state features
				float stateVector[2]={pos[0],angle};

				//float stateVector[2]={angle,aVel};
				pbp.updateResults(i,&control,stateVector,cost);

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
		float control;
		pbp.getBestControl(0,&control);

		float cost=(float)pbp.getSquaredCost();
		//printf("Cost %f \n",cost);



		setCurrentOdeContext(0);
		restoreOdeState(0);

		

	    if (cnt >3)
	    {
	    	
    	dReal MaxForce = dInfinity;
		odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
		odeJointSetHingeParam(hinge,dParamVel,control);

		}

		else 
		{
		control = 0;
		odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
		odeJointSetHingeParam(hinge,dParamVel,control);


		}

		stepOde(0);
	


		//save the resulting state as the starting state for the next iteration
		saveOdeState(0);

		
  	
  		//myfile.close();

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining

	   pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
	   angle=odeJointGetHingeAngle(hinge);


       res.command = control;
	   conv = angle ;

	   last_angle = angle;

	   cnt = cnt +1 ;
	   last_position = pos[0] ;
		//printf("cnt %d\n",cnt );
		final << " best control" <<std::setw(12) << control << std::setw(12) <<"best cost" <<std::setw(12)<< std::setw(12)<<cost <<std::setw(12)<< "position"<< std::setw(12)<<last_position <<endl ;
		

	   ros::Time end = ros::Time::now();
	   double dt = (begin - end).toSec();

	   ROS_INFO("dt %f", dt);


		//printf("FINAL Posx %1.3f,posz = %f  angle %1.3f, cost=%1.3f, control %f \n",pos[0],pos[2],angle*180/3.1416,cost,control);
	  	// printf("angle %1.3f, avel %1.3f, cost=%1.3f\n",angle,aVel,cost);


		
		   
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
   final.open ("final.txt");
   

	//Allocate one simulation context for each sample, plus one additional "master" context
	initOde(nSamples+1);
	setCurrentOdeContext(ALLTHREADS);
	odeRandSetSeed(0);
	odeSetContactSoftCFM(0);
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
    odeBodySetPosition(body1,0.0588,0,2.554f);
	//odeBodySetMass (body1, 0.04);
	odeGeomSetBody(geom1,body1);

	//Set position of the ball
	

	setCurrentOdeContext(0);
	saveOdeState(0);





    //initialize the optimizer
	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,&minControl,&maxControl,&controlMean,&controlStd,&controlDiffStd,&controlDiffDiffStd,mutationScale,NULL);

	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,true,0.001f);



	while (ros::ok())
	{

       
      ros::spinOnce();
  
	//printf("spinning");
		

	}
		printf("Done, press enter.");
		getchar();

		return 0;
	}










