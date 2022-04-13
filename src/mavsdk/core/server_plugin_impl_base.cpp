#include "server_plugin_impl_base.h"
#include "mavsdk.h"
#include "mavsdk_impl.h"

namespace mavsdk {

ServerPluginImplBase::ServerPluginImplBase(Mavsdk& mavsdk) :
    _mavsdk_impl(*mavsdk._impl)
{}

} // namespace mavsdk