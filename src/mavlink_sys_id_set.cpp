// Copyright 2023 Airvolute, s.r.o.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "mavlink_sys_id_set.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <string>

/**
 * @brief Constructs a MavlinkSerialParamSet object.
 *
 * @param port The serial port to use for communication, Name of a device file
 * in dev/ in Linux.
 * @param baud_rate The baud rate for serial communication.
 * @param sys_id The system ID to set.
 * @param target_system_id The target system ID for the parameter set.
 * @param target_component_id The target component ID for the parameter set.
 */
MavlinkSerialParamSet::MavlinkSerialParamSet(const std::string &port,
                                             uint32_t baud_rate, float sys_id,
                                             uint8_t target_system_id,
                                             uint8_t target_component_id)
    : io_service_(), serial_port_(io_service_) {
  boost::system::error_code ec;
  serial_port_.open(port, ec);
  if (ec) {
    std::cerr << "Error opening serial port: " << ec.message() << std::endl;
    throw std::runtime_error(std::string("Failed to open serial port") + ec.message());
  }
  serial_port_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));

  SetSysId(sys_id, target_system_id, target_component_id);
}

/**
 * @brief Destroys the MavlinkSerialParamSet object and disconnects from the
 * serial port.
 */
MavlinkSerialParamSet::~MavlinkSerialParamSet() { Disconnect(); }

/**
 * @brief Sets the system ID.
 *
 * @param sys_id The system ID to set.
 * @param target_system_id The target system ID for the parameter set.
 * @param target_component_id The target component ID for the parameter set.
 */
void MavlinkSerialParamSet::SetSysId(float sys_id, uint8_t target_system_id,
                                     uint8_t target_component_id) {
  mavlink_message_t message;
  mavlink_param_set_t param_set;

  std::strncpy(param_set.param_id, "SYSID_THISMAV",
               MAVLINK_MSG_PARAM_SET_FIELD_PARAM_ID_LEN);
  param_set.param_value = sys_id;
  param_set.target_system = target_system_id;
  param_set.target_component = target_component_id;
  param_set.param_type = MAV_PARAM_TYPE_INT16;

  mavlink_msg_param_set_encode(target_system_id, target_component_id, &message,
                               &param_set);

  uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
  size_t length = mavlink_msg_to_send_buffer(buffer, &message);

  boost::asio::write(serial_port_, boost::asio::buffer(buffer, length));
  std::cout << "SYS ID set successfully!" << std::endl;
}

/**
 * @brief Disconnects from the serial port.
 */
void MavlinkSerialParamSet::Disconnect() {
  if (serial_port_.is_open()) {
    serial_port_.close();
  }
}

int main(int argc, char *argv[]) {
  if (argc < 6) {
    std::cout << "Usage: " << argv[0]
              << " <serial_port> <baud_rate> <new_sys_id> <target_sys_id> "
                 "<target_component_id>"
              << std::endl;
    return 1;
  }
  try {
    MavlinkSerialParamSet sender(argv[1], std::atoi(argv[2]),
                                 std::atof(argv[3]), std::atoi(argv[4]),
                                 std::atoi(argv[5]));
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return 0;
}
