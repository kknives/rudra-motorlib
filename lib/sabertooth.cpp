#include "sabertooth.hpp"
#include <stdexcept>
#include <algorithm>

namespace rml {
std::vector<unsigned char> const&
SimplifiedSerialConfig::construct_command(unsigned char,
                                          unsigned char motor_index,
                                          int motor_spd)
{
  if (motor_index == 0)
    last_cmd[0] = std::clamp(int(motor_spd / 2), 1, 127);
  else if (motor_index == 1)
    last_cmd[1] = std::clamp(int(motor_spd / 2), 128, 255);
  else
    throw std::runtime_error("Cannot command motor at out of range index");
  return last_cmd;
}
int
LinearInterpolation::transform(float speed)
{
  return std::clamp(int(speed * 127 + 128), 1, 255);
}
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
typename OutputInterface::ReturnType
Sabertooth<MotorConfig, Interpolation, OutputInterface>::command(
  unsigned char addr,
  unsigned char motor_index,
  float motor_spd)
{
  int interm_rep_spd = Interpolation::transform(motor_spd);
  auto cmd = MotorConfig::construct_command(addr, motor_index, interm_rep_spd);
  return OutputInterface::send(cmd);
}
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
typename OutputInterface::ReturnType
Sabertooth<MotorConfig, Interpolation, OutputInterface>::command_send()
{
  return OutputInterface::send(MotorConfig::last_cmd);
}
}
