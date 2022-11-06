#include "smartelex.hpp"
#include <cmath>

namespace rml {
std::vector<unsigned char> const&
SerialModeConfig::construct_command(int motor1_spd, int motor2_spd)
{
  last_cmd[0] = 0x2A;
  last_cmd[1] = ((motor1_spd != 128) << 3) | ((motor1_spd > 128) << 2) |
                ((motor2_spd != 128) << 1) | (motor2_spd > 128);
  last_cmd[2] = std::abs(motor1_spd - 128);
  last_cmd[3] = std::abs(motor2_spd - 128);
  last_cmd[4] = 0xFF;
  return last_cmd;
}
SerialModeConfig::SerialModeConfig()
  : last_cmd(5)
{}

template<typename MotorConfig, typename Interpolation, typename OutputInterface>
typename OutputInterface::ReturnType
SmartElex<MotorConfig, Interpolation, OutputInterface>::command(
  float motor1_spd,
  float motor2_spd)
{
  int interm_spd1 = Interpolation::transform(motor1_spd);
  int interm_spd2 = Interpolation::transform(motor2_spd);
  auto cmd = MotorConfig::construct_command(interm_spd1, interm_spd2);
  return OutputInterface::send(cmd);
}
}
