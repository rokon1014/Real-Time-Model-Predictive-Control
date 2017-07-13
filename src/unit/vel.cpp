
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
#include <unit/from_robot.h>
using namespace std;
using namespace AaltoGames;

static const int ANGLE=0;  //state variable index
static const int AVEL=1;	//state variable index



const dReal *pos1,*R1,*pos2,*R2,*pos3,*R3; 

float last_position =0.1, current_position=0.1, ball_vel;
/*
void start()
{
  static float xyz[3] = {0.0,-4.0,4.0};
  static float hpr[3] = {110.0,-10.0,0.0};
  dsSetViewpoint (xyz,hpr);
}
void  prepDrawStuff() {
  fn.version = DS_VERSION;
  fn.start   = &start;
  fn.step    = &stepOde;
  fn.command = NULL;
  fn.stop    = NULL;
  fn.path_to_textures = "../../drawstuff/textures";

  
}

*/




int main(int argc, char **argv)
{
	//prepDrawStuff();
	//we're using 32 samples, i.e., forward-simulated trajectories per animation frame. This corresponds to the N in the paper.
	const int nSamples=80;
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
	
	//dQFromAxisAndAngle(q,0,0,1,0);
	//odeGeomSetQuaternion(geom,q);

	//Create a body and attach it to the geom
	int body=odeBodyCreate();

	odeMassSetBoxTotal(body, 0.94f, 0.4, 0.4 , 0.05) ;
	//odeBodySetMass (body, 0.94f);
	odeBodySetPosition(body,0,0,2.5);
	//odeBodySetDynamic(body);

	odeGeomSetBody(geom,body);

		//Shift the capsule so that one end is at origin
	

	//Create a hinge joint between the world and the capsule, with anchor at origin and z axis as the rotation axis
	int hinge=odeJointCreateHinge();
	//printf("joint ID : %d",hinge);

	odeJointAttach(hinge,body,0);
	odeJointSetHingeAnchor(hinge,0,0,2.55);
	odeJointSetHingeAxis(hinge,0,1,0);
	//odeJointSetHingeParam(hinge,dParamFMax,0); //this hinge has no motor, we'll control it directly using torques
	//odeJointSetHingeParam(hinge,dParamVel,1.0f);



	///////CREATING BALL//////////

	int geom1 = odeCreateSphere(.03f);
	//Create a body and attach it to the geom
	int body1=odeBodyCreate();

	odeBodySetMass (body1, 5);

	// odeMassSetSphereTotal(body1,0.001,0.02);
	odeBodySetPosition(body1,0.0554,0,2.554f);


	//odeMassSetSphereTotal(body1,.05f,1.0f);
	 //odeMassSetSphere()
	//odeBodySetMass (body1, 0.04);
	odeGeomSetBody(geom1,body1);
	
	//Set position of the ball


setCurrentOdeContext(0);	
saveOdeState(0);



//initialize the optimizer
	ControlPBP pbp;
	int nTimeSteps=30;		//K in the paper, resulting in a 0.5s planning horizon
	//const float PI=3.1416f;	
	const int nStateDimensions=2; 
	const int nControlDimensions=1;
	float minControl=-2;	//lower sampling bound
	float maxControl=2;		//upper sampling bound
	float controlMean=0;	//we're using torque as the control, makes sense to have zero mean
	//Square root of the diagonal elements of C_u in the paper, i.e., stdev of a Gaussian prior for control.
	//Note that the optimizer interface does not have the C_u as a parameter, and instead uses meand and stdev arrays as parameters. 
	//The 3D character tests compute the C_u on the Unity side to reduce the number of effective parameters, and then compute the arrays based on it as described to correspond to the products \sigma_0 C_u etc.
	float C=10;	
	float controlStd=0.9f*C;	//sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
	float controlDiffStd=100.0f*C;	//sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
	float controlDiffDiffStd=1000.0f*C; //sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
	float mutationScale=0.25f;		//\sigma_m in the paper
	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,&minControl,&maxControl,&controlMean,&controlStd,&controlDiffStd,&controlDiffDiffStd,mutationScale,NULL);

	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,true,0.001f);  


//run the algorithm for 90 steps (3 seconds)
	for (int n=0; n<5000; n++){
		printf("%d\n",n);
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

		for (int i=0; i<nSamples; i++)
		{
			setCurrentOdeContext(i);
			const dReal *pos = odeBodyGetPosition(body1);
	      //  printf(" position x :%f y:%f z:%f NUM: %d  \n",pos[0],pos[1],pos[2],i);
		}
		
		
		//signal the start of new C-PBP iteration
		setCurrentOdeContext(0); 
		restoreOdeState(0); 

		//float angle=odeJointGetHingeAngle(hinge);
		//float aVel=odeJointGetHingeAngleRate(hinge);
		//float stateVector[2]={angle,aVel};

		const dReal *pos = odeBodyGetPosition(body1);
		float angle=odeJointGetHingeAngle(hinge);

		//printf(" posX x :%f, aVel: %f  \n",pos[0],aVel);
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
    
				
			//	float TruePosition = odeJointGetHingeAngle(hinge);


 
                dReal MaxForce = dInfinity;
	            odeJointSetHingeParam(hinge,dParamFMax,dInfinity); 
	            odeJointSetHingeParam(hinge,dParamVel,control  );

				stepOde(0);


			
                

				const dReal *pos = odeBodyGetPosition(body1);
				float angle=odeJointGetHingeAngle(hinge);


				current_position = pos[0] ;
				ball_vel = (current_position - last_position)/timeStep ;
				//printf("current ball %f\n", current_position );
				//printf("ball vel %f'\n", ball_vel );


				float cost=squared((pos[0])*1000.0f) +squared(control * 0.6)+ squared(ball_vel*0.3)  + squared(angle*0.3f) ; //+squared(control * 5.2);
			







		

			///////////////////  need to remodel .//////////
    /* if (pos[2]<2.3 || pos[2]>3.2)
		{
				cost=cost+1000;
				//printf("I am in\n");
			}*/
			float angle1 =abs( angle*180/PI);
			if (angle1 >24)
			{
			
				cost = cost +1000;
				//printf("I am into degree cost:%f \n",cost);
			}
				
			//////////////////

				//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as in						this simple case.
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
		//printf("best control:%f \n ", control );
		
		
	
 
    dReal MaxForce = dInfinity;
	odeJointSetHingeParam(hinge,dParamFMax,dInfinity); 
	odeJointSetHingeParam(hinge,dParamVel,control);

		stepOde(0);

	


			//save the resulting state as the starting state for the next iteration
		saveOdeState(0);	

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining

		pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
	   angle=odeJointGetHingeAngle(hinge);
	   
	   float ma =odeBodyGetMass(body1) ;
	   printf("mass %f\n",ma );
	   
	   
	   
	   



	   last_position = pos[0] ;

	  // printf("//////////////// last pos %f //////////////////////\n",last_position );
		printf("FINAL Posx %1.3f,posz = %f  angle %1.3f, cost=%1.3f, control = %f \n",pos[0],pos[2],angle,cost,control);
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
	return 0 ;
	}
	
	
	

	


	
	
			


