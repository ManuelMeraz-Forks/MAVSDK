#include "server_component_impl.h"
#include "server_plugin_impl_base.h"
#include "mavsdk_impl.h"

namespace mavsdk {

ServerComponentImpl::ServerComponentImpl(MavsdkImpl& mavsdk_impl, uint8_t component_id) :
    _mavsdk_impl(mavsdk_impl),
    _mavlink_command_receiver(mavsdk_impl),
    _own_component_id(component_id)
{}

ServerComponentImpl::~ServerComponentImpl() {}

void ServerComponentImpl::register_plugin(ServerPluginImplBase* server_plugin_impl)
{
    // This is a bit pointless now but just mirrors what is done for the client plugins.
    server_plugin_impl->init();
}

void ServerComponentImpl::unregister_plugin(ServerPluginImplBase* server_plugin_impl)
{
    // This is a bit pointless now but just mirrors what is done for the client plugins.
    server_plugin_impl->deinit();
}

void ServerComponentImpl::register_mavlink_command_handler(
    uint16_t cmd_id,
    const MavlinkCommandReceiver::MavlinkCommandIntHandler& callback,
    const void* cookie)
{
    _mavlink_command_receiver.register_mavlink_command_handler(cmd_id, callback, cookie);
}

void ServerComponentImpl::register_mavlink_command_handler(
    uint16_t cmd_id,
    const MavlinkCommandReceiver::MavlinkCommandLongHandler& callback,
    const void* cookie)
{
    _mavlink_command_receiver.register_mavlink_command_handler(cmd_id, callback, cookie);
}

void ServerComponentImpl::unregister_mavlink_command_handler(uint16_t cmd_id, const void* cookie)
{
    _mavlink_command_receiver.unregister_mavlink_command_handler(cmd_id, cookie);
}

void ServerComponentImpl::unregister_all_mavlink_command_handlers(const void* cookie)
{
    _mavlink_command_receiver.unregister_all_mavlink_command_handlers(cookie);
}

uint8_t ServerComponentImpl::get_own_system_id() const
{
    return _mavsdk_impl.get_own_system_id();
}

void ServerComponentImpl::set_own_component_id(uint8_t own_component_id)
{
    _own_component_id = own_component_id;
}
uint8_t ServerComponentImpl::get_own_component_id() const
{
    return _own_component_id;
}

Time& ServerComponentImpl::get_time()
{
    return _mavsdk_impl.time;
}

bool ServerComponentImpl::send_message(mavlink_message_t& message)
{
    return _mavsdk_impl.send_message(message);
}

void ServerComponentImpl::add_call_every(
    std::function<void()> callback, float interval_s, void** cookie)
{
    _mavsdk_impl.call_every_handler.add(
        std::move(callback), static_cast<double>(interval_s), cookie);
}

void ServerComponentImpl::change_call_every(float interval_s, const void* cookie)
{
    _mavsdk_impl.call_every_handler.change(static_cast<double>(interval_s), cookie);
}

void ServerComponentImpl::reset_call_every(const void* cookie)
{
    _mavsdk_impl.call_every_handler.reset(cookie);
}

void ServerComponentImpl::remove_call_every(const void* cookie)
{
    _mavsdk_impl.call_every_handler.remove(cookie);
}

mavlink_message_t ServerComponentImpl::make_command_ack_message(
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

mavlink_message_t ServerComponentImpl::make_command_ack_message(
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

} // namespace mavsdk
