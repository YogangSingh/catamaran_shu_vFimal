# catamaran_shu_v2
 repo for the improved catamaran design



# Catamaran SHU

## Introduction
Catamaran SHU V2 is the improved version of the autonomous surface vehicle built and designed by me for my masters thesis. This version has improved electronics hardware and design changes that make the USV betters
## Features
- Feature 1: BNO055 IMU Integration.
- Feature 2: RTK GPS Added (GPS-RTK2 Board - ZED-F9P ).
- Feature 3: Dual mode control: radio control mode and autonomous mode.
- Feature 4: Arduino <-> ROS Integration on Arduino MEGA 2560..
- Feature 5: 360 Degree Lidar. (RPLidar s2)
  
## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
What things you need to install the software and how to install them:
```
[Install ROS Noetic]
wget -c https://raw.githubusercontent.com/qboticslabs/ros_install_noetic/master/ros_install_noetic.sh && chmod +x ./ros_install_noetic.sh && ./ros_install_noetic.sh
``
[Install Ubuntu and Ubuntu Desktop on Raspberry Pi]
sudo apt-get update
sudo apt-get install ubuntu-desktop
```

### Installing
A step-by-step series of examples that tell you how to get a development environment running:

1. Clone the repo:
   ```
   git clone https://github.com/broskunta/catamaran_shu_v2.git
   ```
2. Navigate to the project directory:
   ```
   cd catamaran_shu_v2
   ```
3. Run catkin_make
   ```
   catkin_make
   ```

## Usage
How to use the project:
```
1. Make sure the Arduino board with the IMU is connected on /dev/tty/USB0.
2. Make sure the GPS is connected on /dev/ttyACM0 on the RPi.
3. Make sure the Lidar is connected on /dev/ttyUSB1 on the RPi.
4. Run the robot with the usv_control.launch file
```

## Running the Tests
Explain how to run the automated tests for this system:
```
[For Straight Line Behaviour Test]
roslaunch gps_navigation straight_behaviour.launch
```
```
[For Circular Behaviour Test]
roslaunch gps_navigation circular_behaviour.launch 
```


## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments
- Daniel Riley https://www.rctestflight.com/
- The team at Heron USV https://github.com/heron
- https://github.com/bsb808
- The team at TheConstruct. https://www.theconstructsim.com/
- The Technovation Team https://www.instructables.com/member/Technovation/
-  Sidney Okoroafor https://github.com/Syd-E



---
