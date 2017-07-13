#include <drawstuff/drawstuff.h>

#include <cstdio>
#include <boost/thread.hpp>
//#include <boost/chrono.hpp>
#include <iostream>
#include <valarray> 
#include  <vector>
#include <numeric>
#include <math.h>
#include "MathUtils.h"
#include "ControlPBP.h"
#include "UnityOde.h"
#include <ode/ode.h>
#include <stdio.h>
#include <sstream>
#include <cstdio>
#include <iostream>
//#include <drawstuff/drawstuff.h>
//#include <windows.h>
#include <ctime>
#include<chrono>
#include "boost/date_time/posix_time/posix_time.hpp" 

using namespace std;
using namespace AaltoGames;
static const int ANGLE=0;  //state variable index
static const int AVEL=1;	//state variable index
const dReal *pos,*R1,*pos2,*R2,*pos3,*R3;
dsFunctions fn;
float conv = 0.0;
float pos_robot;
float ang_robot;
int body, hinge,body1;
dReal geom,geom1;
float last_angle ;
float change;
float current_ball = 0.1, last_ball =0.1 ;
 

const int nSamples=80;
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
float controlStd=0.9f*C;	//sqrt(\sigma_{0}^2 C_u) of the paper (we're not explicitly specifying C_u as u is a scalar here). In effect, a "tolerance" for torque minimization in this test
float controlDiffStd=100.0f*C;	//sqrt(\sigma_{1}^2 C_u) in the pape. In effect, a "tolerance" for angular jerk minimization in this test
float controlDiffDiffStd=1000.0f*C; //sqrt(\sigma_{2}^2 C_u) in the paper. A large value to have no effect in this test.
float mutationScale=0.25f;		//\sigma_m in the paper


boost::condition_variable data_ready_cond;
boost::condition_variable data_ready_cond_master;
boost::mutex data_ready_mutex;
boost::mutex grip;
std::vector<bool> data_vector(nSamples,false);
std::vector<int> check_vector(nSamples,0);
int sum = 0;
typedef boost::posix_time::ptime Time;



	
void workFunc(int id)
	{
		
	    int i;
		Time t1;
		grip.lock();
		t1=boost::posix_time::microsec_clock::local_time();
		std::cout << to_iso_extended_string(t1) << "----"<< "in" <<"---"<< id <<"\n";
		grip.unlock();
	    // printf("in thread %d \n", id);
		srand(id);
		thread_start: //printf("continue threading \n");

	    boost::unique_lock<boost::mutex> lock_slave(data_ready_mutex);
		while(!data_vector[id])
			{
			//printf("waiting thread %d, data valud %d  \n", id, data[id]);
			data_ready_cond.wait(lock_slave);
	
		    }

		float control_t;
     	

		//boost::lock_guard<boost::mutex> guard( grip );
		
		//grip.lock();
		pbp.getControl(id,&control_t);
		//printf("control: %f \n",control_t);
		
		//get the mapping from this to previous state (affected by resampling operations)
		int previousStateIdx=pbp.getPreviousSampleIdx(id);
		//simulate to get next state.
		setCurrentOdeContext(id+1);
		//printf("previous state id: %d \n",previousStateIdx);
		restoreOdeState(previousStateIdx+1); //continue the a trajectory (selected in the resampling operation inside ControlPBP)

		dReal Gain_t = 1;
			dReal MaxForce = dInfinity;
		odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
		odeJointSetHingeParam(hinge,dParamVel,control_t  );

        // grip.lock();
		stepOde(1);
		//grip.unlock();
		
		const dReal *pos_t = odeBodyGetPosition(body1);
		float angle_t=odeJointGetHingeAngle(hinge);

		//change_t = angle_t -last_angle_t ;
		//current_ball = pos[0];
		//float ball_vel = (current_ball - last_ball)/timeStep ;

		float cost_t=squared((pos_t[0])*72.0f);// +squared(control_t * 10.5);// + squared(ball_vel * 0.2) + squared(angle*0.3f) ; //+squared(control * 5.2);
		
			
		//store the state and cost to C-PBP. Note that in general, the stored state does not need to contain full simulation state as 					in this simple case.
		//instead, one may use arbitrary state features
		float stateVector_t[2]={pos_t[0],angle_t};

		//float stateVector[2]={angle,aVel};
		//grip.lock();
		pbp.updateResults(id,&control_t,stateVector_t,cost_t);
		// grip.unlock();
		

		{
		data_vector[id] = false; 
		check_vector[id] = 1; 
		}
		
		//master_ready = true ; 
		grip.lock();
		sum = std::accumulate(check_vector.begin(), check_vector.end(), 0);
		grip.unlock();
		data_ready_cond_master.notify_all();
		//printf("END OF thread %d\n",id);
		goto thread_start;
	}



void robot()
{

	   // printf("in robot \n");
	    std::clock_t begin ; 
		begin = clock();

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
			//printf("sample no: %d \n" ,i);
			//save the state to the sample context (needed later in resampling)
			saveOdeState(i+1,0);


		}




		//signal the start of new C-PBP iteration
		setCurrentOdeContext(0);
		restoreOdeState(0);

		const dReal *pos = odeBodyGetPosition(body1);
		float angle=odeJointGetHingeAngle(hinge);

		//printf(" posX x :%f, aVel: %f  \n",pos[0],aVel);
		float stateVector[2]={pos[0],angle};
		pbp.startIteration(true,stateVector);



		//simulate forward
		for (int k=0; k<nTimeSteps; k++)

		{
			//printf("ID of k %d \n", k);
			//signal the start of a planning step
				pbp.startPlanningStep(k);
						//NOTE: for multithreaded operation, one would typically run each iteration of the following loop in a separate thread.
			//The getControl(), getPreviousSampleIdx(), and updateResults() methods of the optimizer are thread-safe.
			//The physics simulation is also thread-safe as we have full copies of the simulated system for each thread/trajectory
				

				//int sum = std::accumulate(check.begin(), check.end(), 0);
				{
				boost::unique_lock<boost::mutex> lock(data_ready_mutex);
				std::fill(data_vector.begin(), data_vector.end(), true);
				std::fill(check_vector.begin(), check_vector.end(), 0);
				}
				data_ready_cond.notify_all();
				
			    boost::unique_lock<boost::mutex> lock_master(data_ready_mutex);
				while (sum < nSamples)
	
					{
					//sum = std::accumulate(check.begin(), check.end(), 0);
					//printf("waiting robot, summ %d  \n", sum);
					data_ready_cond_master.wait(lock_master);	
					}
	
			
				//save all states, will be used at next step (the restoreOdeState() call above)
				
				//printf("HEre \n");
				for (int i=0; i<nSamples; i++)
					{
					saveOdeState(i+1,i+1);
					}

				//signal the end of the planning step. this normalizes the state costs etc. for the next step
				pbp.endPlanningStep(k);
				sum = 0;
				//printf("done roboting of step %d \n", k);
				
				
				/*
				int i = 0;
				loop : cin >> i;
				if ( i != 1)
				goto loop;
				*/
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

		
    	dReal MaxForce = dInfinity;
		odeJointSetHingeParam(hinge,dParamFMax,dInfinity);
		odeJointSetHingeParam(hinge,dParamVel,control);

		stepOde(1);
	


		//save the resulting state as the starting state for the next iteration
		saveOdeState(0);

		//print output, both angle and aVel should converge to 0, with some sampling noise remaining
		pos = odeBodyGetPosition(body1);
		//angle=odeJointGetHingeAngle(hinge);
		angle=odeJointGetHingeAngle(hinge);

        
		conv = angle ;
		last_angle = angle;
		last_ball = pos[0] ;
		
		printf("FINAL Posx %1.3f,posz = %f  angle %1.3f, cost=%1.3f control : %1.3f \n",pos[0],pos[2],angle,cost,control);
	  	// printf("angle %1.3f, avel %1.3f, cost=%1.3f\n",angle,aVel,cost);

		std::clock_t end ; 
		end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		printf("elasped time %f \n", elapsed_secs);
		
	
}










int main(int argc, char **argv)
{

	//Allocate one simulation context for each sample, plus one additional "master" context
	initOde(nSamples+1);
	setCurrentOdeContext(ALLTHREADS);
	odeRandSetSeed(0);
	odeSetContactSoftCFM(0);
	odeWorldSetGravity(0, 0, -9.81f);


		//Build the model:
	//create the stage
    geom = odeCreateBox(.4,.4,0.05);
	//Create a body and attach it to the geom
    body=odeBodyCreate();
   //odeBodySetMass (body, 0.94f);
    odeMassSetBoxTotal(body, 0.94f, 0.4, 0.4 , 0.05) ;
    odeBodySetPosition(body,0,0,2.5);
	odeGeomSetBody(geom,body);

		


	hinge=odeJointCreateHinge();
	odeJointAttach(hinge,body,0);
	odeJointSetHingeAnchor(hinge,0,0,2.55);
	odeJointSetHingeAxis(hinge,0,1,0);




	///////CREATING BALL//////////

	geom1 = odeCreateSphere(.03f);
	//Create a body and attach it to the geom
	body1=odeBodyCreate();
	//odeMassSetSphereTotal(body1,.05f,1.0f);
	//odeMassSetSphere()
	odeMassSetSphereTotal(body1,0.04,0.03);
    odeBodySetPosition(body1,0.1,0,2.554f);
	//odeBodySetMass (body1, 0.04);
	odeGeomSetBody(geom1,body1);


	setCurrentOdeContext(0);
	saveOdeState(0);





    //initialize the optimizer

	pbp.init(nSamples,nTimeSteps,nStateDimensions,nControlDimensions,&minControl,&maxControl,&controlMean,&controlStd,&controlDiffStd,&controlDiffDiffStd,mutationScale,NULL);
	//set further params: portion of "no prior" samples, resampling threshold, whether to use the backwards smoothing pass, and the regularization of the smoothing pass
	pbp.setParams(0.1f,0.5f,true,0.001f);




	//dsSimulationLoop (argc,argv,900,600,&fn);
	printf("Spwning threads \n");

	std::vector<boost::thread *> z;

	for (int i = 0; i < nSamples ; ++i)
	{
		z.push_back(new boost::thread(workFunc,i));
		
		
	}
	
	
	//while (1)

	for (int s=0 ; s<15 ; s++)
	{
		
		robot();
		
	}


		

	
	printf("Done, press enter.");
	getchar();

	return 0;
	}

