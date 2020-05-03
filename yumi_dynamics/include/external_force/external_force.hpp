#pragma once

#include <string>
#include <vector>
#include <future>

#include <urdf/model.h>
#include <kdl_wrapper/kdl_wrapper.h>
#include <eigen3/Eigen/Core>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>

class AsyncSpinner
{
public:
  explicit AsyncSpinner(std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor) : executor_(executor)
  {
  }
  void async_spin()
  {
    handle_ = std::async(std::launch::async, spin, executor_);
  }

private:
  static void spin(std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> exe)
  {
    exe->spin();
  }

  std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor_;
  std::future<void> handle_;
};

namespace yumi_dynamics
{
class ExternalForce // : public rclcpp::Node
{
public:
  ExternalForce(urdf::Model);
  void estimate_TCP_wrench();
  std::shared_ptr<rclcpp::Node> get_force_node();

private:
  KdlWrapper kdl_wrapper_;
  std::vector<float> q_l_;
  std::vector<float> q_r_;
  std::vector<float> q_dot_l_;
  std::vector<float> q_dot_r_;

  Eigen::MatrixXd torques_l_;
  Eigen::MatrixXd torques_r_;

  Eigen::MatrixXd ext_torques_l_;
  Eigen::MatrixXd ext_torques_r_;

  int joints_r_;
  int joints_l_;

  KDL::Jacobian jacobian_l_;
  KDL::Jacobian jacobian_r_;

  std::shared_ptr<rclcpp::Node> ext_force_node_;
  std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::JointState>> joint_state_sub_;
  std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::JointState>> ext_torque_sub_;

  std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::WrenchStamped>> wrench_pub_r_;
  std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::WrenchStamped>> wrench_pub_l_;
  std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> exec_;
  std::shared_ptr<AsyncSpinner> async_spinner_;

  void joint_state_callback(sensor_msgs::msg::JointState::UniquePtr jnt_msg);
  void external_torques_callback(sensor_msgs::msg::JointState::UniquePtr jnt_msg);
  void torques_callback(sensor_msgs::msg::JointState::UniquePtr jnt_msg);
  void populate_wrench_msg(std::string mech_unit, geometry_msgs::msg::WrenchStamped &wrench_msg, Eigen::Matrix<double, 6, 1> &wrench);
};
} // namespace yumi_dynamics