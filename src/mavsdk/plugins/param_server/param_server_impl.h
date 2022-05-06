#pragma once

#include "plugins/param_server/param_server.h"
#include "plugin_impl_base.h"

namespace mavsdk {

class ParamServerImpl : public PluginImplBase {
public:
    explicit ParamServerImpl(System& system);
    explicit ParamServerImpl(std::shared_ptr<System> system);
    ~ParamServerImpl() override;

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<ParamServer::Result, int32_t> retrieve_param_int(std::string name) const;

    ParamServer::Result provide_param_int(std::string name, int32_t value);

    std::pair<ParamServer::Result, float> retrieve_param_float(std::string name) const;

    ParamServer::Result provide_param_float(std::string name, float value);

    ParamServer::AllParams retrieve_all_params() const;

    ParamServer::Result result_from_mavlink_parameters_result(MAVLinkParameters::Result result);

    void subscribe_param_int_changed(ParamServer::ParamIntChangedCallback callback);
    void subscribe_param_float_changed(ParamServer::ParamFloatChangedCallback callback);

private:
    void register_param_int(const std::string &name);
    void register_param_float(const std::string &name);

    ParamServer::ParamIntChangedCallback _param_int_changed_callback = [](const ParamServer::IntParam&) { /* noop */ };
    ParamServer::ParamFloatChangedCallback _param_float_changed_callback = [](const ParamServer::FloatParam&) { /* noop */ };
};

} // namespace mavsdk
