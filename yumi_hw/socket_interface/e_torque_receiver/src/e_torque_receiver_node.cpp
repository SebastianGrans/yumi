#include <e_torque_receiver/e_torque_receiver.hpp>

std::shared_ptr<socket_interface::ETorqueReceiver> etorque_receiver;

// Ctr+C handler
void signal_callback_handler(int signum)
{
  std::cout << "Caught signal " << signum << std::endl;
  // Stop streams
  etorque_receiver->stop_stream();
  // Disconenct
  etorque_receiver->disconnect();
  // Terminate ros node
  rclcpp::shutdown();
  // Terminate script
  exit(signum);
}

int main(int argc, char *argv[])
{
  // Ctrl+C handler
  signal(SIGINT, signal_callback_handler);

  rclcpp::init(argc, argv);
  etorque_receiver = std::make_shared<socket_interface::ETorqueReceiver>("e_torque_receiver", "192.168.125.1", 2020);

  std::cout << "before connect()" << std::endl;
  int num_retries = 10;
  if (!etorque_receiver->establish_connection(num_retries))
  {
    std::cout << "[ERROR] connect() failed after " << num_retries << " retries." << std::endl;
    return -1;
  }

  std::cout << "before start_stream()" << std::endl;
  etorque_receiver->start_stream(false);


  while (1)
  {
    sleep(1);
  }
  return 0;
}