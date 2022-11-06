#pragma once
#include <vector>

namespace rml {
/// @brief The 1-byte command interface for Sabertooth drivers.
///
/// This MotorConfig manages a single Sabertooth driver, connected to the host
/// directly. As opposed to the PacketizedSerialConfig which can command upto 8
/// drivers, all sharing the same serial bus.
class SimplifiedSerialConfig
{
public:
  /// The last command constructed.
  /// This may differ from the last command which was sent, which is dependent
  /// on the OutputInterface.
  std::vector<unsigned char> last_cmd;
  /// @param addr The address of the motor driver to command. For
  /// SimplifiedSerialConfig, this parameter is ignored.
  /// @param motor_index The motor to command, should be either 0 or 1.
  /// @param motor_spd The motor speed in the intermediate representation (an
  /// integer between 0 and 255)
  /// @returns The raw byte values representing the command, which can be
  /// directly sent to the driver.
  std::vector<unsigned char> const& construct_command(unsigned char addr,
                                                      unsigned char motor_index,
                                                      int motor_spd);
  SimplifiedSerialConfig()
    : last_cmd(2)
  {}

protected:
  ~SimplifiedSerialConfig() {}
};

/// @brief Command interface for multiple drivers on the same serial bus.
///
/// This MotorConfig can be used to command multiple (upto 8 according to the
/// datasheet) drivers listening on the same serial bus. Each driver must be
/// configured with a unique address to command it.
class PacketizedSerialConfig
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
                                                      int motor_spd);
  /// Construct a greeting message which is used to identify baudrate being
  /// used. All commands sent before a greet are ignored.
  /// @returns The raw byte values representing the command, which can be
  /// directly sent to the driver.
  std::vector<unsigned char> const& greet();
  PacketizedSerialConfig();

private:
  unsigned char cmd_[2][2];
  unsigned char const GREET_CODE = 0xAA;

protected:
  ~PacketizedSerialConfig() {}
};
/// @brief A linear map from [-1,1] motor speeds.
///
/// Provides no smoothing, so can be used to generate max acceleration.
class LinearInterpolation
{
public:
  /// @param speed The speed in range [-1,1] both inclusive.
  /// @returns The intermediate representation of given speed after
  /// interpolation.
  int transform(float speed);

protected:
  ~LinearInterpolation() {}
};
/// @brief A no-op OutputInterface which simply returns all commands to the
/// user.
///
/// Intended to be used for debugging, testing or extended applications.
/// Does not send any commands over any physical interface.
class ReturnOutput
{
public:
  typedef std::vector<unsigned char> ReturnType;
  /// @param payload The bytes to be sent.
  /// @returns The payload given, without any modifications.
  ReturnType const& send(std::vector<unsigned char> const& payload)
  {
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
/// @brief Operate on a Sabertooth driver.
///
/// Valid MotorConfig: SimplifiedSerialConfig, PacketizedSerialConfig
template<typename MotorConfig, typename Interpolation, typename OutputInterface>
class Sabertooth
  : public MotorConfig
  , public Interpolation
  , public OutputInterface
{
public:
  Sabertooth(){};
  /// @brief Construct and send a command to the driver.
  ///
  /// @param addr Address of the driver to command.
  /// @param motor_index The motor to command, should be either 0 or 1.
  /// @param speed The speed in range [-1,1] both inclusive.
  /// @returns Returned data from OutputInterface::send(), exactly.
  typename OutputInterface::ReturnType command(unsigned char addr,
                                               unsigned char motor_index,
                                               float motor_spd);
  typename OutputInterface::ReturnType command_send();
};
typedef Sabertooth<SimplifiedSerialConfig, LinearInterpolation, SerialOutput>
  BasicSabertooth;
}
