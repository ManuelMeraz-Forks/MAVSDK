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

private:
};

} // namespace mavsdk