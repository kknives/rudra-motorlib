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

}
