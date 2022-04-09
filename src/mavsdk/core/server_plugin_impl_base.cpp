#include "server_plugin_impl_base.h"
#include "mavsdk.h"
#include "mavsdk_impl.h"
#include "server_component.h"

namespace mavsdk {

ServerPluginImplBase::ServerPluginImplBase(Mavsdk& mavsdk) :
    _server_component_impl(mavsdk._impl->server_component(MAV_COMP_ID_CAMERA)->_impl)
{}

} // namespace mavsdk