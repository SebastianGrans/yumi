#include <abb_robot_manager/yumi_robot_manager_sim.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>


auto robot_manager = std::make_shared<abb_robot_manager::YumiRobotManager>(
    "robot_manager",
    "192.168.125.1");


void signal_callback_handler(int signum ) 
{
  std::cout << "Caught signal " << signum << std::endl;
  // Terminate ros node
  rclcpp::shutdown();
  // Terminate program
  exit(signum);
}


int main(int argc, char* argv[])
{

  // Ctrl+C handler
  signal(SIGINT, signal_callback_handler);

  rclcpp::init(argc, argv);
  
  
  // Initialize the state machine
  printf("\n---------- Initialization started ----------\n");
  if(!robot_manager->init())
  {
    printf("Failed to initialize the robot manager\n");
    return -1;
  }
  printf("---------- Initialization Complete ----------\n\n");
  usleep(2*1000000);



  // Start the state machine
  printf("\n---------- Starting StateMachine ----------\n");
  if(!robot_manager->start_state_machine())
  {
    printf("Failed to start the state machine\n");
    return -1;
  }
  printf("---------- StateMachine Started ----------\n\n");
  usleep(2*1000000);



  // Test SmartGrippers
  printf("\n---------- Starting Setup Tests ----------\n");
  if(!robot_manager->run_setup_tests())
   {
    printf("Failed to test the SmartGrippers\n");
    return -1;
  }
  printf("---------- Setup Tests Completed ----------\n\n");




  
  // Keep node alive
  robot_manager->spin();
  while(1)
  {
    usleep(5*1000000);
  }


  return 0;
}