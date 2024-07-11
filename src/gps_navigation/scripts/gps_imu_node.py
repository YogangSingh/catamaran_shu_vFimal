#!/usr/bin/env python

import rospy
from sensor_msgs.msg import NavSatFix, Imu
from geometry_msgs.msg import TwistWithCovarianceStamped, Quaternion
import serial
import pyubx2

# Replace with the appropriate serial port and baud rate
SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE = 115200

# Global variables to store GPS and IMU data
nav_sat_fix = NavSatFix()
imu_data = Imu()

def handle_ubx_message(msg):
    global nav_sat_fix, imu_data

    if msg.identity == 'NAV-PVT':
        nav_sat_fix.header.stamp = rospy.Time.now()
        nav_sat_fix.latitude = msg.lat * 1e-7
        nav_sat_fix.longitude = msg.lon * 1e-7
        nav_sat_fix.altitude = msg.height * 1e-3

        varH = pow(msg.hAcc / 1000.0, 2)  # to [m^2]
        varV = pow(msg.vAcc / 1000.0, 2)  # to [m^2]

        nav_sat_fix.position_covariance = [varH, 0.0, 0.0, 0.0, varH, 0.0, 0.0, 0.0, varV]
        nav_sat_fix.position_covariance_type = NavSatFix.COVARIANCE_TYPE_DIAGONAL_KNOWN
        fix_pub.publish(nav_sat_fix)

    elif msg.identity == 'NAV-VELNED':
        vel_msg = TwistWithCovarianceStamped()
        vel_msg.header.stamp = rospy.Time.now()
        vel_msg.twist.twist.linear.x = msg.velN * 1e-3
        vel_msg.twist.twist.linear.y = msg.velE * 1e-3
        vel_msg.twist.twist.linear.z = msg.velD * 1e-3

        covSpeed = pow(msg.sAcc * 1e-3, 2)  # Convert from mm/s to m/s
        vel_msg.twist.covariance = [covSpeed]*6 + [-1.0]*30  # Only speed variance is known
        vel_pub.publish(vel_msg)

    # Assuming your device provides IMU data in a custom UBX message,
    # you would handle it in a similar way here.

if __name__ == '__main__':
    rospy.init_node('gps_imu_node')

    fix_pub = rospy.Publisher('/fix', NavSatFix, queue_size=10)
    vel_pub = rospy.Publisher('/vel', TwistWithCovarianceStamped, queue_size=10)
    imu_pub = rospy.Publisher('/imu', Imu, queue_size=10)

    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        ubx = pyubx2.UBXReader(ser)

        while not rospy.is_shutdown():
            if ser.in_waiting > 0:
                try:
                    (raw_data, parsed_data) = ubx.read()
                    if parsed_data:
                        handle_ubx_message(parsed_data)
                except pyubx2.UBXStreamError as err:
                    rospy.logwarn(f"Stream error: {err}")
                except Exception as e:
                    rospy.logwarn(f"General error: {e}")

    except serial.SerialException as e:
        rospy.logerr(f"Serial port error: {e}")
    finally:
        if ser:
            ser.close()