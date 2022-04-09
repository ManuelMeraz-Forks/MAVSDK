#pragma once

#include "unused.h"
#include "log.h"
#include "mavlink_include.h"
#include "server_component_impl.h"

namespace mavsdk {

class Mavsdk;

class ServerPluginImplBase {
public:
    explicit ServerPluginImplBase(Mavsdk& mavsdk);
    virtual ~ServerPluginImplBase() = default;

    /*
     * The method `init()` is called when a plugin is instantiated.
     */
    virtual void init() = 0;

    /*
     * The method `deinit()` is called before a plugin is destructed which happens
     * usually only at the very end when a Mavsdk instance is destructed.
     *
     * Plugins should do any cleanup of what has been set up during init.
     */
    virtual void deinit() = 0;

    ServerPluginImplBase(const ServerPluginImplBase&) = delete;
    const ServerPluginImplBase& operator=(const ServerPluginImplBase&) = delete;

protected:
    std::shared_ptr<ServerComponentImpl> _server_component_impl;
};

} // namespace mavsdk
