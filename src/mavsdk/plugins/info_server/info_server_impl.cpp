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

void InfoServerImpl::init() {}

void InfoServerImpl::deinit() {}

void InfoServerImpl::enable() {}

void InfoServerImpl::disable() {}

std::pair<InfoServer::Result, InfoServer::AutopilotVersion>
InfoServerImpl::provide_autopilot_version()
{
    // TODO :)
    return {};
}

std::pair<InfoServer::Result, InfoServer::ProtocolVersion>
InfoServerImpl::provide_protocol_version()
{
    // TODO :)
    return {};
}

} // namespace mavsdk