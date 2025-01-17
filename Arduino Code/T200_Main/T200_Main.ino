//IMU Code to send IMY data to the IMu part to ROS
//I will need angular velocity, linear acceleration,
//Orientation in quat from the IMU sensor to make this
//code work

//Working code is from ros-robot-programming-book-by turtlebot3-Developers

//ROS Libraries
#include <ros.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/Imu.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>

//include libraries for Transmitter
#include <SPI.h>
#include <Servo.h>
#include <IBusBM.h>
/*
 *    Connections
   =======================
   IMU
   =======================
   Connect SCL to SCL pin (analog 5 on Arduino UNO)
   Connect SDA to SDA pin (analog 4 on Arduino UNO)
   Connect VDD to 3-5V DC (depending on your board's logic level)
   Connect GROUND to common ground

  =======================
   GPS
  =======================


  =======================
   TRANSMITTER
  =======================


  =======================
   ESCs
  =======================

  =======================
   BOARD RADIO COMMS
  =======================
 * 
 */

// Other arduino libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//Start ROS Handle
ros::NodeHandle nh;

const int MAX_PATH_SIZE = 10;

sensor_msgs::Imu imu_msg;

ros::Publisher imu_pub("imu", &imu_msg);

geometry_msgs::TransformStamped tfs_msg;
tf::TransformBroadcaster tfbroadcaster;

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

/* Define BNO055 Object */
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

//define comms pins for transmitter
#define RUDDER_PIN 5
#define THROTTLE_PIN 6
#define MODE_PIN 7

//define ESCs pin
Servo left_esc; //left motor
Servo right_esc; //right motor

int left_motor_speed = 1500;
int right_motor_speed = 1500;

// // Read the number of a given channel and convert to the range provided.
// // If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

// Callback functions for the path topics
void generatedPathCallback(const nav_msgs::Path& msg) {
    // Process and print data for /straight_line_behavior_generated_path
    Serial.println("Generated Path Data:");
    for (int i = 0; i < MAX_PATH_SIZE; ++i) {
        // Assuming a pose of all zeros is not valid and indicates the end of the array
        if (msg.poses[i].pose.position.x != 0 || msg.poses[i].pose.position.y != 0 || msg.poses[i].pose.position.z != 0) {
            Serial.print(msg.poses[i].pose.position.x);
            Serial.print(",");
            Serial.print(msg.poses[i].pose.position.y);
            Serial.print(",");
            Serial.print(msg.poses[i].pose.position.z);
            Serial.print("\n");
        } else {
            // Break the loop if the pose is not valid
            break;
        }
    }
}

void followedPathCallback(const nav_msgs::Path& msg) {
    // Process and print data for /straight_line_behavior_followed_path
    Serial.println("Followed Path Data:");
    for (int i = 0; i < MAX_PATH_SIZE; ++i) {
        if (msg.poses[i].pose.position.x != 0 || msg.poses[i].pose.position.y != 0 || msg.poses[i].pose.position.z != 0) {
            Serial.print(msg.poses[i].pose.position.x);
            Serial.print(",");
            Serial.print(msg.poses[i].pose.position.y);
            Serial.print(",");
            Serial.print(msg.poses[i].pose.position.z);
            Serial.print("\n");
        } else {
            break;
        }
    }
}



//wheel data
float wheel_radius = 0.1015; // meters
float wheel_separation = 0.31; // meters
int micros_min = 1500; // Min microseconds for ESC
int micros_max = 2000; // Max microseconds for ESC

//variables for autospeed command
int left_motor_speed_auto, right_motor_speed_auto;

// 25% of max RPM at 11.1V for 850KV motor
float max_rpm_at_25_percent = 850 * 11.1 * 0.25; // 25% of Maximum RPM

// Function to map RPM to microseconds
int mapRpmToMicroseconds(float rpm) {
    // Mapping from -max_rpm_at_25_percent to max_rpm_at_25_percent to micros_min to micros_max
    int micros = map(rpm, -max_rpm_at_25_percent, max_rpm_at_25_percent, micros_min, micros_max);
    return constrain(micros, micros_min, micros_max);
}

//Velocity callback function
void cmdVelCallback(const geometry_msgs::Twist& msg) {
    float linear_velocity = msg.linear.x; // Linear velocity in m/s
    float angular_velocity = msg.angular.z; // Angular velocity in rad/s

    // Calculate wheel linear velocities
    float v_left = linear_velocity - (angular_velocity * wheel_separation / 2);
    float v_right = linear_velocity + (angular_velocity * wheel_separation / 2);

    // Convert linear velocities to angular velocities (rad/s)
    float omega_left = v_left / wheel_radius;
    float omega_right = v_right / wheel_radius;

    // Convert angular velocities to RPM
    float rpm_left = (omega_left * 60) / (2 * PI);
    float rpm_right = (omega_right * 60) / (2 * PI);

    // Map RPM to microseconds
    left_motor_speed_auto = mapRpmToMicroseconds(rpm_left);
    right_motor_speed_auto = mapRpmToMicroseconds(rpm_right);
}
//define subscriber to cmd_vel to get angular and linear velocities
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", cmdVelCallback);


//define subscribers
ros::Subscriber<nav_msgs::Path> subGeneratedPath("/straight_line_behavior_generated_path", generatedPathCallback);
ros::Subscriber<nav_msgs::Path> subFollowedPath("/straight_line_behavior_followed_path", followedPathCallback);

// define channel values from the transmitter
int ch1_value, ch3_value;
bool ch6_value;

static uint32_t pre_time;

//Setup
void setup() {

  // begin serial window
  Serial.begin(115200);

  //begin ROS parts
  nh.initNode();
  nh.advertise(imu_pub);
  nh.subscribe(sub);
  nh.subscribe(subGeneratedPath);
  nh.subscribe(subFollowedPath);
  tfbroadcaster.init(nh);
  Wire.begin();
  bno.begin();
  delay(1000);

    //define pins
  pinMode(RUDDER_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);

  Serial.println("[INFO]\tArming Motors");

  
    //attach ESCs
  left_esc.attach(31);
  right_esc.attach(13);

  left_esc.writeMicroseconds(left_motor_speed);
  right_esc.writeMicroseconds(right_motor_speed);
  delay(2000);
  Serial.println("[INFO]\tMotors are armed");
}

void loop()
{
  

  //start imu
  start_imu();
  
  nh.spinOnce();
  delay(BNO055_SAMPLERATE_DELAY_MS);

    get_control_data();
  
  bool mode = get_mode();
  if (mode == 0) //manual mode
    {
      //get throttle and rudder values
      int throttle = get_throttle();
      int rudder = get_rudder();

      //write values to esc
      left_motor_speed = left_speed_manual(throttle, rudder);
      right_motor_speed = right_speed_manual(throttle,rudder);

      left_esc.writeMicroseconds(left_motor_speed);
      right_esc.writeMicroseconds(right_motor_speed);
    }
  else
  {
    //auto mode
    //read speed values of Vx and Vz from ROS publisher cmd_vel
    // then translate it to speed for each esc.
    
    //stop motors
    left_motor_speed = 1500;
    right_motor_speed = 1500;

    //get speed from the publisher
    //inear_// reads linear and angular velocity
    //get speed from the publisher
    //inear_// reads linear and angular velocity
    left_esc.writeMicroseconds(left_motor_speed_auto);
    right_esc.writeMicroseconds(right_motor_speed_auto);
    //convert speeds to 
    //write speeds to the motors
   

  }

  Serial.print("Left Motor Speed: ");
  Serial.println(left_motor_speed);
  Serial.print(" Right Motor Speed: ");
  Serial.println(right_motor_speed);

  nh.spinOnce(); // Handle ROS callbacks
      
}

void start_imu()
{
  
    //get readings from IMU
  imu::Quaternion quat = bno.getQuat(); //quarternion
  imu::Vector<3> gyroData = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); //angular velocity
  imu::Vector<3> accData = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);  // Get acceleration data


  if (millis() - pre_time >= 50)
  {
  pre_time = millis();  
  
  imu_msg.header.stamp = nh.now();
  imu_msg.header.frame_id = "imu_link";
  
  //print out the data going to the message.
  //quat messages
  imu_msg.angular_velocity.x = gyroData.x();
  imu_msg.angular_velocity.y = gyroData.y();
  imu_msg.angular_velocity.z = gyroData.z();
  imu_msg.angular_velocity_covariance[0] = 0.02;
  imu_msg.angular_velocity_covariance[1] = 0;
  imu_msg.angular_velocity_covariance[2] = 0;
  imu_msg.angular_velocity_covariance[3] = 0;
  imu_msg.angular_velocity_covariance[4] = 0.02;
  imu_msg.angular_velocity_covariance[5] = 0;
  imu_msg.angular_velocity_covariance[6] = 0;
  imu_msg.angular_velocity_covariance[7] = 0;
  imu_msg.angular_velocity_covariance[8] = 0.02;

  //linear accelaration
  imu_msg.linear_acceleration.x = accData.x();
  imu_msg.linear_acceleration.y= accData.y();
  imu_msg.linear_acceleration.z = accData.z();
  imu_msg.linear_acceleration_covariance[0] = 0.04;
  imu_msg.linear_acceleration_covariance[1] = 0;
  imu_msg.linear_acceleration_covariance[2] = 0;
  imu_msg.linear_acceleration_covariance[3] = 0;
  imu_msg.linear_acceleration_covariance[4] = 0.04;
  imu_msg.linear_acceleration_covariance[5] = 0;
  imu_msg.linear_acceleration_covariance[6] = 0;
  imu_msg.linear_acceleration_covariance[7] = 0;
  imu_msg.linear_acceleration_covariance[8] = 0.04;

  //Linear orientation
  imu_msg.orientation.w = quat.w();
  imu_msg.orientation.x = quat.x();
  imu_msg.orientation.y = quat.y();
  imu_msg.orientation.z = quat.z();

  imu_msg.orientation_covariance[0] = 0.0025;
  imu_msg.orientation_covariance[1] = 0;
  imu_msg.orientation_covariance[2] = 0;
  imu_msg.orientation_covariance[3] = 0;
  imu_msg.orientation_covariance[4] = 0.0025;
  imu_msg.orientation_covariance[5] = 0;
  imu_msg.orientation_covariance[6] = 0;
  imu_msg.orientation_covariance[7] = 0;
  imu_msg.orientation_covariance[8] = 0.0025;

  //publish the messages
  imu_pub.publish(&imu_msg);

  tfs_msg.header.stamp = nh.now();
  tfs_msg.header.frame_id = "base_link";
  tfs_msg.child_frame_id = "imu_link";
  tfs_msg.transform.rotation.w = quat.w();
  tfs_msg.transform.rotation.x = quat.x();
  tfs_msg.transform.rotation.y = quat.y();
  tfs_msg.transform.rotation.z = quat.z();

  tfs_msg.transform.translation.x = 0.0;
  tfs_msg.transform.translation.y = 0.0;
  tfs_msg.transform.translation.z = 0.0;

  tfbroadcaster.sendTransform(tfs_msg);
  }

}

void get_control_data()
{
  ch1_value = pulseIn(RUDDER_PIN, HIGH);
  ch3_value = pulseIn(THROTTLE_PIN, HIGH);
  ch6_value = redSwitch(MODE_PIN, false);

  Serial.print("Rudder: ");
  Serial.print(ch1_value);
  Serial.print(" Throttle: ");
  Serial.print(ch3_value);
  Serial.print(" Mode: ");
  Serial.print(ch6_value);
  Serial.print(" Left Motor Speed: ");
  Serial.print(left_motor_speed);
  Serial.print( "Right Motor Speed: ");
  Serial.print(right_motor_speed);
  delay(500);
}

bool get_mode()
{
  ch6_value = redSwitch(MODE_PIN, false);
  return ch6_value;
}

int get_throttle()
{
  return pulseIn(THROTTLE_PIN, HIGH);
}

int get_rudder()
{
   return pulseIn(RUDDER_PIN, HIGH);
}

int left_speed_manual(int throttle, int rudder) {
    // Calculate the difference from the neutral rudder position (1500)
    int rudder_offset = rudder - 1500;

    // Increase speed on the left motor when rudder moves to the right (positive offset)
    int speed = throttle + (rudder_offset / 2); // Divide to reduce effect

    // Ensure the speed does not exceed 2000 RPM or drop below 1500 RPM
    return constrain(speed, 1500, 2000);
}

int right_speed_manual(int throttle, int rudder) {
    // Calculate the difference from the neutral rudder position (1500)
    int rudder_offset = rudder - 1500;

    // Decrease speed on the right motor when rudder moves to the right (positive offset)
    int speed = throttle - (rudder_offset / 2); // Divide to reduce effect

    // Ensure the speed does not exceed 2000 RPM or drop below 1500 RPM
    return constrain(speed, 1500, 2000);
}