#include "info_server_impl.h"

namespace mavsdk {

InfoServerImpl::InfoServerImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

InfoServerImpl::InfoServerImpl(std::shared_ptr<System> system) : PluginImplBase(std::move(system))
{
    _parent->register_plugin(this);
}

InfoServerImpl::~InfoServerImpl()
{
    _parent->unregister_plugin(this);
}

void InfoServerImpl::init()
{
    _parent->register_mavlink_command_handler(
        MAV_CMD_REQUEST_MESSAGE,
        [this](const MavlinkCommandReceiver::CommandLong& command) {
            return process_command_request_message(command);
        },
        this);

    _parent->register_mavlink_command_handler(
        MAV_CMD_REQUEST_AUTOPILOT_CAPABILITIES,
        [this](const MavlinkCommandReceiver::CommandLong& command) {
            return process_command_request_autopilot_capabilities(command);
        },
        this);

    _parent->register_mavlink_command_handler(
        MAV_CMD_REQUEST_PROTOCOL_VERSION,
        [this](const MavlinkCommandReceiver::CommandLong& command) {
            return process_command_request_protocol_version(command);
        },
        this);
}

void InfoServerImpl::deinit() {}

void InfoServerImpl::enable() {}

void InfoServerImpl::disable() {}

std::pair<InfoServer::Result, InfoServer::AutopilotVersion>
InfoServerImpl::provide_autopilot_version()
{
    return {InfoServer::Result::Success, _autopilot_version};
}

std::pair<InfoServer::Result, InfoServer::ProtocolVersion>
InfoServerImpl::provide_protocol_version()
{
    return {InfoServer::Result::Success, _protocol_version};
}

mavlink_message_t
InfoServerImpl::process_command_request_message(const MavlinkCommandReceiver::CommandLong& command)
{
    int msg_id = static_cast<int>(command.params.param1);
    if (msg_id == MAVLINK_MSG_ID_AUTOPILOT_VERSION) {
        send_autopilot_version();
        return _parent->make_command_ack_message(command, MAV_RESULT::MAV_RESULT_ACCEPTED);
    }

    if (msg_id == MAVLINK_MSG_ID_PROTOCOL_VERSION) {
        send_protocol_version();
        return _parent->make_command_ack_message(command, MAV_RESULT::MAV_RESULT_ACCEPTED);
    }

    return _parent->make_command_ack_message(command, MAV_RESULT::MAV_RESULT_DENIED);
}

mavlink_message_t InfoServerImpl::process_command_request_autopilot_capabilities(
    const MavlinkCommandReceiver::CommandLong& command)
{
    _parent->send_flight_information_request();
    return _parent->make_command_ack_message(command, MAV_RESULT::MAV_RESULT_ACCEPTED);
}

mavlink_message_t InfoServerImpl::process_command_request_protocol_version(
    const MavlinkCommandReceiver::CommandLong& command)
{
    return _parent->make_command_ack_message(command, MAV_RESULT::MAV_RESULT_ACCEPTED);
}

void InfoServerImpl::send_protocol_version() const
{
    std::lock_guard<std::mutex> lock(_protocol_version_mutex);

    mavlink_message_t msg;
    mavlink_msg_protocol_version_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &msg,
        _protocol_version.version,
        _protocol_version.min_version,
        _protocol_version.max_version,
        reinterpret_cast<const uint8_t*>(_protocol_version.spec_version_hash.data()),
        reinterpret_cast<const uint8_t*>(_protocol_version.library_version_hash.data()));

    _parent->send_message(msg);
}

void InfoServerImpl::send_autopilot_version() const
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);

    mavlink_message_t msg;
    mavlink_msg_autopilot_version_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &msg,
        _autopilot_version.capabilities,
        _autopilot_version.flight_sw_version,
        _autopilot_version.middleware_sw_version,
        _autopilot_version.os_sw_version,
        _autopilot_version.board_version,
        reinterpret_cast<const uint8_t*>(_autopilot_version.flight_custom_version.data()),
        reinterpret_cast<const uint8_t*>(_autopilot_version.middleware_custom_version.data()),
        reinterpret_cast<const uint8_t*>(_autopilot_version.os_custom_version.data()),
        _autopilot_version.vendor_id,
        _autopilot_version.product_id,
        _autopilot_version.uid,
        reinterpret_cast<const uint8_t*>(_autopilot_version.uid2.data()));

    _parent->send_message(msg);
}

InfoServer::Result InfoServerImpl::set_autopilot_version(const InfoServer::AutopilotVersion& autopilot_version)
{
    std::lock_guard<std::mutex> lock(_autopilot_version_mutex);
    _autopilot_version = autopilot_version;
    return InfoServer::Result::Success;
}

InfoServer::Result InfoServerImpl::set_protocol_version(const InfoServer::ProtocolVersion& protocol_version)
{
    std::lock_guard<std::mutex> lock(_protocol_version_mutex);
    _protocol_version = protocol_version;
    return InfoServer::Result::Success;
}

} // namespace mavsdk