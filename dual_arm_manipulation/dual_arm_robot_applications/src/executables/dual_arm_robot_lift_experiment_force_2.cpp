//
// Created by Chunting  on 08.04.17.
//

// ROS
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

// MoveIt!
#include <moveit_msgs/PlanningScene.h>
#include <moveit/move_group_interface/move_group.h>
#include <moveit_msgs/GetStateValidity.h>
#include <moveit_msgs/DisplayRobotState.h>

// Rviz
#include <moveit_msgs/DisplayTrajectory.h>

// Dual Arm Tools
#include "dual_arm_toolbox/TrajectoryProcessor.h"
#include "dual_arm_toolbox/Transform.h"

// Dual Arm Demonstrator
#include "dual_arm_demonstrator_iml/DualArmRobot.h"
#include "dual_arm_demonstrator_iml/SceneManager.h"

// UR Logger
#include "ur_logging/UrLogger.h"


int main(int argc, char **argv) {
    ros::init(argc, argv, "dual_arm_robot_demonstration");
    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::NodeHandle nh;

    #ifdef OFFLINE
    ROS_WARN("Robot offline");
    #endif

    // Dual Arm Robot Setup
    dual_arm_demonstrator_iml::DualArmRobot dualArmRobot(nh);

    // Scene Setup
    dual_arm_demonstrator_iml::SceneManager sceneManager(nh);
    sceneManager.setupSceneLift();

    // move home
    // dualArmRobot.moveHome();

    // setup constraints
    moveit_msgs::JointConstraint jcm;
    moveit_msgs::Constraints left_constraints;
    moveit_msgs::Constraints right_constraints;

    jcm.joint_name="right_shoulder_pan_joint";
    jcm.position = 2.4;
    jcm.tolerance_above = 0.7;
    jcm.tolerance_below = 2.5;
    jcm.weight = 1.0;
    right_constraints.joint_constraints.push_back(jcm);
    dualArmRobot.right_.setPathConstraints(right_constraints);

    jcm.joint_name="left_shoulder_pan_joint";
    jcm.position = 0.0;
    jcm.tolerance_above = 1.0;
    jcm.tolerance_below = 1.0;
    jcm.weight = 1.0;
    left_constraints.joint_constraints.push_back(jcm);
    dualArmRobot.left_.setPathConstraints(left_constraints);

    // Pick box1
    geometry_msgs::Vector3Stamped direction;
    direction.header.frame_id = "world";
    direction.vector.x = 0;
    direction.vector.y = 0;
    direction.vector.z = 0;//0.01;
    if (!dualArmRobot.pickBox("box1", direction)) {
        ROS_WARN("Pick failed");
        ROS_ERROR("Can't execute demonstration without successful pick. Demonstration aborted.");
        return 0;
    }
    /*
    geometry_msgs::PoseStamped place_pose_left = dualArmRobot.left_.getCurrentPose(dualArmRobot.left_.getEndEffectorLink());
    place_pose_left.pose.position.z = place_pose_left.pose.position.z - direction.vector.z;*/

    // clear path constraints
    dualArmRobot.left_.clearPathConstraints();
    dualArmRobot.right_.clearPathConstraints();

    // plan move and execute
    //geometry_msgs::PoseStamped start_pose = dualArmRobot.left_.getCurrentPose(dualArmRobot.left_.getEndEffectorLink());
    //geometry_msgs::PoseStamped lift_pose = start_pose;
    //lift_pose.pose.position.z = lift_pose.pose.position.z +1;
    geometry_msgs::Vector3Stamped linear_move_direction;
    linear_move_direction.vector.z = 0.00;
    linear_move_direction.header.frame_id = "world";

//#ifndef OFFLINE
    // experiment just hold
    ROS_INFO("Starting Log in 10");
    sleep(7);
    ROS_INFO("Starting Log in 3");
    sleep(1);
    ROS_INFO("Starting Log in 2");
    sleep(1);
    ROS_INFO("Starting Log in 1");
    sleep(1);
    ROS_INFO("Starting Log now");
    // start log
    std::vector<std::string> ur_namespaces;
    ur_namespaces.push_back("left");
    UR_Logger ur_logger(nh, ur_namespaces);
    ur_logger.start(100);

    ROS_INFO("Waiting 10 seconds");
    ros::Duration(7).sleep();
    ROS_INFO("Stop Log in 3");
    sleep(1);
    ROS_INFO("Stop Log in 2");
    sleep(1);
    ROS_INFO("Stop Log in 1");
    sleep(1);

    ur_logger.stop();
/*
    // experiment hold and linear move
    ROS_INFO("Starting next experiment in 10 Seconds");
    sleep(7);
    ROS_INFO("Starting Log in 3");
    sleep(1);
    ROS_INFO("Starting Log in 2");
    sleep(1);
    ROS_INFO("Starting Log in 1");
    sleep(1);
    ROS_INFO("Starting Log now");

    ROS_INFO("Waiting 5 seconds");
    sleep(5);
#endif
    ROS_INFO("Linear Move up");
    dualArmRobot.linearMoveParallel(linear_move_direction, "box1", 0.5);

#ifndef OFFLINE
    ROS_INFO("Waiting 5 seconds");
    sleep(2);
    ROS_INFO("Stop Log in 3");
    sleep(1);
    ROS_INFO("Stop Log in 2");
    sleep(1);
    ROS_INFO("Stop Log in 1");
    sleep(1);
    ur_logger.stop();
#endif
*/
    // place box
    /*
    geometry_msgs::Vector3Stamped close_direction;
    close_direction.vector.z = -direction.vector.z;
    dualArmRobot.placeBox("box1", place_pose_left, close_direction.vector);*/

    // END
    ROS_INFO("Finished demonstration");
    ros::shutdown();
    return 0;
}