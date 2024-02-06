#include "mavlink_serial_param_setter.hpp"

#include <iostream>
#include <boost/asio.hpp>


/**
 * @brief Constructs a MavlinkSerialParamSet object.
 * 
 * @param port The serial port to use for communication, Name of a deive file in dev/ in linux.
 * @param baud_rate In this case 921600.
 */
MavlinkSerialParamSet::MavlinkSerialParamSet(const std::string& port, uint32_t baud_rate)
    : io_service_(), serial_port_(io_service_) {
    boost::system::error_code ec;
    serial_port_.open(port, ec);
    if (ec) {
        std::cerr << "Error opening serial port: " << ec.message() << std::endl;
        throw std::runtime_error("Failed to open serial port");
    }
    serial_port_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}

MavlinkSerialParamSet::~MavlinkSerialParamSet() {
    Disconnect();
}

void MavlinkSerialParamSet::Start() {
}

void MavlinkSerialParamSet::Stop() {
    Disconnect();
}

void MavlinkSerialParamSet::SendParamSet(const char* param_id, float param_value, uint8_t system_id, uint8_t component_id) {
    mavlink_message_t message;
    mavlink_param_set_t param_set;

    std::strncpy(param_set.param_id, param_id, MAVLINK_MSG_PARAM_SET_FIELD_PARAM_ID_LEN);
    param_set.param_value = param_value;
    param_set.target_system = system_id;
    param_set.target_component = component_id;
    param_set.param_type = MAV_PARAM_TYPE_REAL32;

    mavlink_msg_param_set_encode(system_id, component_id, &message, &param_set);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t length = mavlink_msg_to_send_buffer(buffer, &message);

    std::lock_guard<std::mutex> lock(write_mutex_);
    boost::asio::write(serial_port_, boost::asio::buffer(buffer, length));
}

void MavlinkSerialParamSet::Disconnect() {
    if (serial_port_.is_open()) {
        serial_port_.close();
    }
}

int main() {
    try {
        MavlinkSerialParamSet sender("/dev/ttyTHS0", 921600);
        sender.SendParamSet("SYSID_THISMAV", 1.0f, 1, 1);
        std::cout << "PARAM_SET message sent." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
