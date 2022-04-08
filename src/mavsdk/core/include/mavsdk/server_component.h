#pragma once

namespace mavsdk {

class ServerComponentImpl;

/**
 * TODO: add comments
 */
class ServerComponent {
public:
private:
    std::shared_ptr<ServerComponentImpl> server_component_impl() { return _server_component_impl; };

    std::shared_ptr<ServerComponentImpl> _server_component_impl;

    friend ServerPluginImplBase;
};

} // namespace mavsdk