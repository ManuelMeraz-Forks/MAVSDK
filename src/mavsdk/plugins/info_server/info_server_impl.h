#pragma once

#include "plugins/info_server/info_server.h"
#include "plugin_impl_base.h"

namespace mavsdk {

class InfoServerImpl : public PluginImplBase {
public:
    explicit InfoServerImpl(System& system);
    explicit InfoServerImpl(std::shared_ptr<System> system);
    ~InfoServerImpl() override;

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<InfoServer::Result, InfoServer::AutopilotVersion> provide_autopilot_version();

    std::pair<InfoServer::Result, InfoServer::ProtocolVersion> provide_protocol_version();

    InfoServer::Result set_autopilot_version(const InfoServer::AutopilotVersion &);
    InfoServer::Result set_protocol_version(const InfoServer::ProtocolVersion &);

private:
    mavlink_message_t
    process_command_request_message(const MavlinkCommandReceiver::CommandLong& command);
    mavlink_message_t
    process_command_request_autopilot_version(
        const MavlinkCommandReceiver::CommandLong& command);
    mavlink_message_t
    process_command_request_protocol_version(const MavlinkCommandReceiver::CommandLong& command);

    void send_protocol_version() const;
    void send_autopilot_version() const;

    InfoServer::ProtocolVersion _protocol_version{};
    mutable std::mutex _protocol_version_mutex{};

    InfoServer::AutopilotVersion _autopilot_version{};
    mutable std::mutex _autopilot_version_mutex{};
};

} // namespace mavsdk