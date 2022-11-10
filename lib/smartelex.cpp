#include "smartelex.hpp"
#include <cmath>
#include <stdexcept>

namespace rml {
std::vector<unsigned char> const&
SerialModeConfig::construct_multiple(int motor1_spd, int motor2_spd)
{
  last_cmd[0] = 0x2A;
  last_cmd[1] = ((motor1_spd != 128) << 3) | ((motor1_spd > 128) << 2) |
                ((motor2_spd != 128) << 1) | (motor2_spd > 128);
  last_cmd[2] = 2*std::abs(motor1_spd - 128);
  last_cmd[3] = 2*std::abs(motor2_spd - 128);
  last_cmd[4] = 0x23;
  return last_cmd;
}
std::vector<unsigned char> const&
SerialModeConfig::construct_command(unsigned char,
                                    unsigned char motor_index,
                                    int motor_spd)
{
  if (motor_index == 0) {
    last_cmd[0] = 0x2A;
    last_cmd[1] =
      (last_cmd[1]&0x03) | (((motor_spd != 128) << 3) | ((motor_spd > 128) << 2));
    last_cmd[2] = 2*std::abs(motor_spd - 128);
    last_cmd[4] = 0x23;
    return last_cmd;
  } else if (motor_index == 1) {
    last_cmd[0] = 0x2A;
    last_cmd[1] = (last_cmd[1]&0x0C) | (((motor_spd != 128) << 1) | (motor_spd > 128));
    last_cmd[3] = 2*std::abs(motor_spd - 128);
    last_cmd[4] = 0x23;
    return last_cmd;
  } else {
    throw std::runtime_error("Cannot command motor at out of range index");
  }
}
SerialModeConfig::SerialModeConfig()
  : last_cmd(5)
{}

}
