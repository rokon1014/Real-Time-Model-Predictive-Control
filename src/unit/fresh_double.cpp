
#include <drawstuff/drawstuff.h>
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
dsFunctions fn;
using namespace AaltoGames;


float h_floor_table = 0.65f ;
float h_base = 0.32f ;
float h_first= 0.60f ;
float h_main = 0.12f ;
float h_sphere= 0.08f;
float h_support = 0.14f ;
float stage_dim[3]= {0.4f, 0.4f, 0.05f };
const int nSamples=80;
//physics simulation time step
float timeStep=1.0f/100.0f;
ControlPBP pbp;
int nTimeSteps=30;
const int nStateDimensions=2;
const int nControlDimensions=2;
float minControl[2]={-2.5,-2.5}; //lower sampling bound
float maxControl[2]={2.5,2.5}; //upper sampling bound
float controlMean[2]={0,0}; //we're using torque as the control, makes sense to have zero mean
//Square root of the diagonal elements of C_u in the paper, i.e., stdev of a Gaussian prior for control.
//Note that the optimizer interface does not have the C_u as a parameter, and instead uses meand and stdev arrays as parameters.
//The 3D character tests compute the C_u on the Unity side to reduce the number of effective parameters, and then compute the arrays based on it as described to correspond to the products \sigma_0 C_u etc.
float C=10;
float controlStd[2]={0.8f*C,0.8f*C}; //sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
float controlDiffStd[2]={100.0f*C,100.0f*C}; //sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
float controlDiffDiffStd[2]={1000.0f*C,1000.0f*C}; //sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
float mutationScale=0.25f;
bool tick=true;




class objects
{
	public:
	dReal body;
	dReal geom;
	dReal joint;
	dReal joint_extra;
	float radius;
	float length;
};



objects stage, support, LinkBall, mainLink,firstLink,base, ball, obs;
void robot(int com)
{
	
	setCurrentOdeContext(0);
	restoreOdeState(0);
	saveOdeState(0);

	//init all trajectories to the master state
	for (int i=0; i<nSamples; i++)
	{
		//activate the context for this sample
		setCurrentOdeContext(i+1);
		//load the state from the master context
		restoreOdeState(0);
		saveOdeState(i+1,0);
	}

	setCurrentOdeContext(0);
	restoreOdeState(0);
	const dReal *pos = odeBodyGetPosition(ball.body);
	float angle=odeJointGetHingeAngle(mainLink.joint);
	float angle_second=odeJointGetHingeAngle(LinkBall.joint);
	//printf("position befor simulate forward %f \n", pos[1]);

	float stateVector[4]={pos[0],pos[1], angle, angle_second};
	pbp.startIteration(true,stateVector);

	//simulate forward
	for (int k=0; k<nTimeSteps; k++)
	{
	//signal the start of a planning step
		pbp.startPlanningStep(k);
		for (int i=0; i<nSamples; i++)
		{
			//get control from C-PBP
			float control[2];
			pbp.getControl(i,control);
			//get the mapping from this to previous state (affected by resampling operations)
			int previousStateIdx=pbp.getPreviousSampleIdx(i);
			//simulate to get next state.
			setCurrentOdeContext(i+1);
			//continue the a trajectory (selected in the resampling operation inside ControlPBP)
			restoreOdeState(previousStateIdx+1);
			const dReal *pos = odeBodyGetPosition(ball.body);
			float angle=odeJointGetHingeAngle(mainLink.joint);
			
			dReal Gain = 1;
			dReal MaxForce = dInfinity;
			//dReal Gain = 10;
			//dReal MaxForce = 100;
			//dReal TruePosition = dJointGetHingeAngle(joint);
			dReal DesiredPosition = control[0];
			dReal Error = angle - DesiredPosition;
			//printf("error %f \n", Error);
			dReal DesiredVelocity = -Error * Gain;
			//printf("des vel %f \n", DesiredVelocity);
			odeJointSetHingeParam(mainLink.joint, dParamFMax, MaxForce);
			odeJointSetHingeParam(mainLink.joint, dParamVel,control[0] );
			odeJointSetHingeParam(LinkBall.joint, dParamFMax, MaxForce);
			odeJointSetHingeParam(LinkBall.joint, dParamVel,control[1] );
			//odeJointSetHingeParam(LinkBall.joint,dParamFMax,dInfinity);
			//odeJointSetHingeParam(LinkBall.joint,dParamVel,control );
			stepOde(1);
			pos = odeBodyGetPosition(ball.body);
			angle=odeJointGetHingeAngle(mainLink.joint);
			float angle_second = odeJointGetHingeAngle(LinkBall.joint);
			
			float cost=squared((-0.08-pos[0])*12.0f)+squared((0.00-pos[1])*12.0f);//+squared(angle*5)+squared(angle_second*5);//+ squared(angle*30.0f) ;
			
		
		//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as in this simple case.
		//instead, one may use arbitrary state features
			float stateVector[4]={pos[0],pos[1], angle, angle_second};
			pbp.updateResults(i,control,stateVector,cost);
		}
		/*
		int j = 0;
		loop : std::cin >> j;
		if ( j != 1)
		goto loop;
		*/

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
	dReal MaxForce = dInfinity;
	dReal Gain = 1 ;
	dReal DesiredPosition = control[0];
	dReal Error = angle - DesiredPosition;
	dReal DesiredVelocity = -Error * Gain;
	odeJointSetHingeParam(mainLink.joint,dParamFMax,dInfinity);
	odeJointSetHingeParam(mainLink.joint,dParamVel,control[0]);
	odeJointSetHingeParam(LinkBall.joint,dParamFMax,dInfinity);
	odeJointSetHingeParam(LinkBall.joint,dParamVel,control[1]);
	stepOde(1);
	saveOdeState(0);
	const dReal *pos1 = odeBodyGetPosition(ball.body);
	float angle1=odeJointGetHingeAngle(mainLink.joint);
	
	//printf("rel_vec %f and stage %f\n", re_vec, stage_pos[0]);
	printf("FINAL Posx %1.3f,posz = %f angle %1.3f, cost=%1.3f, control %f \n",pos1[0],pos1[1],angle1*180/3.1416,cost,control[0]);
	/*
	int j = 0;
	loop : std::cin >> j;
	if ( j != 1)
	goto loop;
	*/
}

/*

void start()
{
	static float xyz[3] = {0.0,-1.6,3.0};
	static float hpr[3] = {100.0,-30.0,10.0};
	dsSetViewpoint (xyz,hpr);
}

void prepDrawStuff()
{
	fn.version = DS_VERSION;
	fn.start = &start;
	fn.step = &robot;
	fn.command = NULL;
	fn.stop = NULL;
	fn.path_to_textures = "/home/rokon/ode-0.12/drawstuff/textures";
}
*/

int main(int argc, char **argv)
{
	//prepDrawStuff();
	initOde(nSamples+1);
	setCurrentOdeContext(ALLTHREADS);
	odeRandSetSeed(0);
	odeSetContactSoftCFM(0);
	odeWorldSetGravity(0, 0, -9.81f);

	//creating stage
	stage.body = odeBodyCreate();
	stage.geom = odeCreateBox(0.4f, 0.4f, 0.05f);
	odeMassSetBoxTotal(stage.body, 0.94, 0.4f,0.4f, 0.05f);
	odeBodySetPosition(stage.body,0,0.0,h_floor_table+h_base+h_sphere+h_support+stage_dim[2]/2);
	odeGeomSetBody(stage.geom,stage.body);
	printf(" Stage body id %f, geom id %f \n", stage.body, stage.geom);
	const dReal *pos;
	pos= odeBodyGetPosition(stage.body);
	printf("stage position x%f y%f z%f \n",pos[0],pos[1], pos[2]);

	
	//creating support //
	//odeMassSetCapsuleTotal(int bodyId, float total_mass, float radius, float length)
	support.radius = 0.05f ;
	support.length = 0.2f;
	support.body = odeBodyCreate();
	support.geom = odeCreateCapsule(0, support.radius, support.length-support.radius);
	odeMassSetCapsuleTotal(support.body, 0.5f, support.radius, support.length-support.radius);
	odeBodySetPosition(support.body,0.0f,0.0f,h_floor_table+h_base+h_sphere+(h_support/2)-stage_dim[2]/2);
	odeGeomSetBody(support.geom,support.body);
	printf("Support capsule body id %f, geom id %f \n", support.body, support.geom);
	pos= odeBodyGetPosition(support.body);
	printf("stage position %f \n", pos[2]);


	//creating ball link
	LinkBall.radius = 0.08f ;
	LinkBall.body = odeBodyCreate();
	LinkBall.geom = odeCreateSphere( LinkBall.radius);
	odeMassSetSphereTotal(LinkBall.body, 0.5,LinkBall.radius);
	odeBodySetPosition(LinkBall.body,0,0,h_floor_table+h_base-support.radius);
	odeGeomSetBody(LinkBall.geom,LinkBall.body);
	printf("Ball link body id %f, geom id %f \n", LinkBall.body, LinkBall.geom);


	/*
	//creating main link
	mainLink.radius= 0.05f;
	mainLink.length= 0.12f;
	mainLink.body = odeBodyCreate();
	mainLink.geom = odeCreateCapsule(0, mainLink.radius, mainLink.length-mainLink.radius);
	odeMassSetCapsuleTotal(mainLink.body, 0.5,mainLink.radius, mainLink.length-mainLink.length);
	odeBodySetPosition(mainLink.body,(mainLink.length+mainLink.radius),0, h_floor_table+h_base-mainLink.radius);
	odeGeomSetBody(mainLink.geom,mainLink.body);
	printf("capsule main link link body id %f, geom id %f \n", mainLink.body, mainLink.geom);
	dQuaternion q1;
	dQFromAxisAndAngle (q1,0,1,0,1.57) ; //dQFromAxisAndAngle (dQuaternion q, dReal ax, dReal ay, dReal az, dReal angle);
	odeGeomSetQuaternion(mainLink.geom,q1);
	pos= odeBodyGetPosition(mainLink.body);
	printf("main link position x %f y %f z%f \n",pos[0],pos[1], pos[2]);
	*/


	//creating main link
	mainLink.radius= 0.05f;
	mainLink.length= 0.12f;
	mainLink.body = odeBodyCreate();
	mainLink.geom = odeCreateBox(0.12, 0.05,0.05);
	odeMassSetBoxTotal(mainLink.body, 0.5,0.12,0.05,0.05);
	odeBodySetPosition(mainLink.body,(mainLink.length+mainLink.radius),0, h_floor_table+h_base-mainLink.radius);
	odeGeomSetBody(mainLink.geom,mainLink.body);
	printf("capsule main link link body id %f, geom id %f \n", mainLink.body, mainLink.geom);


	



	//Creating Ball
	ball.radius = 0.03f ;
	ball.body = odeBodyCreate();
	ball.geom = odeCreateSphere( ball.radius);
	odeMassSetSphereTotal(ball.body, 0.04,ball.radius);
	odeBodySetPosition(ball.body,0.08,0.00,h_floor_table+h_base+h_sphere+h_support+2*stage_dim[2]);
	odeGeomSetBody(ball.geom,ball.body);
	printf("ball body id %f, geom id %f \n", ball.body, ball.geom);


	/*
	//creating Obstacle
	obs.radius = 0.03f;
	obs.body = odeBodyCreate();
	obs.geom = odeCreateSphere( obs.radius);
	odeMassSetSphereTotal(obs.body, 0.005,obs.radius);
	odeBodySetPosition(obs.body,0,0.0,h_floor_table+h_base+h_sphere+h_support+stage_dim[2]+obs.radius);
	odeGeomSetBody(obs.geom,obs.body);
	printf("capsule obstacle body id %f, geom id %f \n", obs.body, obs.geom);
	*/








	// Main link and World
	mainLink.joint =odeJointCreateHinge();
	odeJointAttach(mainLink.joint,mainLink.body,0);
	//odeJointSetHingeAnchor(capsule2.joint[0],-0.5,0,0.60);
	odeJointSetHingeAxis(mainLink.joint,1,0,0);
	//odeJointSetHingeParam(mainLink.joint,dParamHiStop,2.5);
	//odeJointSetHingeParam(mainLink.joint,dParamLoStop,-2.5);


	//Ball link and Main link
	LinkBall.joint =odeJointCreateHinge();
	odeJointAttach(LinkBall.joint,LinkBall.body,mainLink.body);
	//odeJointSetHingeAnchor(capsule3.joint[1],-1.1,0.5,0.60);
	odeJointSetHingeAxis(LinkBall.joint,0,1,0);
	


	// Support and Ball link
	support.joint =odeJointCreateFixed();
	odeJointAttach(support.joint,support.body,LinkBall.body);
	odeJointSetFixed(support.joint);


	//Stage and support link
	stage.joint =odeJointCreateFixed();
	odeJointAttach(stage.joint,stage.body,support.body);
	odeJointSetFixed(stage.joint);


	


	setCurrentOdeContext(0);
	saveOdeState(0);
	//initialize the optimizer
	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,minControl,maxControl,controlMean,controlStd,controlDiffStd,controlDiffDiffStd,mutationScale,NULL);
	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,true,0.001f);

	while(1)
	{
	robot(1);
	}
	//dsSimulationLoop (argc,argv,900,600,&fn);
}

