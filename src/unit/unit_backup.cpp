
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
//#include <conio.h>
#include <iostream>
using namespace std;
using namespace AaltoGames;

static const int ANGLE=0;  //state variable index
static const int AVEL=1;	//state variable index



const dReal *pos,*R1,*pos2,*R2,*pos3,*R3;
dMass m1;
dReal DesiredVelocity ;

float conv = 0.0;



int main(int argc, char **argv)
{

	ros::init(argc,argv, "talker");
	ros::NodeHandle n ;


	ros::Publisher chatter_pub = n.advertise<std_msgs::Float32>("from_ode", 1000);
	
	std_msgs::Float32 msg; 

    	ROS_INFO("%f", msg.data);

        ros::spinOnce();

	const int nSamples=32;
	//physics simulation time step
	float timeStep=1.0f/30.0f;


	//Allocate one simulation context for each sample, plus one additional "master" context
	initOde(nSamples+1);
	setCurrentOdeContext(ALLTHREADS);
	odeRandSetSeed(0);
	odeSetContactSoftCFM(0);
	odeWorldSetGravity(0, 0, -9.81f);


		//Build the model:
	//create the stage
	dReal geom = odeCreateBox(.4,.4,0.05);
	dQuaternion q;
	//dQFromAxisAndAngle(q,0,0,1,0);
	//odeGeomSetQuaternion(geom,q);

	//Create a body and attach it to the geom
	int body=odeBodyCreate();
	//odeBodySetDynamic(body);
	odeBodySetMass (body, 0.94f);

	odeGeomSetBody(geom,body);

		
	odeBodySetPosition(body,0,0,2.5);

	int hinge=odeJointCreateHinge();
	//printf("joint ID : %d",hinge);

	odeJointAttach(hinge,0,body);
	odeJointSetHingeAnchor(hinge,0,0,2.55);
	odeJointSetHingeAxis(hinge,0,1,0);
	//odeJointSetHingeParam(hinge,dParamFMax,0); //this hinge has no motor, we'll control it directly using torques
	//odeJointSetHingeParam(hinge,dParamVel,1.0f);



	///////CREATING BALL//////////

	int geom1 = odeCreateSphere(.059f);
	//Create a body and attach it to the geom
	int body1=odeBodyCreate();


	//odeMassSetSphereTotal(body1,.05f,1.0f);
	 //odeMassSetSphere()

	odeBodySetMass (body1, 0.04);
	//odeBodySetDynamic(body1);
	odeGeomSetBody(geom1,body1);

	//Set position of the ball
	odeBodySetPosition(body1,0,0,2.6f);

	setCurrentOdeContext(0);
	saveOdeState(0);



//initialize the optimizer
	ControlPBP pbp;
	int nTimeSteps=15;		//K in the paper, resulting in a 0.5s planning horizon
	//const float PI=3.1416f;
	const int nStateDimensions=2;
	const int nControlDimensions=1;
	float minControl=-100;	//lower sampling bound
	float maxControl=100;		//upper sampling bound
	float controlMean=0;	//we're using torque as the control, makes sense to have zero mean
	//Square root of the diagonal elements of C_u in the paper, i.e., stdev of a Gaussian prior for control.
	//Note that the optimizer interface does not have the C_u as a parameter, and instead uses meand and stdev arrays as parameters.
	//The 3D character tests compute the C_u on the Unity side to reduce the number of effective parameters, and then compute the arrays based on it as described to correspond to the products \sigma_0 C_u etc.
	float C=10;
	float controlStd=1.0f*C;	//sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
	float controlDiffStd=100.0f*C;	//sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
	float controlDiffDiffStd=100.0f*C; //sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
	float mutationScale=0.25f;		//\sigma_m in the paper
	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,&minControl,&maxControl,&controlMean,&controlStd,&controlDiffStd,&controlDiffDiffStd,mutationScale,NULL);

	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,true,0.001f);


//run the algorithm for 90 steps (3 seconds)
	for (int n=0; n<500; n++){
	

	msg.data = conv;
	chatter_pub.publish(msg); 	
	ros::spinOnce();



//init all trajectories to the master state
		for (int i=0; i<nSamples; i++)
		{
			//activate the context for this sample
			setCurrentOdeContext(i+1);
			//load the state from the master context
			restoreOdeState(0);
			//printf("sample no: %d \n" ,i);
			//save the state to the sample context (needed later in resampling)
			saveOdeState(i+1,0);


		}

	


		//signal the start of new C-PBP iteration
		setCurrentOdeContext(0);
		restoreOdeState(0);

		//float angle=odeJointGetHingeAngle(hinge);
		//float aVel=odeJointGetHingeAngleRate(hinge);
		//float stateVector[2]={angle,aVel};

		pos = odeBodyGetPosition(body1);
		float po = pos[0];
		float aVel=odeJointGetHingeAngle(hinge);
		float rate=odeJointGetHingeAngleRate(hinge);

		//printf(" posX x :%f, aVel: %f  \n",pos[0],aVel);
		float stateVector[3]={po,aVel,rate};
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

				pos = odeBodyGetPosition(body1);
				aVel=odeJointGetHingeAngle(hinge);
				rate=odeJointGetHingeAngleRate(hinge);
		                // printf("before Posx %1.3f,posz = %f  avel %1.3f, \n",pos[0],pos[2],(aVel*180/PI));
		                // printf("control %f \n",control);
				//dVector3 torque={0,control,0};
				//odeBodyAddTorque(body,torque);

				dReal Gain = 1;

				float TruePosition = odeJointGetHingeAngle(hinge);
				if (TruePosition <=control-0.2f || TruePosition >=control +0.2f  )
				{
				float degree = (TruePosition*180)/3.1416 ;
				//printf("angle %f\n", degree);
				dReal DesiredPosition =control;
				dReal Error = degree - DesiredPosition;
				//printf("error%f\n", Error);
				dReal DesiredVelocity = Error * Gain;

   				dReal MaxForce = dInfinity;
				odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
				odeJointSetHingeParam(hinge,dParamVel,DesiredVelocity  );

               
				
				stepOde(0);
				}
				pos = odeBodyGetPosition(body1);
				aVel=odeJointGetHingeAngle(hinge);
				//printf("AFTER Posx %1.3f,posz = %f  avel %1.3f,\n",pos[0],pos[2],aVel);


				const dReal *pos = odeBodyGetPosition(body1);
				float po1 = pos[0];
				float aVel=odeJointGetHingeAngle(hinge);
				float rate=odeJointGetHingeAngleRate(hinge);
				float cost=squared((0.1-po1)*70.0f)+ squared(aVel*1.0f) + squared(rate*1.0f);


    				 if (pos[2]<2.3 || pos[2]>3.2)
				{
				cost=cost+1000;
				//printf("I am in\n");
				}
				float avel =abs( aVel*180/PI);
				if (avel >24)
				{

				cost = cost +1000;
				//printf("I am into degree cost:%f \n",cost);
				}


				//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as 					in this simple case.
				//instead, one may use arbitrary state features
				
				float stateVector[2]={po1,aVel};

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

		
		pos = odeBodyGetPosition(body1);
		aVel=odeJointGetHingeAngle(hinge);
		rate=odeJointGetHingeAngleRate(hinge);
		//printf("best control:%f \n ", control);


	

		dReal Gain = 1;
		float TruePosition = odeJointGetHingeAngle(hinge);
		if (TruePosition <=control-0.2f || TruePosition >=control +0.2f  )
		{
		float degree = (TruePosition*180)/3.1416 ;
		//printf("angle %f\n", degree);
		dReal DesiredPosition =control;
		dReal Error = degree - DesiredPosition;
		//printf("error%f\n", Error);
		dReal DesiredVelocity = Error * Gain;

    		dReal MaxForce = dInfinity;
		odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
		odeJointSetHingeParam(hinge,dParamVel,DesiredVelocity);

		stepOde(0);
		}
	


		//save the resulting state as the starting state for the next iteration
		saveOdeState(0);

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining

		pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
	   	aVel=odeJointGetHingeAngle(hinge);
	   	rate=odeJointGetHingeAngleRate(hinge);

       
		conv = aVel ;
		
		printf("FINAL Posx %1.3f,posz = %f  angle %1.3f, cost=%1.3f \n",pos[0],pos[2],aVel*180/3.1416,cost);
	  	// printf("angle %1.3f, avel %1.3f, cost=%1.3f\n",angle,aVel,cost);

		/*
		int i = 0;
		loop : cin >> i;
		if ( i != 1)
		goto loop;
		*/
	
}
		printf("Done, press enter.");
		getchar();

		return 0;
	}











