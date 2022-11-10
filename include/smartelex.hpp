#pragma once
#include <vector>
namespace rml {
class SerialModeConfig
{
public:
  std::vector<unsigned char> last_cmd;
  std::vector<unsigned char> const& construct_command(unsigned char addr,
                                                      unsigned char motor_index,
                                                      int motor_spd);
  std::vector<unsigned char> const& construct_multiple(int motor1_spd,
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
  typename OutputInterface::ReturnType command(unsigned char addr,
                                               unsigned char motor_index,
                                               float motor_spd)
  {
    int interm_spd = Interpolation::transform(motor_spd);
    auto cmd = MotorConfig::construct_command(addr, motor_index, interm_spd);
    return OutputInterface::send(cmd);
  }
  typename OutputInterface::ReturnType command_multiple(float motor1_spd,
                                                        float motor2_spd)
  {
    int interm_spd1 = Interpolation::transform(motor1_spd);
    int interm_spd2 = Interpolation::transform(motor2_spd);
    auto cmd = MotorConfig::construct_multiple(interm_spd1, interm_spd2);
    return OutputInterface::send(cmd);
  }
};
}
