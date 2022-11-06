#include "sabertooth.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>

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
PacketizedSerialConfig::PacketizedSerialConfig() : last_cmd(4) {
  cmd_[0][0] = 0x01; // Drive motor 1 backwards
  cmd_[0][1] = 0x00; // Drive motor 1 forwards
  cmd_[1][0] = 0x04; // Drive motor 2 backwards
  cmd_[1][1] = 0x05; // Drive motor 2 forwards
}
std::vector<unsigned char> const&
PacketizedSerialConfig::construct_command(unsigned char addr,
                                          unsigned char motor_index,
                                          int motor_spd)
{
  last_cmd[0] = addr;
  last_cmd[1] = cmd_[motor_index][motor_spd>128];
  last_cmd[2] = motor_spd/2;
  last_cmd[3] = std::accumulate(last_cmd.begin(), last_cmd.end()-1,0) & 0x7F;
  return last_cmd;
}
std::vector<unsigned char> const&
PacketizedSerialConfig::greet() {
  std::fill(last_cmd.begin(), last_cmd.end(), 0);
  last_cmd[3] = GREET_CODE;
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
