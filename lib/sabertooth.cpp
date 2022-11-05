#include "sabertooth.hpp"
#include <stdexcept>
#include <algorithm>

namespace rml {
std::vector<unsigned char> const&
SimplifiedSerialConfig::construct_command(
  unsigned int motor_index,
  int motor_spd)
{
  if (motor_index == 0)
  last_cmd[0] = std::clamp(int(motor_spd/2), 1, 127);
  else if (motor_index == 1)
  last_cmd[1] = std::clamp(int(motor_spd/2), 128, 255);
  else
    throw std::runtime_error("Cannot command motor at out of range index");
  return last_cmd;
}
int
LinearInterpolation::transform(float speed)
{
  return std::clamp(int(speed * 127 + 128), 1, 255);
}
void SerialOutput::send() {}
}
