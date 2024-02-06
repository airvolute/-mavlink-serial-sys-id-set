## Installation

Mavlink is a header only library, which specifies mavlink message types and other useful functions. Therefore it does not need building and it is copied into system headers directory.
### Mavlink install
```bash
git clone https://github.com/mavlink/c_library_v2.git
cd c_library_v2/
sudo mkdir /usr/local/include/mavlink
sudo cp -r . /usr/local/include/mavlink/
```

### MavlinkUdpInterface Installation
```bash
cd mavlink-serial-comm-lib
mkdir build
cd build
cmake ..
make
sudo make install
```

### Execute script
Run in terminal
```bash
cd path/serial_setup/mavlink-serial-comm/build
sudo ./mavlink_serial_sender
```
Location of Cube: /dev/ttyTHS0
Baud rate: 921600

Parameter ID: SYSID_THISMAV
Parameter Value: 1.0f
Target ID: 1
Component ID: 1

MavlinkSerialInterface sender("/dev/ttyTHS0", 921600);
sender.SendParamSet("SYSID_THISMAV", 1.0f, 1, 1);

### Mavlink message
| Field Name        | Type         | Values          | Description |
|-------------------|--------------|-----------------|-------------|
| target_system     | uint8_t      |                 | System ID   |
| target_component  | uint8_t      |                 | Component ID|
| param_id          | char[16]     |                 | Onboard parameter id, terminated by NULL if the length is less than 16 human-readable chars and WITHOUT null termination (NULL) byte if the length is exactly 16 chars - applications have to provide 16+1 bytes storage if the ID is stored as string |
| param_value       | float        |                 | Onboard parameter value |
| param_type        | uint8_t      | MAV_PARAM_TYPE  | Onboard parameter type. |

