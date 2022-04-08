#pragma once

#include <cstdint>
#include "mavlink_include.h"
#include "mavlink_command_receiver.h"
#include "mavsdk_time.h"

namespace mavsdk {

class MavsdkImpl;

class ServerComponentImpl {
public:
    explicit ServerComponentImpl(MavsdkImpl& parent);
    ~ServerComponentImpl();

    void register_mavlink_command_handler(
        uint16_t cmd_id,
        const MavlinkCommandReceiver::MavlinkCommandIntHandler& callback,
        const void* cookie);
    void register_mavlink_command_handler(
        uint16_t cmd_id,
        const MavlinkCommandReceiver::MavlinkCommandLongHandler& callback,
        const void* cookie);
    void unregister_mavlink_command_handler(uint16_t cmd_id, const void* cookie);
    void unregister_all_mavlink_command_handlers(const void* cookie);

    uint8_t get_own_system_id() const;

    void set_own_component_id(uint8_t own_component_id);
    uint8_t get_own_component_id() const;

    Time& get_time();

    bool send_message(mavlink_message_t& message);

    void add_call_every(std::function<void()> callback, float interval_s, void** cookie);
    void change_call_every(float interval_s, const void* cookie);
    void reset_call_every(const void* cookie);
    void remove_call_every(const void* cookie);

    mavlink_message_t
    make_command_ack_message(const MavlinkCommandReceiver::CommandLong& command, MAV_RESULT result);
    mavlink_message_t
    make_command_ack_message(const MavlinkCommandReceiver::CommandInt& command, MAV_RESULT result);

private:
    MavsdkImpl& _mavsdk_impl;
    MavlinkCommandReceiver _mavlink_command_receiver;
    uint8_t _own_component_id{MAV_COMP_ID_AUTOPILOT1};
};

} // namespace mavsdk
