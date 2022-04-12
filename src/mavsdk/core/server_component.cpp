#include "server_component.h"
#include "server_component_impl.h"
#include "mavsdk_impl.h"

namespace mavsdk {

ServerComponent::ServerComponent(MavsdkImpl& mavsdk_impl, uint8_t component_id) :
    _impl(std::make_shared<ServerComponentImpl>(mavsdk_impl, component_id))
{}

void
ServerComponent::send_heartbeat()
{
    _impl->send_heartbeat();
}

void ServerComponent::set_system_status(uint8_t system_status)
{
    return _impl->set_system_status(system_status);
}

uint8_t ServerComponent::get_system_status() const
{
    return _impl->get_system_status();
}

void ServerComponent::set_base_mode(uint8_t base_mode)
{
    _impl->set_base_mode(base_mode);
}

uint8_t ServerComponent::get_base_mode() const
{
    return _impl->get_base_mode();
}

void ServerComponent::set_custom_mode(uint32_t custom_mode)
{
    _impl->set_custom_mode(custom_mode);
}

uint32_t ServerComponent::get_custom_mode() const
{
    return _impl->get_custom_mode();
}

} // namespace mavsdk