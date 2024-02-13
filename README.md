## Installation

Mavlink is a header only library, which specifies mavlink message types and other useful functions. Therefore it does not need building and it is copied into system headers directory.
### Mavlink install
```bash
git clone https://github.com/mavlink/c_library_v2.git
cd c_library_v2/
sudo mkdir /usr/local/include/mavlink
sudo cp -r . /usr/local/include/mavlink/
```

### mavlink-serial-sys-id-set install
```bash
git clone https://gitlab.airvolute.com/sw/linux/app/mavlink-serial-sys-id-set.git
cd mavlink-serial-sys-id-set
mkdir build
cd build
cmake ..
make
sudo make install
```

### Execute script
Run in terminal
```bash
cd path/serial_setup/mavlink-serial-sys-id-set/build
./mavlink_sys_id_set <serial_port> <baud_rate> <new_sys_id> <target_sys_id> <target_component_id>
```
For example:
```bash
./mavlink_sys_id_set /dev/ttyTHS0 921600 5 1 0
```

