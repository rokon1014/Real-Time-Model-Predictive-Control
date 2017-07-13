
#define dSINGLE
#include <ode/ode.h>
#include "ros/ros.h"

#include <stdio.h>
#include <cstdio> 
#include <iostream>  
#include "std_msgs/Float32.h"
#include <ctime>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>
//#include <conio.h> 

//initializatton



static dWorldID world;
static dSpaceID space;
static dGeomID  ground;
static dGeomID  slope;
static dJointGroupID contactgroup;
static int flag = 0;

int c=0;

bool taint = false;


using namespace std;
dReal side2[3] = {.4,.4,0.05};
dReal side_small[3]= {.5,.5,.05};
static dJointID joint[3];
const dReal *pos1,*R1,*pos2,*R2,*pos3,*R3;  
//initialization of drawstuff


#ifdef  dDOUBLE
#define dsDrawBox      dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCapsule  dsDrawCapsuleD
#endif

//variable to be used

 std_msgs::Float32 msg_pid; 
 ros::Publisher chatter_pub ;

 float cam;

  dMass m3;

int num= 1; 
int move=1; 
float last_error=0;
float vel, error=0,sum_error=0,dif_error=0,prev_vel=0;
 dReal x, y, z;

  float dist;
  float target = 0;
  float kp,kd,ki;
  float step = 0.03;

  double last_pos = 0;
  int tick = 4 ;

//initialization for body and geometry

typedef struct {
  dBodyID body;     // a body
  dGeomID geom;    //  a geometry
  dReal   radius,length,mass;  // radius[m], length[m], mass[kg]
} myLink;

myLink stage,ball,sbox;



void cameraCallback(const std_msgs::Float32& cam_sim)
{

 cam = cam_sim.data;
//ROS_INFO("In camera: [%f]", cam_sim.data);
}

//collission detection 

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
  const int N = 10;
  dContact contact[N];

  if (((o1 == ground) && (o2 == stage.geom )) ||
	  ((o1 == stage.geom)  && (o2 == stage.geom))) return;



  int isGround = ((ground == o1 || stage.geom == o1)
	  || (ground == o2 || stage.geom == o2));


  
  int n =  dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));


   if (isGround)  {
    for (int i = 0; i < n; i++) {
      if ((o1 == ball.geom) || (o2 == ball.geom)) {
        // friciton: dContactApprox1 is the friction pyramid model
        contact[i].surface.mode = dContactApprox1|dContactSoftERP|dContactSoftCFM;
      }
	     else {
        // friction: default is the constant force limit model
        contact[i].surface.mode = dContactSoftERP|dContactSoftCFM;
      }


contact[i].surface.mu   = dInfinity;
      contact[i].surface.soft_erp = 1.0; //1.0;
      contact[i].surface.soft_cfm = 1e-10;
      dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
      dJointAttach(c,dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
    }
  }
}


//finding the current position of object

void printPos(dBodyID obj_body) 
{
	dInitODE();
  const dReal *pos;// Do not forget const to prevent an error
 
  pos = dBodyGetPosition(obj_body);// Return value is an pointer to a structure
  x = pos[0];y = pos[1]; z = pos[2];
  printf("x=%f y=%f z=%f \n", x, y, z);

  double change = x -last_pos ;

  last_pos = x ;



}



//simulation 

static void simLoop (int pause)
{
   
  // clock_t begin = clock();



   kp = .35;
   ki = .01; 
   kd=  .7;

   
   printPos(ball.body) ; 

  //  const dReal *po = dBodyGetPosition(ball.body);
	//printf("before postion: %f \n",po[0]);

   error = target-x ;  //proportional 
 // printf("error%f\n", error);

   sum_error=sum_error+(error*step);  //integral  
   //  printf("sum error%f\n", sum_error);

   dif_error = (error-last_error)/step;  //differential
    //printf("diff error%f\n", dif_error);
   vel = kp*error+ki*sum_error+kd*dif_error;  //control value 

   //printf("vel = %f", vel);

   last_error = error; 


dReal Gain = .1;


double TruePosition = dJointGetHingeAngle(joint[1]);





double degree = (TruePosition*180)/3.1416 ;

 
//printf("desired angle %f\n", vel);
dReal DesiredPosition =vel;
dReal Error = degree - DesiredPosition;
//printf("error%f\n", Error);
//dReal DesiredVelocity = -Error * Gain;

dReal DesiredVelocity = 2;



 dBodyGetMass(stage.body, &m3);
//cout<<m3.mass<<endl;



if (taint ) 
{  


	//if (tick == 0 )
//{
dReal MaxForce = dInfinity;

dJointSetHingeParam(joint[1], dParamFMax, MaxForce);  //setting maximum force

dJointSetHingeParam(joint[1], dParamVel,DesiredVelocity);

printf("current angle %f\n",degree );
printf(" velocity %f\n", DesiredVelocity);

ros::spinOnce();

//dJointSetHingeParam(joint[1], dParamVel,.05*(vel-prev_vel)); //setting velocity for rotation
prev_vel=vel;
tick = 3 ;
//printf("giving \n");

c = c+1 ;



 // printf("count %d\n", c);
//}

//printf("ín taint" );

}

tick = tick -1 ;

//printf("not giving \n");


taint = true ;


//dReal torque={0,2,0};
//dBodyAddTorque(stage.body,0,.001,0);
 //dJointAddHingeTorque (joint[1],.001);
dSpaceCollide(space,0,&nearCallback); // Collision detection

dWorldStep(world,step);// Step a world for 0.01 [sec]



  msg_pid.data = DesiredVelocity;
  chatter_pub.publish(msg_pid);   

 
//TruePosition = dJointGetHingeAngle(joint[1]);
//// degree = (TruePosition*180)/3.1416 ;
//printf(" after angle %f\n", degree);
//const dReal *po1 = dBodyGetPosition(ball.body);
	//printf("after postion: %f \n",po1[0]);

dJointGroupEmpty(contactgroup);  // Empty the joint group

//sleep(1);


//clock_t end = clock();
  //double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

 // printf(" time %f\n", elapsed_secs );

	
	int i = 0;
	loop : cin >> i;

		   if ( i != 1)
			   goto loop;

  
}



void createstage()
{
	dMass m1, m2;
  dReal x0 = 0.0, y0 = 0.0, z0 = 1.5;    
  stage.radius = 0.2;
  stage.mass   = 0.94;
  stage.body = dBodyCreate(world);


    dMassSetZero(&m1);
    dMassSetBoxTotal(&m1, stage.mass, side2[0],side2[1],side2[2]);
    dBodySetMass(stage.body, &m1);
    dBodySetPosition(stage.body, x0, y0, z0);

    

	stage.geom  = dCreateBox(space,side2[0],side2[1],side2[2] );
	dGeomSetBody(stage.geom, stage.body);

}





void makeBall()
{
  dMass m1;
  dReal x0 = 0.0, y0 = 0.0, z0 = 1.6;


    ball.radius = 0.059;
    ball.mass =.04;
    ball.body = dBodyCreate(world);
    dMassSetZero(&m1);
	dMassSetSphereTotal(&m1,ball.mass,ball.radius);

	dMassSetSphere(&m1, ball.mass, ball.radius);
    dBodySetMass(ball.body, &m1);
    dBodySetPosition(ball.body, x0, y0, z0);

	ball.geom = dCreateSphere(space, ball.radius);
    dGeomSetBody(ball.geom, ball.body);
  }





int main (int argc, char **argv)
{

   timeval a;
    timeval b;


   




  ros::init(argc,argv, "talker_pid");
  ros::NodeHandle n ;

   chatter_pub = n.advertise<std_msgs::Float32>("from_ode_pid", 1);


  ros::Subscriber sub_camera_sim = n.subscribe("/from_camera", 2, cameraCallback);
 
  ros::Rate loop_rate(33);


  dInitODE();       // Initialize ODE
  

  world = dWorldCreate();// Create a world
  space = dHashSpaceCreate(0);// Create a collision space
  contactgroup = dJointGroupCreate(0);// Create a joint group
  dWorldSetGravity(world,0,0,-9.8);  // Set gravity
  ground = dCreatePlane(space,0,0,1,0); // Create a ground
  createstage();// Create the stage
  makeBall();
  
  
   joint[1] = dJointCreateHinge(world, 0);   //create hinge joint
   dJointAttach(joint[1],stage.body,NULL);   
   dJointSetHingeAnchor (joint[1], 0, 0, 1.55);  //set the anchor for the joint
   dJointSetHingeAxis(joint[1], 0,1,0);
    pos1 = dBodyGetPosition(stage.body);
    R1   = dBodyGetRotation(stage.body);



ros::spinOnce();

while (1)

	//for (int e = 0 ; e<90 ; e++)
  {

      
   simLoop (0);


   // ros::spinOnce();
 loop_rate.sleep();  
  }
   
   
 

  dWorldDestroy (world);// Destroy the world 　
  dCloseODE();  // Close ODE
  
  return 0;

}

