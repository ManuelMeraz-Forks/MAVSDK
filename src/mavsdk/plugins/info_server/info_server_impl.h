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

    std::pair<InfoServer::Result, InfoServer::FlightInfo> provide_flight_information();

    std::pair<InfoServer::Result, InfoServer::Identification> provide_identification();

    std::pair<InfoServer::Result, InfoServer::Product> provide_product();

    std::pair<InfoServer::Result, InfoServer::Version> provide_version();

    std::pair<InfoServer::Result, double> provide_speed_factor();

private:
    void process_autopilot_version(const mavlink_message_t& message);
    void process_flight_information(const mavlink_message_t& message);
    void process_attitude(const mavlink_message_t& message);

    InfoServer::Version _version{};
    InfoServer::Product _product{};
    InfoServer::Identification _identification{};
    InfoServer::FlightInfo _flight_info{};

    mutable std::mutex _mutex{};
};

} // namespace mavsdk
