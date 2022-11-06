#pragma once
#include <vector>
namespace rml {
class SerialModeConfig
{
public:
  std::vector<unsigned char> last_cmd;
  std::vector<unsigned char> const& construct_command(int motor1_spd,
                                                      int motor2_spd);
  SerialModeConfig();

protected:
  ~SerialModeConfig() {}
};
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
class SmartElex
  : public MotorConfig
  , public Interpolation
  , public OutputInterface
{
public:
  SmartElex(){};
  typename OutputInterface::ReturnType command(float motor1_spd,
                                               float motor2_spd);
};
}
