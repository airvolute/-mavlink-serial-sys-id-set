#ifndef MAVLINK_SERIAL_PARAM_SETTER_HPP_
#define MAVLINK_SERIAL_PARAM_SETTER_HPP_

#include <mavlink/common/mavlink.h>
#include <boost/asio.hpp>
#include <string>
#include <mutex>

class MavlinkSerialParamSet {
public:
    MavlinkSerialParamSet(const std::string& port, uint32_t baud_rate);
    ~MavlinkSerialParamSet();

    void Start();
    void Stop();
    void SendParamSet(const char* param_id, float param_value, uint8_t system_id, uint8_t component_id);

private:
    void Disconnect();

    boost::asio::io_service io_service_;
    boost::asio::serial_port serial_port_;
    std::mutex write_mutex_;
};

#endif  // MAVLINK_SERIAL_PARAM_SETTER_HPP_
