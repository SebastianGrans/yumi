#include <motion_coordinator/motion_coordinator.hpp>

std::shared_ptr<motion_coordinator::MotionCoordinator> yumi_motion_coordinator;

// Ctr+C handler
void signal_callback_handler(int signum)
{
  std::cout << "Caught signal " << signum << std::endl;
  // Terminate EGM session
  yumi_motion_coordinator->terminate_egm_session();
  // Terminate ros node
  rclcpp::shutdown();
  // Terminate program
  exit(signum);
}

void spin(std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> exe)
{
  exe->spin();
}

int main(int argc, char** argv)
{
  // Ctrl+C handler
  signal(SIGINT, signal_callback_handler);
  
  rclcpp::init(argc, argv);

  yumi_motion_coordinator = std::make_shared<motion_coordinator::MotionCoordinator>("motion_coordinator");
  if(!yumi_motion_coordinator->init()) return -1;

  // Spin the moveit2 node countiously in another thread via an executor
  auto executor = std::make_shared<rclcpp::executors::MultiThreadedExecutor>();
  executor->add_node(yumi_motion_coordinator->get_node());
  auto future_handle = std::async(std::launch::async, spin, executor);
  
  if(!yumi_motion_coordinator->activate())
  {
    std::cout << "ERROR: MotionCoordinator failed to activate." << std::endl;
    return -1;  
  }

  std::vector<double> pose =     {0.4,  0.1, -0.1, 30, 0, 0}; 
  std::vector<double> bin_pose = {0.4, -0.1, -0.2, 0, 0, 0 };
  int counter = 0; int retries = 3; double percentage = 1; double speed_scale = 1; double acc_scale = 1;
  

  while(!yumi_motion_coordinator->should_stop())
  {
    yumi_motion_coordinator->move_to_pose("left_arm", {0.4, 0.1, 0.3, 0, 0, 180}, true, 3, false, true, false);
    
    yumi_motion_coordinator->grip_out("left_arm", true);
    yumi_motion_coordinator->grip_out("right_arm", true);

    yumi_motion_coordinator->jog_gripper("left_arm", 0.01, true);
    yumi_motion_coordinator->jog_gripper("right_arm", 0.01, true);

    yumi_motion_coordinator->grip_in("left_arm", true);
    yumi_motion_coordinator->grip_in("right_arm", true);

    yumi_motion_coordinator->move_to_home("left_arm", 3);
  }

  
  std::cout << "Motion completed, please ctrl+c" << std::endl;
  while(1)
  {
    sleep(1);
  }
  return 0;
}
