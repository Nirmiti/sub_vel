#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "math.h"
#include <cereal_port/CerealPort.h>
#include "std_msgs/Float64.h"
#define PI 3.1416
float rec_vel_x;
float rec_vel_y;
//float rec_ang_z;
float base_vel;
//int base_vel_int;
float base_angle;
int base_angle_int;
//std::stringstream vel_send;
//std::stringstream ang_send;
cereal::CerealPort device;
//char vel_send[5];
char ang_send[1];
int flag = 0;
//int old_base_vel_int = 0;
int old_base_angle_int = 0;
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void cmd_vel_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
  	float temp_val;

	rec_vel_x = msg->linear.x;
	rec_vel_y = msg->linear.y;
	//rec_ang_z = msg->angular.z;
	/*temp_val = (rec_vel_x*rec_vel_x) + (rec_vel_y*rec_vel_y);
	base_vel = sqrt(temp_val);
	if(base_vel >=0.0)
	{
		base_vel*=100;		
		//base_vel_int = 45.0;
		base_vel_int = (int) (base_vel+ 0.5);
	}*/
	if(rec_vel_x == 0 && rec_vel_y ==0)
	{
		ang_send[0]='x';
		base_vel = 0.0;
		base_angle_int = 0;
	}
	if (rec_vel_x !=0 && rec_vel_y !=0) 
	{
		base_angle = atan2(rec_vel_y,rec_vel_x)*180/PI;
	}
	if(base_angle >0.0 && base_angle<90.0)
	{	
		base_angle = 45.0;
		base_angle_int = (int) (base_angle+ 0.5);
	}
	 if(base_angle>90.0 && base_angle<=180.0)
	 {
		base_angle = 135.0;
		base_angle_int =(int) (base_angle_int + 0.5);
	}

	if(base_angle == 90.0)
	{

		base_angle = 90.0;
		base_angle_int =(int) (base_angle_int + 0.5);
	}
	if(base_angle <= -90.0 && base_angle >= -180.0) 
	{
		 base_angle = 135.0;
                base_angle_int = (int) (base_angle_int +0.5);
	}

	 if(base_angle >-90.0 && base_angle <0.0)
        {
                 base_angle = 45.0;
                base_angle_int = (int) (base_angle_int +0.5);
        }


	/* to set format as required by mbed*/
	/*if(base_vel_int<=9)
    	{
		vel_send[0]='v';
		vel_send[1]='0'+base_vel_int;
		vel_send[2]=' ';
		vel_send[3]=' ';
		vel_send[4]=' '; 
	}
	if(base_vel_int>=10 && base_vel_int<=99)
       // {vel_send<<"v"<<base_vel<<"  ";}
	   {
                vel_send[0]='v';
                vel_send[1]='0'+(base_vel_int/10);
                vel_send[2]='0'+(base_vel_int%10);
                vel_send[3]=' ';
                vel_send[4]=' ';
        }

	if(base_vel_int>99)
	// {vel_send<<"v"<<base_vel<<" ";}
	//msg.data = ss.str()
	{
		 vel_send[0]='v';
                vel_send[1]='0'+(base_vel_int/100);
                vel_send[2]='0'+((base_vel_int/10)%10);
                vel_send[3]='0'+(base_vel_int%10);
                vel_send[4]=' ';
	}*/
//	ROS_INFO("vel send %d\n",base_vel_int);
	//device.write(vel_send,5);
	 if(base_angle_int == 90 )
       // {ang_send<<"a"<<base_angle<<"   ";}
          {
               ang_send[0] ='z';
		base_vel = 0.55;
        }

	if(base_angle_int = 45)
       // {ang_send<<"a"<<base_angle<<"  ";}
           {
               /* ang_send[0]='a';
                ang_send[1]='0'+(base_angle_int/10);
                ang_send[2]='0'+(base_angle_int%10);
                ang_send[3]=' ';
                ang_send[4]=' ';*/
		ang_send[0] ='a';
		base_vel = 0.3;

        }


	if(base_angle_int= 135)
        // {ang_send<<"a"<<base_angle<<" ";}
	{
               /* ang_send[0]='a';
                ang_send[1]='0'+(base_angle_int/100);
                ang_send[2]='0'+((base_angle_int/10)%10);
                ang_send[3]='0'+(base_angle_int%10);
                ang_send[4]=' ';*/
		ang_send[0] ='s';
		base_vel = 0.3;
        }
	

	if (flag == 0 || (old_base_angle_int != base_angle_int) ) 
	{ 
	//ROS_INFO("vel send %d\n",base_vel_int);
        //device.write(vel_send,5);
	
	//ROS_INFO("angle written %d\n",base_angle_int);
	device.write(ang_send,1);
	flag =1;
	//old_base_vel_int = base_vel_int;
	old_base_angle_int = base_angle_int;
	}

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener_vel");

  ros::NodeHandle n;
   // char reply[REPLY_SIZE];
	try{ device.open("/dev/ttyACM1", 9600); } 
    catch(cereal::Exception& e)
    {
        ROS_FATAL("Failed to open the serial port!!!");
        ROS_BREAK();
    }
    ROS_INFO("The serial port is opened.");
  ros::Subscriber sub = n.subscribe("/cmd_vel", 1000, cmd_vel_Callback);
   ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("act_vel", 1000);
 ros::Publisher ang_pub = n.advertise<std_msgs::Float64>("act_th", 1000);

 // ros::Rate loop_rate(10);
  ros::Rate loop_rate(10);	
  while(ros::ok())
{
	/**
   * ros::spin() will enter a loop, pumping callbacks.  With this
   * version, all callbacks will be called from within this thread
   * (the main one).  ros::spin() will exit when Ctrl-C is pressed,
   * or the node is shutdown by the master.
   */
  ros::spinOnce();

	std_msgs::Float64 th;
	geometry_msgs::Twist vel;
	
	float to_pub_velx;
	float to_pub_vely;
	float to_pub_ang;
	float temp;
	temp = PI*base_angle_int;
	to_pub_ang = temp/180;
	to_pub_velx = base_vel*cos(to_pub_ang);
	to_pub_vely = base_vel*sin(to_pub_ang);
	vel.linear.x = to_pub_velx; 
	vel.linear.y = to_pub_vely;
	
	th.data = to_pub_ang;

	vel_pub.publish(th);
	ang_pub.publish(vel);
	loop_rate.sleep();
}


  return 0;
}
