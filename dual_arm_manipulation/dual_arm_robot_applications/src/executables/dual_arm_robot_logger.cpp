//
// Created by Chunting on 24.03.17.
//

// ROS
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

#include "ur_logging/UrLogger.h"


int main(int argc, char **argv){
    ros::init(argc, argv, "dual_arm_robot_logger");
    ros::NodeHandle nh;

    ros::AsyncSpinner asyncSpinner(2);
    asyncSpinner.start();

    std::vector<std::string> ur_namespaces;
    ur_namespaces.push_back("left");
    ur_namespaces.push_back("right");
    UR_Logger ur_logger(nh, ur_namespaces);
    // sampling rate, hz
    ur_logger.start(20);
    // sampling time, second
    ros::waitForShutdown();
    ur_logger.stop();

    ros::shutdown();
    return 0;
}