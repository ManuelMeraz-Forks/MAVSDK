#include "server_component.h"
#include "server_plugin_impl_base.h"
#include "mavsdk_impl.h"

namespace mavsdk {

ServerComponent::ServerComponent(MavsdkImpl& mavsdk_impl, uint8_t component_id) :
    _mavsdk_impl(mavsdk_impl),
    _mavlink_command_receiver(mavsdk_impl),
    _own_component_id(component_id)
{}

ServerComponent::~ServerComponent() {}

void ServerComponent::register_plugin(ServerPluginImplBase* server_plugin_impl)
{
    // This is a bit pointless now but just mirrors what is done for the client plugins.
    server_plugin_impl->init();
}

void ServerComponent::unregister_plugin(ServerPluginImplBase* server_plugin_impl)
{
    // This is a bit pointless now but just mirrors what is done for the client plugins.
    server_plugin_impl->deinit();
}

void ServerComponent::register_mavlink_command_handler(
    uint16_t cmd_id,
    const MavlinkCommandReceiver::MavlinkCommandIntHandler& callback,
    const void* cookie)
{
    _mavlink_command_receiver.register_mavlink_command_handler(cmd_id, callback, cookie);
}

void ServerComponent::register_mavlink_command_handler(
    uint16_t cmd_id,
    const MavlinkCommandReceiver::MavlinkCommandLongHandler& callback,
    const void* cookie)
{
    _mavlink_command_receiver.register_mavlink_command_handler(cmd_id, callback, cookie);
}

void ServerComponent::unregister_mavlink_command_handler(uint16_t cmd_id, const void* cookie)
{
    _mavlink_command_receiver.unregister_mavlink_command_handler(cmd_id, cookie);
}

void ServerComponent::unregister_all_mavlink_command_handlers(const void* cookie)
{
    _mavlink_command_receiver.unregister_all_mavlink_command_handlers(cookie);
}

void ServerComponent::register_mavlink_message_handler(
    uint16_t msg_id, const MavlinkMessageHandler& callback, const void* cookie)
{
    _mavsdk_impl.mavlink_message_handler.register_one(msg_id, callback, cookie);
}

void ServerComponent::register_mavlink_message_handler(
    uint16_t msg_id, uint8_t cmp_id, const MavlinkMessageHandler& callback, const void* cookie)
{
    _mavsdk_impl.mavlink_message_handler.register_one(msg_id, cmp_id, callback, cookie);
}

void ServerComponent::unregister_mavlink_message_handler(uint16_t msg_id, const void* cookie)
{
    _mavsdk_impl.mavlink_message_handler.unregister_one(msg_id, cookie);
}

void ServerComponent::unregister_all_mavlink_message_handlers(const void* cookie)
{
    _mavsdk_impl.mavlink_message_handler.unregister_all(cookie);
}

uint8_t ServerComponent::get_own_system_id() const
{
    return _mavsdk_impl.get_own_system_id();
}

void ServerComponent::set_own_component_id(uint8_t own_component_id)
{
    _own_component_id = own_component_id;
}
uint8_t ServerComponent::get_own_component_id() const
{
    return _own_component_id;
}

Time& ServerComponent::get_time()
{
    return _mavsdk_impl.time;
}

bool ServerComponent::send_message(mavlink_message_t& message)
{
    return _mavsdk_impl.send_message(message);
}

void ServerComponent::add_call_every(
    std::function<void()> callback, float interval_s, void** cookie)
{
    _mavsdk_impl.call_every_handler.add(
        std::move(callback), static_cast<double>(interval_s), cookie);
}

void ServerComponent::change_call_every(float interval_s, const void* cookie)
{
    _mavsdk_impl.call_every_handler.change(static_cast<double>(interval_s), cookie);
}

void ServerComponent::reset_call_every(const void* cookie)
{
    _mavsdk_impl.call_every_handler.reset(cookie);
}

void ServerComponent::remove_call_every(const void* cookie)
{
    _mavsdk_impl.call_every_handler.remove(cookie);
}

mavlink_message_t ServerComponent::make_command_ack_message(
    const MavlinkCommandReceiver::CommandLong& command, MAV_RESULT result)
{
    const uint8_t progress = std::numeric_limits<uint8_t>::max();
    const uint8_t result_param2 = 0;

    mavlink_message_t msg{};
    mavlink_msg_command_ack_pack(
        get_own_system_id(),
        get_own_component_id(),
        &msg,
        command.command,
        result,
        progress,
        result_param2,
        command.origin_system_id,
        command.origin_component_id);
    return msg;
}

mavlink_message_t ServerComponent::make_command_ack_message(
    const MavlinkCommandReceiver::CommandInt& command, MAV_RESULT result)
{
    const uint8_t progress = std::numeric_limits<uint8_t>::max();
    const uint8_t result_param2 = 0;

    mavlink_message_t msg{};
    mavlink_msg_command_ack_pack(
        get_own_system_id(),
        get_own_component_id(),
        &msg,
        command.command,
        result,
        progress,
        result_param2,
        command.origin_system_id,
        command.origin_component_id);
    return msg;
}

void ServerComponent::send_heartbeat()
{
    mavlink_message_t message;
    mavlink_msg_heartbeat_pack(
        get_own_system_id(),
        get_own_component_id(),
        &message,
        _mavsdk_impl.get_mav_type(),
        get_own_component_id() == MAV_COMP_ID_AUTOPILOT1 ? MAV_AUTOPILOT_GENERIC :
                                                           MAV_AUTOPILOT_INVALID,
        get_own_component_id() == MAV_COMP_ID_AUTOPILOT1 ? _base_mode.load() : 0,
        get_own_component_id() == MAV_COMP_ID_AUTOPILOT1 ? _custom_mode.load() : 0,
        get_system_status());
    send_message(message);
}

void ServerComponent::set_system_status(uint8_t system_status)
{
    _system_status = system_status;
}

uint8_t ServerComponent::get_system_status() const
{
    return _system_status;
}

void ServerComponent::set_base_mode(uint8_t base_mode)
{
    _base_mode = base_mode;
}

uint8_t ServerComponent::get_base_mode() const
{
    return _base_mode;
}

void ServerComponent::set_custom_mode(uint32_t custom_mode)
{
    _custom_mode = custom_mode;
}

uint32_t ServerComponent::get_custom_mode() const
{
    return _custom_mode;
}

void ServerComponent::call_user_callback_located(
    const std::string& filename, const int linenumber, const std::function<void()>& func)
{
    _mavsdk_impl.call_user_callback_located(filename, linenumber, func);
}

void ServerComponent::add_capabilities(uint64_t add_capabilities)
{
    std::unique_lock<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.capabilities |= add_capabilities;

    // We need to resend capabilities...
    lock.unlock();
    if (_should_send_autopilot_version) {
        send_autopilot_version();
    }
}

void ServerComponent::set_flight_sw_version(uint32_t flight_sw_version)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.flight_sw_version = flight_sw_version;
}

void ServerComponent::set_middleware_sw_version(uint32_t middleware_sw_version)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.middleware_sw_version = middleware_sw_version;
}

void ServerComponent::set_os_sw_version(uint32_t os_sw_version)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.os_sw_version = os_sw_version;
}

void ServerComponent::set_board_version(uint32_t board_version)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.board_version = board_version;
}

void ServerComponent::set_vendor_id(uint16_t vendor_id)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.vendor_id = vendor_id;
}

void ServerComponent::set_product_id(uint16_t product_id)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version.product_id = product_id;
}

bool ServerComponent::set_uid2(std::string uid2)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    if (uid2.size() > _autopilot_version.uid2.size()) {
        return false;
    }
    _autopilot_version.uid2 = {0};
    std::copy(uid2.begin(), uid2.end(), _autopilot_version.uid2.data());
    return true;
}


void ServerComponent::send_autopilot_version()
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    const uint8_t custom_values[8] = {0}; // TO-DO: maybe?

    mavlink_message_t msg;
    mavlink_msg_autopilot_version_pack(
        _mavsdk_impl.get_own_system_id(),
        get_own_component_id(),
        &msg,
        _autopilot_version.capabilities,
        _autopilot_version.flight_sw_version,
        _autopilot_version.middleware_sw_version,
        _autopilot_version.os_sw_version,
        _autopilot_version.board_version,
        custom_values,
        custom_values,
        custom_values,
        _autopilot_version.vendor_id,
        _autopilot_version.product_id,
        0,
        _autopilot_version.uid2.data());

    _mavsdk_impl.send_message(msg);
}


} // namespace mavsdk
