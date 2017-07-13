
#include <math.h>
#include "MathUtils.h"
#include "ControlPBP.h"
#include "UnityOde.h"
#include <ode/ode.h>
#include <stdio.h>
#include <sstream>
#include <cstdio>
#include <iostream>
using namespace std;
using namespace AaltoGames;

static const int ANGLE=0;  //state variable index
static const int AVEL=1;	//state variable index



const dReal *pos1,*R1,*pos2,*R2,*pos3,*R3; 
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
	dReal geom = odeCreateBox(.6,.6,0.05);
	dQuaternion q;
	//dQFromAxisAndAngle(q,0,0,1,0);
	//odeGeomSetQuaternion(geom,q);

	//Create a body and attach it to the geom
	int body=odeBodyCreate();
	odeBodySetDynamic(body);

	odeGeomSetBody(geom,body);

		//Shift the capsule so that one end is at origin
	odeBodySetPosition(body,0,0,2.5);

	//Create a hinge joint between the world and the capsule, with anchor at origin and z axis as the rotation axis
	int hinge=odeJointCreateHinge();
	int hinge1=odeJointCreateHinge();
	//printf("joint ID : %d",hinge);

	odeJointAttach(hinge,0,body);
	odeJointSetHingeAnchor(hinge,0,0,2.55);
	odeJointSetHingeAxis(hinge,0,1,0);


	odeJointAttach(hinge1,0,body);
	odeJointSetHingeAnchor(hinge1,0,0,2.55);
	odeJointSetHingeAxis(hinge1,1,0,0);
	//odeJointSetHingeParam(hinge,dParamFMax,0); //this hinge has no motor, we'll control it directly using torques
	//odeJointSetHingeParam(hinge,dParamVel,1.0f);



	///////CREATING BALL//////////

	int geom1 = odeCreateSphere(.03f);
	//Create a body and attach it to the geom
	int body1=odeBodyCreate();


	//odeMassSetSphereTotal(body1,.05f,1.0f);
	 //odeMassSetSphere()
	odeBodySetDynamic(body1);
	odeGeomSetBody(geom1,body1);
	
	//Set position of the ball
	odeBodySetPosition(body1,0.1,0.1,2.6f);

setCurrentOdeContext(0);	
saveOdeState(0);



//initialize the optimizer
	ControlPBP pbp;
	int nTimeSteps=30;		//K in the paper, resulting in a 0.5s planning horizon
	//const float PI=3.1416f;	
	const int nStateDimensions=2; 
	const int nControlDimensions=2;
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

		//printf("1\n");
//init all trajectories to the master state
		for (int i=0; i<nSamples; i++)
		{
			//printf("2\n" );
			//activate the context for this sample
			setCurrentOdeContext(i+1); 
			//load the state from the master context
			restoreOdeState(0);
			//printf("sample no: %d \n" ,i);
			//save the state to the sample context (needed later in resampling)
			saveOdeState(i+1,0); 
			//printf("3\n" );


		}

		for (int i=0; i<nSamples; i++)
		{
			//printf("4\n" );
			setCurrentOdeContext(i);
			const dReal *pos = odeBodyGetPosition(body1);
	      //  printf(" position x :%f y:%f z:%f NUM: %d  \n",pos[0],pos[1],pos[2],i);
		}
		
		
		//printf("5\n" );
		//signal the start of new C-PBP iteration
		setCurrentOdeContext(0); 
		restoreOdeState(0); 

		//printf("6\n" );

		//float angle=odeJointGetHingeAngle(hinge);
		//float aVel=odeJointGetHingeAngleRate(hinge);
		//float stateVector[2]={angle,aVel};

		const dReal *pos = odeBodyGetPosition(body1);
		float aVel=odeJointGetHingeAngle(hinge);
		//printf("7\n" );

		//printf(" posX x :%f, aVel: %f  \n",pos[0],aVel);
		float stateVector[2]={pos[0],pos[1]};
		pbp.startIteration(true,stateVector);
		//printf("8\n" );

		

		//simulate forward 
		for (int k=0; k<nTimeSteps; k++)

		{
			//printf("9\n" );


			//signal the start of a planning step
			pbp.startPlanningStep(k);
						//NOTE: for multithreaded operation, one would typically run each iteration of the following loop in a separate thread. 
			//The getControl(), getPreviousSampleIdx(), and updateResults() methods of the optimizer are thread-safe.
			//The physics simulation is also thread-safe as we have full copies of the simulated system for each thread/trajectory

			for (int i=0; i<nSamples; i++)
			{
				//printf("10\n" );
				//get control from C-PBP
				float control[2];
				pbp.getControl(i,&control[0]);
				pbp.getControl(i,&control[1]);
				//printf("11\n" );
				//printf("k=%i",k);
				//printf("i=%d \n",i);
				//printf("control: %f  control 1 %f\n",control,control1);


				//get the mapping from this to previous state (affected by resampling operations)
				int previousStateIdx=pbp.getPreviousSampleIdx(i);
				//simulate to get next state.
				setCurrentOdeContext(i+1);
				//printf("previous state id: %d \n",previousStateIdx);
				restoreOdeState(previousStateIdx+1); //continue the a trajectory (selected in the resampling operation inside ControlPBP)

				pos = odeBodyGetPosition(body1);
				aVel=odeJointGetHingeAngle(hinge);
		// printf("before Posx %1.3f,posz = %f  avel %1.3f, \n",pos[0],pos[2],(aVel*180/PI));
		 // printf("control %f \n",control);
				//dVector3 torque={0,control,0};
				//odeBodyAddTorque(body,torque);

				//printf("12\n" );
 
  			  dReal MaxForce = dInfinity;
			  odeJointSetHingeParam(hinge,dParamFMax,dInfinity); 
			  odeJointSetHingeParam(hinge,dParamVel,control[0]  );

			  odeJointSetHingeParam(hinge1,dParamFMax,dInfinity); 
			  odeJointSetHingeParam(hinge1,dParamVel,control[1]  );

				stepOde(1);

				pos = odeBodyGetPosition(body1);
				aVel=odeJointGetHingeAngle(hinge);
		//	printf("AFTER Posx %1.3f,posz = %f  avel %1.3f,\n",pos[0],pos[2],aVel);


				const dReal *pos = odeBodyGetPosition(body1);
				float aVel=odeJointGetHingeAngle(hinge);
			float cost=squared((pos[0])*100.0f)+squared((pos[1])*100.0f)+ squared(aVel*1.5f);+ squared(control[0]* 900.0f)+ squared(control[1]*900.0f);

			





/*

			///////////////////  need to remodel .//////////
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
				
			//////////////////
			*/
				//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as in						this simple case.
				//instead, one may use arbitrary state features
				float stateVector[2]={pos[0],pos[1]};
				//float cont[2]= {control,control1};

				//float stateVector[2]={angle,aVel};
				pbp.updateResults(i,control,stateVector,cost);
				//printf("13\n" );

				//printf("cost in the loop end: %f posz: %f \n",cost,pos[2]);
			}
//save all states, will be used at next step (the restoreOdeState() call above)
	for (int i=0; i<nSamples; i++)
			{
				saveOdeState(i+1,i+1);
			}
			//printf("14\n" );

			//signal the end of the planning step. this normalizes the state costs etc. for the next step
			pbp.endPlanningStep(k);
		}
			//signal the end of an iteration. this also executes the backwards smoothing pass
		pbp.endIteration();
		
		//deploy the best control found using the master context
		float control[2];
		pbp.getBestControl(0,control);
		
		float cost=(float)pbp.getSquaredCost();
		printf("Cost %f \n",cost);
		
	
		
		setCurrentOdeContext(0);
		restoreOdeState(0);

		//for(int j=0; j<20; j++)
		//{
			pos = odeBodyGetPosition(body1);
		aVel=odeJointGetHingeAngle(hinge);
		//printf("before Posx %1.3f,posz = %f  avel %1.3f, cost=%1.3f\n",pos[0],pos[2],(aVel*180/PI),cost);
		printf("best control 0 :%f control 1 %f \n ", control[0], control[1]);
		
		
		//dVector3 torque={0,control,0};
		//odeBodyAddTorque(body,torque);



 
    dReal MaxForce = dInfinity;
	odeJointSetHingeParam(hinge,dParamFMax,dInfinity); 
	odeJointSetHingeParam(hinge,dParamVel,control[0]);

	odeJointSetHingeParam(hinge1,dParamFMax,dInfinity); 
	odeJointSetHingeParam(hinge1,dParamVel,control[1]);

		stepOde(1);
		//printf(" after Posx %1.3f,posz = %f  avel %1.3f, cost=%1.3f\n",pos[0],pos[2],aVel,cost);


			//save the resulting state as the starting state for the next iteration
		saveOdeState(0);	

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining

		pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
	   aVel=odeJointGetHingeAngle(hinge);
		printf("FINAL Posx %1.3f,posy = %f  avel %1.3f, cost=%1.3f \n",pos[0],pos[1],(aVel*180/PI),cost);
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
	
	
	

	


	
	
			


