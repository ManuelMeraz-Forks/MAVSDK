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

    void send_heartbeat();
    void set_system_status(uint8_t system_status);
    [[nodiscard]] uint8_t get_system_status() const;
    void set_base_mode(uint8_t base_mode);
    [[nodiscard]] uint8_t get_base_mode() const;
    void set_custom_mode(uint32_t custom_mode);
    [[nodiscard]] uint32_t get_custom_mode() const;

private:
    friend ServerPluginImplBase;

    std::shared_ptr<ServerComponentImpl> _impl;
};

} // namespace mavsdk