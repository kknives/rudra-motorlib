#pragma once
#include "sabertooth.hpp"
#include <boost/crc.hpp>

namespace rml {
/// @brief Command interface for multiple drivers on the same serial bus.
///
/// This BasicMicro specific config can be used to command multiple (upto 8
/// according to the datasheet) drivers listening on the same serial bus. Each
/// driver must be configured with a unique address to command it.
///
/// Please note that the SabertoothPacketizedConfig is incompatible with
/// BasicMicro. Use this class instead of SabertoothPacketizedConfig.
class BasicMicroPacketizedConfig
{
public:
  /// The last command constructed.
  /// This may differ from the last command which was sent, which is dependent
  /// on the OutputInterface.
  std::vector<unsigned char> last_cmd;
  /// @param addr The address of the motor driver to command. Specified by the
  /// datasheet to have values > 128.
  /// @param motor_index The motor to command, should be either 0 or 1.
  /// @param motor_spd The motor speed in the intermediate representation (an
  /// integer between 0 and 255)
  /// @returns The raw byte values representing the command, which can be
  /// directly sent to the driver.
  std::vector<unsigned char> const& construct_command(unsigned char addr,
                                                      unsigned char motor_index,
                                                      int motor_spd)
  {
    last_cmd[0] = addr;
    last_cmd[1] = cmd_[motor_index][motor_spd > 128];
    last_cmd[2] = motor_spd / 2;
    crc_ccitt2 =
      std::for_each(last_cmd.begin(), last_cmd.begin() + 3, crc_ccitt2);
    unsigned int crc_res = crc_ccitt2();
    last_cmd[3] = crc_res >> 8;
    last_cmd[4] = crc_res & 0xFF;
    return last_cmd;
  }
  BasicMicroPacketizedConfig()
    : last_cmd(5)
  {

    cmd_[0][0] = 0x01; // Drive motor 1 backwards
    cmd_[0][1] = 0x00; // Drive motor 1 forwards
    cmd_[1][0] = 0x04; // Drive motor 2 backwards
    cmd_[1][1] = 0x05; // Drive motor 2 forwards
  }

private:
  unsigned char cmd_[2][2];
  boost::crc_optimal<16, 0x1021, 0xFFFF, 0, false, false> crc_ccitt2;

protected:
  ~BasicMicroPacketizedConfig() {}
};

/// @brief Operate on a BasicMicro RoboClaw driver.
///
/// Valid MotorConfig: SimplifiedSerialConfig, BasicMicroPacketizedConfig
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
class BasicMicroRoboClaw
  : public MotorConfig
  , public Interpolation
  , public OutputInterface
{
public:
  BasicMicroRoboClaw(){};
  /// @brief Construct and send a command to the driver.
  ///
  /// @param addr Address of the driver to command.
  /// @param motor_index The motor to command, should be either 0 or 1.
  /// @param speed The speed in range [-1,1] both inclusive.
  /// @returns Returned data from OutputInterface::send(), exactly.
  typename OutputInterface::ReturnType command(unsigned char addr,
                                               unsigned char motor_index,
                                               float motor_spd)
  {
    int interm_rep_spd = Interpolation::transform(motor_spd);
    auto cmd =
      MotorConfig::construct_command(addr, motor_index, interm_rep_spd);
    return OutputInterface::send(cmd);
  }
  typename OutputInterface::ReturnType command_send()
  {
    return OutputInterface::send(MotorConfig::last_cmd);
  }
};
}
