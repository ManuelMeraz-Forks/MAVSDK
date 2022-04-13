#pragma once

#include "mavlink_include.h"
#include "mavlink_command_receiver.h"
#include "mavsdk_time.h"
#include "flight_mode.h"

#include <atomic>
#include <cstdint>

namespace mavsdk {

class MavsdkImpl;
class ServerPluginImplBase;

class ServerComponent {
public:
    ServerComponent(MavsdkImpl& mavsdk_impl, uint8_t component_id);
    ~ServerComponent();

    void register_plugin(ServerPluginImplBase* server_plugin_impl);
    void unregister_plugin(ServerPluginImplBase* server_plugin_impl);

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

    [[nodiscard]] uint8_t get_own_system_id() const;

    void set_own_component_id(uint8_t own_component_id);
    [[nodiscard]] uint8_t get_own_component_id() const;

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

    void send_heartbeat();

    void set_system_status(uint8_t system_status);
    [[nodiscard]] uint8_t get_system_status() const;
    void set_base_mode(uint8_t base_mode);
    [[nodiscard]] uint8_t get_base_mode() const;
    void set_custom_mode(uint32_t custom_mode);
    [[nodiscard]] uint32_t get_custom_mode() const;

    void call_user_callback_located(
        const std::string& filename, const int linenumber, const std::function<void()>& func);

private:
    MavsdkImpl& _mavsdk_impl;
    MavlinkCommandReceiver _mavlink_command_receiver;
    uint8_t _own_component_id{MAV_COMP_ID_AUTOPILOT1};

    std::atomic<uint8_t> _system_status {0};
    std::atomic<uint8_t> _base_mode {0};
    std::atomic<uint32_t> _custom_mode {0};
};

} // namespace mavsdk
