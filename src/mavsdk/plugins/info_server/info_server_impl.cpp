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
    //    _parent->register_mavlink_command_handler(
    //        MAV_CMD_REQUEST_MESSAGE,
    //        [this](const MavlinkCommandReceiver::CommandLong& command) {
    //            return handle_command_long(command);
    //        },
    //        this);
    //    _parent->register_mavlink_command_handler(
    //        mavlink::minimal,
    //        [this](const mavlink_message_t& message) { process_autopilot_version(message); },
    //        this);

    _parent->register_mavlink_message_handler(
        MAV_CMD_REQUEST_AUTOPILOT_CAPABILITIES,
        [this](const mavlink_message_t& message) { process_flight_information(message); },
        this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_ATTITUDE,
        [this](const mavlink_message_t& message) { process_attitude(message); },
        this);
}

void InfoServerImpl::deinit() {}

void InfoServerImpl::enable() {}

void InfoServerImpl::disable() {}

std::pair<InfoServer::Result, InfoServer::FlightInfo> InfoServerImpl::provide_flight_information()
{
    // TODO :)

    return {};
}

void InfoServerImpl::process_autopilot_version(const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_autopilot_version_t autopilot_version;
    mavlink_msg_autopilot_version_decode(&message, &autopilot_version);

    _version.flight_sw_major = (autopilot_version.flight_sw_version >> (8 * 3)) & 0xFF;
    _version.flight_sw_minor = (autopilot_version.flight_sw_version >> (8 * 2)) & 0xFF;
    _version.flight_sw_patch = (autopilot_version.flight_sw_version >> (8 * 1)) & 0xFF;

    // first three bytes of flight_custom_version (little endian) describe vendor version
    _version.flight_sw_git_hash = swap_and_translate_binary_to_str(
        autopilot_version.flight_custom_version + 3,
        sizeof(autopilot_version.flight_custom_version) - 3);

    _version.flight_sw_vendor_major = autopilot_version.flight_custom_version[2];
    _version.flight_sw_vendor_minor = autopilot_version.flight_custom_version[1];
    _version.flight_sw_vendor_patch = autopilot_version.flight_custom_version[0];

    _version.os_sw_major = (autopilot_version.os_sw_version >> (8 * 3)) & 0xFF;
    _version.os_sw_minor = (autopilot_version.os_sw_version >> (8 * 2)) & 0xFF;
    _version.os_sw_patch = (autopilot_version.os_sw_version >> (8 * 1)) & 0xFF;

    // Debug() << "flight version: "
    //     << _version.flight_sw_major
    //     << "."
    //     << _version.flight_sw_minor
    //     << "."
    //     << _version.flight_sw_patch;

    // Debug() << "os version: "
    //     << _version.os_sw_major
    //     << "."
    //     << _version.os_sw_minor
    //     << "."
    //     << _version.os_sw_patch;

    _version.os_sw_git_hash = swap_and_translate_binary_to_str(
        autopilot_version.os_custom_version, sizeof(autopilot_version.os_custom_version));

    _product.vendor_id = autopilot_version.vendor_id;
    _product.vendor_name = vendor_id_str(autopilot_version.vendor_id);

    _product.product_id = autopilot_version.product_id;
    _product.product_name = product_id_str(autopilot_version.product_id);

    _identification.hardware_uid =
        translate_binary_to_str(autopilot_version.uid2, sizeof(autopilot_version.uid2));

    _identification.legacy_uid = autopilot_version.uid;

    _information_received = true;
}

void InfoImpl::process_flight_information(const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_mutex);

    mavlink_flight_information_t flight_information;
    mavlink_msg_flight_information_decode(&message, &flight_information);

    _flight_info.time_boot_ms = flight_information.time_boot_ms;
    _flight_info.flight_uid = flight_information.flight_uuid;

    _flight_information_received = true;
}

std::pair<InfoServer::Result, InfoServer::Identification> InfoServerImpl::provide_identification()
{
    // TODO :)
    return {};
}

std::pair<InfoServer::Result, InfoServer::Product> InfoServerImpl::provide_product()
{
    // TODO :)
    return {};
}

std::pair<InfoServer::Result, InfoServer::Version> InfoServerImpl::provide_version()
{
    // TODO :)
    return {};
}

std::pair<InfoServer::Result, double> InfoServerImpl::provide_speed_factor()
{
    // TODO :)
    return {};
}

} // namespace mavsdk