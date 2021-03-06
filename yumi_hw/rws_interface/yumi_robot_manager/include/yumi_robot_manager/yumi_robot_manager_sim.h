// Copyright 2020 Markus Bjønnes and Marius Nilsen.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rcutils/logging_macros.h>
#include <yumi_robot_manager/visibility_control.h>
#include <yumi_robot_manager_interfaces/srv/stop_egm.hpp>
#include <yumi_robot_manager_interfaces/srv/start_egm.hpp>
#include <yumi_robot_manager_interfaces/srv/is_ready.hpp>
#include <yumi_robot_manager_interfaces/srv/stop_motors.hpp>

namespace yumi_robot_manager
{
  using StopEgm  = yumi_robot_manager_interfaces::srv::StopEgm;
  using StartEgm = yumi_robot_manager_interfaces::srv::StartEgm;
  using IsReady  = yumi_robot_manager_interfaces::srv::IsReady;
  using StopMotors = yumi_robot_manager_interfaces::srv::StopMotors;

class YumiRobotManager
{
public:
  YUMI_ROBOT_MANAGER_PUBLIC
  YumiRobotManager(const std::string &name, const std::string &ip_address);
  
  YUMI_ROBOT_MANAGER_PUBLIC
  bool init();

  YUMI_ROBOT_MANAGER_PUBLIC
  bool start_state_machine();

  YUMI_ROBOT_MANAGER_PUBLIC
  bool go_to_state(std::string mode);

  YUMI_ROBOT_MANAGER_PUBLIC
  bool configure();  

  YUMI_ROBOT_MANAGER_PUBLIC
  bool stop_egm();

  YUMI_ROBOT_MANAGER_PUBLIC
  void spin();

private:
  std::string name_;
  std::shared_ptr<rclcpp::Node> node_;
  bool first_execution_ = true;
  std::string requested_state_;
  bool is_ready_ = false;

  std::string task_L_ = "T_ROB_L";    
  std::string task_R_ = "T_ROB_R";

  std::string mech_unit_L_ = "ROB_L"; 
  std::string mech_unit_R_ = "ROB_R";

  // Helper functions 
  bool configure_egm();
  bool calibrate_grippers();
  void busy_wait_until_idle();
  void wait_for_gripper_to_finish_motion();

  // Service server
  rclcpp::Service<StopEgm>::SharedPtr stop_egm_srv_;
  rclcpp::Service<StartEgm>::SharedPtr start_egm_srv_;
  rclcpp::Service<IsReady>::SharedPtr is_ready_srv_;
  rclcpp::Service<StopMotors>::SharedPtr stop_motors_srv_;

  void handle_StopEgm(const std::shared_ptr<rmw_request_id_t> request_header,
                      const std::shared_ptr<StopEgm::Request> request,
                      const std::shared_ptr<StopEgm::Response> response);

  void handle_StartEgm(const std::shared_ptr<rmw_request_id_t> request_header,
                       const std::shared_ptr<StartEgm::Request> request,
                       const std::shared_ptr<StartEgm::Response> response);

  void handle_IsReady(const std::shared_ptr<rmw_request_id_t> request_header,
                      const std::shared_ptr<IsReady::Request> request,
                      const std::shared_ptr<IsReady::Response> response);

  void handle_StopMotors(const std::shared_ptr<rmw_request_id_t> request_header,
                         const std::shared_ptr<StopMotors::Request> request,
                         const std::shared_ptr<StopMotors::Response> response);

};

} //namespace yumi_robot_manager
