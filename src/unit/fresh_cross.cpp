
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
#include <unit/for_double.h>
#include <unit/for_feedback.h>
#include <unit/twoBall.h>
dsFunctions fn;
using namespace AaltoGames;

float pos_robot1x;
float pos_robot1y;


float pos_robot2x;
float pos_robot2y;

float ang_robot_a4;
float ang_robot_a5;


ros::ServiceServer service;
ros::Publisher chatter_pub;
std_msgs::Float32 msg; 



float h_floor_table = 0.65f ;
float h_base = 0.32f ;
float h_first= 0.60f ;
float h_main = 0.12f ;
float h_sphere= 0.08f;
float h_support = 0.14f ;
float stage_dim[3]= {0.46f, 0.4f, 0.052f };
std::ofstream work;



const int nSamples=85; //working 65
//physics simulation time step
float timeStep=1.0f/24.0f;
ControlPBP pbp;
int nTimeSteps=18; // 15 working
const int nStateDimensions=6;
const int nControlDimensions=2;
float minControl[2]={-0.8,-0.8}; //lower sampling bound -0.8 both working
float maxControl[2]={0.8,0.8}; //upper sampling bound 0.8 both working
float controlMean[2]={0,0}; //we're using torque as the control, makes sense to have zero mean
//Square root of the diagonal elements of C_u in the paper, i.e., stdev of a Gaussian prior for control.
//Note that the optimizer interface does not have the C_u as a parameter, and instead uses meand and stdev arrays as parameters.
//The 3D character tests compute the C_u on the Unity side to reduce the number of effective parameters, and then compute the arrays based on it as described to correspond to the products \sigma_0 C_u etc.
float C=0.4;
float controlStd[2]={0.85f*C,0.85f*C}; // 0.65 both working //sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
float controlDiffStd[2]={0.9f*C, 0.9f*C}; // 0.9 both working //sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
float controlDiffDiffStd[2]={20.5f*C,20.5f*C}; //18.5 both working//sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
float mutationScale=0.25f;



bool tick=true;
bool debug = false;
bool final_debug = true;
bool debug_inside =false ;


float starting_pos1X = -0.106; //NEED TO SET IT EVERYTIME
float starting_pos1Y = -0.008;


float starting_pos2X = -0.106; //NEED TO SET IT EVERYTIME
float starting_pos2Y = -0.008;

float last_pos1X= starting_pos1X;
float last_pos1Y= starting_pos1Y; 


float last_pos2X= starting_pos2X;
float last_pos2Y= starting_pos2Y;


float vel_est1x;
float vel_est1y;

float vel_est2x;
float vel_est2y;

float last_vel_est1x=0.0;
float last_vel_est1y=0.0;

float last_vel_est2x=0.0;
float last_vel_est2y=0.0;


float current_pos1X, current_pos1Y,current_pos2X, current_pos2Y;


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

objects stage, support, LinkBall, mainLink,firstLink,base, ball,ball2, obs, obs2;



bool robot(unit::twoBall::Request &req, unit::twoBall::Response &res)
{

	ros::Time begin = ros::Time::now();

	pos_robot1x= req.position1x;
	pos_robot1y= req.position1y;

	pos_robot2x= req.position2x;
	pos_robot2y= req.position2y;
	
	


	ang_robot_a4 = req.angle_a4 ;
	ang_robot_a5 = req.angle_a5 ;


	setCurrentOdeContext(0);
	restoreOdeState(0);

///////////////////FEEDBACKS//////////////////
	//joint angle feedbak
	/*
	float angle_sim_a4=odeJointGetHingeAngle(mainLink.joint);
	float angle_sim_a5=odeJointGetHingeAngle(LinkBall.joint);
	
	if (debug) printf(" simlutor  angle before a4:%f  a5:%f  \n", angle_sim_a4, angle_sim_a5);

	float time_step =24  ;
	dReal MaxForce = dInfinity;

	dReal DesiredPosition_a4 = ang_robot_a4;
	dReal DesiredPosition_a5 = ang_robot_a5;

	dReal Error_a4 = angle_sim_a4 - DesiredPosition_a4;
	dReal Error_a5 = angle_sim_a5 - DesiredPosition_a5;

	dReal DesiredVelocity_a4 = -Error_a4*time_step;
	dReal DesiredVelocity_a5 = -Error_a5*time_step;
		
		
		
	if (debug) printf("robot angle  a4:%f ,  a5: %f \n", ang_robot_a4,ang_robot_a5);
	if (debug) printf("Error  a4%f, a5 %f \n", Error_a4, Error_a5);
	if (debug) printf("desired velocity a4%f  a5 %f\n", DesiredVelocity_a4,DesiredVelocity_a5);
	*/
	//odeJointSetHingeParam(mainLink.joint,dParamFMax,dInfinity);
	//odeJointSetHingeParam(mainLink.joint,dParamVel,DesiredVelocity_a4);

	//odeJointSetHingeParam(LinkBall.joint,dParamFMax,dInfinity);
	//odeJointSetHingeParam(LinkBall.joint,dParamVel,DesiredVelocity_a5);
	
	//const dReal *vel_stage;
	//vel_stage =odeBodyGetLinearVel(stage.body);
	//odeBodySetLinearVel(stage.body, 0.0, 0.0, 0.0);
	


	//stepOde(1);
	
	//float angle_f=odeJointGetHingeAngle(mainLink.joint);
	//float angle_f1=odeJointGetHingeAngle(mainLink.joint);
	//if (debug) printf("angle after  a4%f  a5 %f\n", angle_f,angle_f1 );



	//position feedback
	const dReal *pos_first1 = odeBodyGetPosition(ball.body);
	const dReal *pos_first2 = odeBodyGetPosition(ball2.body);
	//printf(" ball position befor setting  x %f  y %f \n", pos_first[0],pos_first[1] );
	//printf(" setting postion x%f , y %f \n",pos_robotx,pos_roboty  );

	odeBodySetPosition(ball.body,pos_robot1x,pos_robot1y,pos_first1[2]);
	odeBodySetPosition(ball2.body,pos_robot2x,pos_robot2y,pos_first2[2]);	




	//velocity feedback

	//velocity filter

	const dReal *vel, *vel2;
	float vel_robot1X,vel_robot1Y,vel_robot2X,vel_robot2Y ;

	float alpha = 0.0;

	current_pos1X = pos_robot1x;
	current_pos1Y = pos_robot1y;

	
	current_pos2X = pos_robot2x;
	current_pos2Y = pos_robot2y;

	//printf(" current_posX %f current_posY %f\n", current_posX, current_posY );
	vel_robot1X = (current_pos1X-last_pos1X)/timeStep;  //TODO: also get the values in other axes.
	vel_robot1Y = (current_pos1Y-last_pos1Y)/timeStep;

	vel_robot2X = (current_pos2X-last_pos2X)/timeStep;  //TODO: also get the values in other axes.
	vel_robot2Y = (current_pos2Y-last_pos2Y)/timeStep;

	vel_est1x = alpha*last_vel_est1x +(1-alpha)*vel_robot1X; 
	vel_est1y = alpha*last_vel_est1y +(1-alpha)*vel_robot1Y; 

	vel_est2x = alpha*last_vel_est2x +(1-alpha)*vel_robot2X; 
	vel_est2y = alpha*last_vel_est2y +(1-alpha)*vel_robot2Y; 


	vel =odeBodyGetLinearVel(ball.body);
	vel2 =odeBodyGetLinearVel(ball2.body);

	if (debug) printf("vel of the body %f and real ball x%f  y%f \n", vel[1], vel_est1x, vel_est1y);
	
	odeBodySetLinearVel(ball.body, vel_est1x, vel_est1y, vel[2]);
	odeBodySetLinearVel(ball2.body, vel_est2x, vel_est2y, vel2[2]);
	stepOde(1);


///////////////////////////////////////////////////////////////////


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
	const dReal *pos2 = odeBodyGetPosition(ball2.body);


	float angle=odeJointGetHingeAngle(mainLink.joint);
	float angle_second=odeJointGetHingeAngle(LinkBall.joint);
	
	if(debug) printf("position befor simulate forward  x%f  y %f \n", pos[0],pos[1]);


	float rel1, rel_second1,rel2, rel_second2;
	rel1= odeBodyGetPosRelPoint( stage.body, pos[0], pos[1], pos[2]);
	rel_second1=odeBodyGetPosRelPoint1( stage.body, pos[0], pos[1], pos[2]);


	rel2= odeBodyGetPosRelPoint( stage.body, pos2[0], pos2[1], pos2[2]);
	rel_second2=odeBodyGetPosRelPoint1( stage.body, pos2[0], pos2[1], pos2[2]);

	//float stateVector[6]={pos[0],pos[1], pos2[0], pos2[1], angle, angle_second};
	float stateVector[6]={rel1,rel_second1, rel2, rel_second2, angle, angle_second};


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
		
			
			dReal MaxForce = dInfinity;
			
			odeJointSetHingeParam(mainLink.joint, dParamFMax, MaxForce);
			odeJointSetHingeParam(mainLink.joint, dParamVel,control[0] );
			odeJointSetHingeParam(LinkBall.joint, dParamFMax, MaxForce);
			odeJointSetHingeParam(LinkBall.joint, dParamVel,control[1] );

			stepOde(1);
			const dReal *pos= odeBodyGetPosition(ball.body);
			const dReal *pos2= odeBodyGetPosition(ball2.body);


			float rel1, rel_second1,rel2, rel_second2;
			rel1= odeBodyGetPosRelPoint( stage.body, pos[0], pos[1], pos[2]);
			rel_second1=odeBodyGetPosRelPoint1( stage.body, pos[0], pos[1], pos[2]);


			rel2= odeBodyGetPosRelPoint( stage.body, pos2[0], pos2[1], pos2[2]);
			rel_second2=odeBodyGetPosRelPoint1( stage.body, pos2[0], pos2[1], pos2[2]);


			float angle=odeJointGetHingeAngle(mainLink.joint);
			float angle_second = odeJointGetHingeAngle(LinkBall.joint);

			const dReal *vel_inside  = odeBodyGetLinearVel(ball.body);
			const dReal *vel_inside2  = odeBodyGetLinearVel(ball2.body);
			
			float weight_pos = 11.5f ;
			float weight_angle = 10.5f;
			float weight_vel= 9.5f;
			//working 12.5, 12.5, 9.5,9.5
			float cost=squared((0.08-rel1)*weight_pos)+squared((0.03-rel_second1)*weight_pos)+squared((0.08-rel2)*weight_pos)+squared((-0.03-rel_second2)*weight_pos)+squared(angle*weight_angle)+squared(angle_second*weight_angle)+squared(vel_inside[0]*(0.08-rel1)*weight_vel) + squared(vel_inside[1]*(0.03-rel_second1)*weight_vel)+squared(vel_inside2[0]*(0.08-rel2)*weight_vel) + squared(vel_inside2[1]*(-0.03-rel_second2)*weight_vel) ;
			//+squared(control[0]*1.5)+squared(control[1]*1.5) ;//+ squared(vel_robotX*0.05f)+ squared(vel_robotY*0.05f) ;
			
	/*
			if (-0.04<pos[1] && pos[1] <0.04 && -0.04<pos[0] && pos[0]<0.00 )
			{
			cost = cost+500;
			}

			if (-0.04<pos2[1] && pos2[1] <0.04 && -0.04<pos2[0] && pos2[0]<0.00 )
			{
			cost = cost+500;
			}
	*/	
		//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as in this simple case.
		//instead, one may use arbitrary state features
			float stateVector[6]={rel1,rel_second1,rel2,rel_second2, angle, angle_second};
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
	

	odeJointSetHingeParam(mainLink.joint,dParamFMax,dInfinity);
	odeJointSetHingeParam(mainLink.joint,dParamVel,control[0]);
	odeJointSetHingeParam(LinkBall.joint,dParamFMax,dInfinity);
	odeJointSetHingeParam(LinkBall.joint,dParamVel,control[1]);

	stepOde(1);
	saveOdeState(0);


	const dReal *pos_final1 = odeBodyGetPosition(ball.body);
	const dReal *pos_final2 = odeBodyGetPosition(ball2.body);
	float angle1=odeJointGetHingeAngle(mainLink.joint);

	const dReal *vel_sim;

	//vel_sim =odeBodyGetLinearVel(ball.body);
	
	//sending the control for real robot
	res.commandx = control[0];
    res.commandy = control[1];


	ros::Time end = ros::Time::now();
	double dt = (begin - end).toSec();
	if (debug) printf(" dt %f\n", dt );

	last_pos1X = pos_robot1x;
	last_pos1Y = pos_robot1y; 
	last_vel_est1x = vel_est1x;
	last_vel_est1y = vel_est1y;


	last_pos2X = pos_robot2x;
	last_pos2Y = pos_robot2y; 
	last_vel_est2x = vel_est2x;
	last_vel_est2y = vel_est2y;

	//work << std::setw(10)<<pos1[0] <<std::setw(10)<<pos1[0] << std::setw(10)<< pos_robotx  << std::setw(10)<< pos_roboty<< std::setw(10)<<vel_estx  <<std::setw(10)<<vel_esty << std::setw(10)<<vel_sim[0] <<std::setw(10)<<vel_sim[1]<< std::setw(10)<<ang_robot_a5  <<std::setw(10)<<ang_robot_a4<<std::setw(10)<<angle  <<std::setw(15)<< angle1<<std::endl ;
	
	
	//printf("rel_vec %f and stage %f\n", re_vec, stage_pos[0]);
	if (final_debug) printf("FINAL Posx %1.3f,posy = %f angle %1.3f, cost=%1.3f, control %f \n",pos_final1[0],pos_final1[1],angle1*180/3.1416,cost,control[0]);
	/*
	int j = 0;
	loop : std::cin >> j;
	if ( j != 1)
	goto loop;
	*/
}







int main(int argc, char **argv)
{
	ros::init(argc,argv, "talker");
	ros::NodeHandle n ;


	chatter_pub = n.advertise<std_msgs::Float32>("from_ode", 1);
	service = n.advertiseService("twoBall", robot);


	initOde(nSamples+1);
	setCurrentOdeContext(ALLTHREADS);
	odeRandSetSeed(0);
	odeSetContactSoftCFM(0);
	odeWorldSetGravity(0, 0, -9.81f);
	//work.open ("fresh_working.txt");
	//creating stage
	stage.body = odeBodyCreate();
	stage.geom = odeCreateBox(0.46f, 0.38f, 0.05f);
	odeMassSetBoxTotal(stage.body, 1.05, 0.46f,0.38f, 0.05f);
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
	ball.radius = 0.02f ;
	ball.body = odeBodyCreate();
	ball.geom = odeCreateSphere( ball.radius);
	odeMassSetSphereTotal(ball.body, 0.026,ball.radius);
	odeBodySetPosition(ball.body,-0.08,0.03,h_floor_table+h_base+h_sphere+h_support+2*stage_dim[2]);
	odeGeomSetBody(ball.geom,ball.body);
	printf("ball body id %f, geom id %f \n", ball.body, ball.geom);



	//creating second ball
	ball2.radius = 0.015f ;
	ball2.body = odeBodyCreate();
	ball2.geom = odeCreateSphere( ball2.radius);
	odeMassSetSphereTotal(ball2.body, 0.026,ball2.radius);
	odeBodySetPosition(ball2.body,-0.08,-0.03,h_floor_table+h_base+h_sphere+h_support+2*stage_dim[2]);
	odeGeomSetBody(ball2.geom,ball2.body);
	printf("ball body2 id %f, geom id %f \n", ball2.body, ball2.geom);


	

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



	//creating Obstacle
	obs.radius = 0.02f;


	obs.body = odeBodyCreate();
	obs.geom = odeCreateBox( 0.061,0.031,0.02); 
	odeMassSetBoxTotal(obs.body, 0.006,0.062,0.031,0.031); 
	odeBodySetPosition(obs.body,-0.05,0.0,h_floor_table+h_base+h_sphere+h_support+stage_dim[2]+obs.radius);
	odeGeomSetBody(obs.geom,obs.body);
	printf("capsule obstacle body id %f, geom id %f \n", obs.body, obs.geom);



	//creating Obstacle2
	obs2.radius = 0.02f;


	obs2.body = odeBodyCreate();
	obs2.geom = odeCreateBox( 0.031,0.013,0.02); 
	odeMassSetBoxTotal(obs2.body, 0.006,0.031,0.13,0.031); 
	odeBodySetPosition(obs2.body,0.00,0.0,h_floor_table+h_base+h_sphere+h_support+stage_dim[2]+obs.radius);
	odeGeomSetBody(obs2.geom,obs2.body);
	printf("capsule obstacle body id %f, geom id %f \n", obs2.body, obs2.geom);


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

	
	//Stage and Obstacle
	obs.joint =odeJointCreateFixed();
	odeJointAttach(obs.joint,stage.body,obs.body);
	odeJointSetFixed(obs.joint);



	//Stage and Obstacle 2
	obs2.joint =odeJointCreateFixed();
	odeJointAttach(obs2.joint,stage.body,obs2.body);
	odeJointSetFixed(obs2.joint);


	


	setCurrentOdeContext(0);
	saveOdeState(0);
	//initialize the optimizer
	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,minControl,maxControl,controlMean,controlStd,controlDiffStd,controlDiffDiffStd,mutationScale,NULL);
	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,true,0.001f);

	while (ros::ok())
	{

        	ros::spinOnce();
	}

	return 0;
}

