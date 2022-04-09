#pragma once

#include <cstdint>
#include <memory>

namespace mavsdk {

class MavsdkImpl;
class ServerComponentImpl;
class ServerPluginImplBase;

/**
 * TODO: add comments
 */
class ServerComponent {
public:
    ServerComponent(MavsdkImpl& mavsdk_impl, uint8_t component_id);

private:
    friend ServerPluginImplBase;

    std::shared_ptr<ServerComponentImpl> _impl;
};

} // namespace mavsdk