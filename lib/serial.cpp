#include "sabertooth.hpp"
#include <algorithm>
#include <numeric>
#include <vector>

namespace rml {
SerialOutput::SerialOutput()
  : io_()
  , port_if_(io_)
  , port_d_{}
{}
void
SerialOutput::open(std::string const& port_d)
{
  port_d_ = port_d;
  port_if_.open(port_d);
  port_if_.set_option(boost::asio::serial_port_base::baud_rate(9600));
}
SerialOutput::ReturnType
SerialOutput::send(std::vector<unsigned char> const& payload)
{
  boost::asio::write(port_if_, boost::asio::buffer(payload));
}
}
