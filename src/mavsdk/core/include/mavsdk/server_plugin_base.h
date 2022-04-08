#pragma once
#include "server_component_impl.h"
#include <memory>

namespace mavsdk {

class ServerComponent;
class ServerComponentImpl;

class ServerPluginBase {
public:
    /**
     * @brief Default Constructor.
     */
    ServerPluginBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~ServerPluginBase() = default;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    ServerPluginBase(const ServerPluginBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const ServerPluginBase& operator=(const ServerPluginBase&) = delete;
};

} // namespace mavsdk
