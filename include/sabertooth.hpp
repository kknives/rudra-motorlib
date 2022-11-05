#pragma once
#include <vector>

namespace rml {
class SimplifiedSerialConfig
{
public:
  std::vector<unsigned char> last_cmd;
  std::vector<unsigned char> const & construct_command(unsigned int motor_index,
                                              int motor_spd);
  SimplifiedSerialConfig() : last_cmd(2) {}

protected:
  ~SimplifiedSerialConfig() {}
};
class LinearInterpolation
{
public:
  int transform(float speed);

protected:
  ~LinearInterpolation() {}
};
class SerialOutput
{
public:
  void send(std::vector<unsigned char> const& payload);

protected:
  ~SerialOutput() {}
};
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
class Sabertooth
  : public MotorConfig
  , public Interpolation
  , public OutputInterface
{
public:
  Sabertooth();
  void command();
  void command_send();
};
}
