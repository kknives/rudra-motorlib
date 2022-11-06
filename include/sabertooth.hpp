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
class ReturnOutput {
  public:
    typedef std::vector<unsigned char> ReturnType;
    ReturnType const& send(std::vector<unsigned char> const& payload) {
      return payload;
    }
  protected:
    ~ReturnOutput() {}
};
class SerialOutput
{
public:
  typedef void ReturnType;
  ReturnType send(std::vector<unsigned char> const& payload);

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
  Sabertooth() {};
  typename OutputInterface::ReturnType command(int motor_index, float motor_spd);
  void command_send();
};
typedef Sabertooth<SimplifiedSerialConfig, LinearInterpolation, SerialOutput> BasicSabertooth;
}
