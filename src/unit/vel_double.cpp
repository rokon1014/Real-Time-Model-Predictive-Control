
//#include <drawstuff/drawstuff.h>
#include "ros/ros.h"
#include	"std_msgs/String.h"
#include	"std_msgs/Float32.h"
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
int body, hinge, hinge1,hinge2, body1, body_pole1,body_pole2,body_pole3,body_pole4,body2,body_obs;
int geom,geom1,geom_pole1,geom_pole2,geom_pole3,geom_pole4,geom2,geom_obs;
int joint,joint1,joint2,joint3,joint4;




int main(int argc, char **argv)
{
	const int nSamples=70;
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
	geom = odeCreateBox(.1,.1,0.05);
    body=odeBodyCreate();
    odeMassSetBoxTotal(body, 0.01f, 0.4, 0.20 , 0.05) ;
    odeBodySetPosition(body,0,0,1.5);
	odeGeomSetBody(geom,body);

	joint1= odeJointCreateHinge();
	odeJointAttach(joint1,0,body);
	odeJointSetHingeAnchor(joint1,0.2,0,1.525);
	odeJointSetHingeAxis(joint1,1,0,0);




	
	geom_pole2 = odeCreateCapsule(0,0.04,.1);
	body_pole2=odeBodyCreate();
	odeMassSetCapsuleTotal(body_pole2, 0.01f, 0.04, 0.1 ) ;
    odeBodySetPosition(body_pole2,0,0,1.575);
	odeGeomSetBody(geom_pole2,body_pole2);

	
	joint2=odeJointCreateHinge();
	//joint4 =odeJointCreateFixed();



	odeJointAttach(joint2,body,body_pole2);
	//odeJointAttach(joint4,body,body_pole2);
	odeJointSetHingeAnchor(joint2,0,0,1.525);
	odeJointSetHingeAxis(joint2,0,1,0);
	



	geom2 = odeCreateBox(.4,.4,0.05);
    body2=odeBodyCreate();
    odeMassSetBoxTotal(body2, 0.9f, 0.4, 0.4 , 0.05) ;
    odeBodySetPosition(body2,0,0,1.66);
	odeGeomSetBody(geom2,body2);

	joint3= odeJointCreateFixed();
	odeJointAttach(joint3,body2,body_pole2);
	odeJointSetFixed (joint3);


	///////////obstacle/////////
	 geom_obs = odeCreateSphere(.025f);
	//Create a body and attach it to the geom
	body_obs=odeBodyCreate();


	//odeMassSetSphereTotal(body1,.05f,1.0f);
	 //odeMassSetSphere()


    
    odeMassSetSphereTotal(body_obs,0.02,0.02);
    odeBodySetPosition(body_obs,0.0,0.0,1.7f);
	//odeBodySetMass (body1, 0.04);
	odeGeomSetBody(geom_obs,body_obs);
	joint4= odeJointCreateFixed();
	odeJointAttach(joint4,body2,body_obs);
	odeJointSetFixed (joint4);
	////////////////////

	///////CREATING BALL//////////

	 geom1 = odeCreateSphere(.03f);
	//Create a body and attach it to the geom
	body1=odeBodyCreate();


	//odeMassSetSphereTotal(body1,.05f,1.0f);
	 //odeMassSetSphere()


    
    odeMassSetSphereTotal(body1,0.04,0.03);
    odeBodySetPosition(body1,0.1,0.0,1.7f);
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
	const int nControlDimensions=2;
	float minControl=-2;	//lower sampling bound
	float maxControl=2;		//upper sampling bound
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
	for (int n=0; n<5000; n++)
	{
		printf("sss %d\n",n);

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
			

		setCurrentOdeContext(0); 
		restoreOdeState(0); 
		const dReal *pos = odeBodyGetPosition(body1);
		float aVel=odeJointGetHingeAngle(joint2);

		//printf(" posX x :%f, aVel: %f  \n",pos[0],aVel);
		float stateVector[2]={pos[0],pos[1]};
		pbp.startIteration(true,stateVector);



				for (int k=0; k<nTimeSteps; k++)

			{
				pbp.startPlanningStep(k);
				

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

						pos = odeBodyGetPosition(body1);
						aVel=odeJointGetHingeAngle(joint2);

						dReal MaxForce = dInfinity;
						odeJointSetHingeParam(joint2,dParamFMax,dInfinity); 
						odeJointSetHingeParam(joint2,dParamVel,control  );

						odeJointSetHingeParam(joint1,dParamFMax,dInfinity); 
						odeJointSetHingeParam(joint1,dParamVel,control1  );
						stepOde(1);

						pos = odeBodyGetPosition(body1);
						aVel=odeJointGetHingeAngle(joint2);
						const dReal *pos = odeBodyGetPosition(body1);
						float aVel=odeJointGetHingeAngle(joint2);
						float cost=squared((-.05-pos[0])*100.0f)+squared((pos[1])*0.5f);//+ squared(aVel*10.5f);+ squared(control* 1000.0f)+ squared(control1*900.0f);

						float stateVector[2]={pos[0],pos[1]};
						float cont[2]= {control,control1};
						pbp.updateResults(i,cont,stateVector,cost);
				


					}

					
						for (int i=0; i<nSamples; i++)
					{
						saveOdeState(i+1,i+1);
					}


				pbp.endPlanningStep(k);

			}


		pbp.endIteration();
		float control[2];
		pbp.getBestControl(0,control);
		float cost=(float)pbp.getSquaredCost();
		printf("Cost %f \n",cost);

		setCurrentOdeContext(0);
		restoreOdeState(0);

		pos = odeBodyGetPosition(body1);
		aVel=odeJointGetHingeAngle(joint2);
		printf("best control 0 :%f control 1 %f \n ", control[0], control[1]);

		 dReal MaxForce = dInfinity;
		odeJointSetHingeParam(joint2,dParamFMax,dInfinity); 
		odeJointSetHingeParam(joint2,dParamVel,control[0]);

		odeJointSetHingeParam(joint1,dParamFMax,dInfinity); 
		odeJointSetHingeParam(joint1,dParamVel,control[1]);
		

		stepOde(1);	
		saveOdeState(0);	

		pos = odeBodyGetPosition(body1);
		aVel=odeJointGetHingeAngle(joint2);
		//printf("FINAL Posx %1.3f,posy = %f  avel %1.3f, cost=%1.3f \n",pos[0],pos[1],aVel*180/3.1416,cost);









	 }	 
			  
	printf("Done, press enter.");
	getchar();
	return 0 ;
}
	
	
	

	


	
	
			


