#include "param_server_impl.h"

namespace mavsdk {

ParamServerImpl::ParamServerImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

ParamServerImpl::ParamServerImpl(std::shared_ptr<System> system) : PluginImplBase(std::move(system))
{
    _parent->register_plugin(this);
}

ParamServerImpl::~ParamServerImpl()
{
    _parent->unregister_plugin(this);
}

void ParamServerImpl::init()
{
//    const std::map<std::string, MAVLinkParameters::ParamValue> params = _parent->get_all_params();
//    for (const auto& [param_name, value] : params) {
//        const bool is_int = value.is<uint8_t>() or value.is<int8_t>() or value.is<uint16_t>() or
//                            value.is<int16_t>() or value.is<uint32_t>() or value.is<int32_t>() or
//                            value.is<uint64_t>() or value.is<int64_t>();
//
//        const bool is_float = value.is<float>() or value.is<double>();
//
//        if (is_float) {
//            register_param_float(param_name);
//        } else if (is_int) {
//            register_param_int(param_name);
//        }
//    }
}

void ParamServerImpl::deinit() {}

void ParamServerImpl::enable() {}

void ParamServerImpl::disable() {}

std::pair<ParamServer::Result, int32_t> ParamServerImpl::retrieve_param_int(std::string name) const
{
    std::pair<MAVLinkParameters::Result, int> result = _parent->retrieve_server_param_int(name);

    if (result.first == MAVLinkParameters::Result::Success) {
        return {ParamServer::Result::Success, result.second};
    }

    return {ParamServer::Result::NotFound, -1};
}

ParamServer::Result ParamServerImpl::provide_param_int(std::string name, int32_t value)
{
//    auto [current_value_result, current_value] = retrieve_param_int(name);
    _parent->provide_server_param_int(name, value);
//    auto [new_value_result, new_value] = retrieve_param_int(name);
//
//    const bool is_param_registered = current_value_result == ParamServer::Result::Success and
//                                     new_value_result == ParamServer::Result::Success;
//    if (not is_param_registered) {
//        register_param_int(name);
//    }

    return ParamServer::Result::Success;
}

std::pair<ParamServer::Result, float> ParamServerImpl::retrieve_param_float(std::string name) const
{
    std::pair<MAVLinkParameters::Result, float> result = _parent->retrieve_server_param_float(name);

    if (result.first == MAVLinkParameters::Result::Success) {
        return {ParamServer::Result::Success, result.second};
    }
    return {ParamServer::Result::NotFound, NAN};

}

ParamServer::Result ParamServerImpl::provide_param_float(std::string name, float value)
{
//    auto [current_value_result, current_value] = retrieve_param_float(name);
    _parent->provide_server_param_float(name, value);
//    auto [new_value_result, new_value] = retrieve_param_float(name);

//    const bool is_param_registered = current_value_result == ParamServer::Result::Success and
//                                     new_value_result == ParamServer::Result::Success;
//    if (not is_param_registered) {
//        register_param_float(name);
//    }

    return ParamServer::Result::Success;
}

ParamServer::AllParams ParamServerImpl::retrieve_all_params() const
{
    auto tmp = _parent->retrieve_all_server_params();

    ParamServer::AllParams res{};

    for (auto const& parampair : tmp) {
        if (parampair.second.is<float>()) {
            ParamServer::FloatParam tmp_param;
            tmp_param.name = parampair.first;
            tmp_param.value = parampair.second.get<float>();
            res.float_params.push_back(tmp_param);
        } else if (parampair.second.is<int32_t>()) {
            ParamServer::IntParam tmp_param;
            tmp_param.name = parampair.first;
            tmp_param.value = parampair.second.get<int32_t>();
            res.int_params.push_back(tmp_param);
        }
    }

    return res;
}

void ParamServerImpl::subscribe_param_int_changed(ParamServer::ParamIntChangedCallback callback)
{
    _param_int_changed_callback = std::move(callback);
}

void ParamServerImpl::subscribe_param_float_changed(ParamServer::ParamFloatChangedCallback callback)
{
    _param_float_changed_callback = std::move(callback);
}

ParamServer::Result
ParamServerImpl::result_from_mavlink_parameters_result(MAVLinkParameters::Result result)
{
    switch (result) {
        case MAVLinkParameters::Result::Success:
            return ParamServer::Result::Success;
        case MAVLinkParameters::Result::NotFound:
            return ParamServer::Result::NotFound;
        case MAVLinkParameters::Result::ParamNameTooLong:
            return ParamServer::Result::ParamNameTooLong;
        case MAVLinkParameters::Result::WrongType:
            return ParamServer::Result::WrongType;
        default:
            LogErr() << "Unknown param error";
            return ParamServer::Result::Unknown;
    }
}
void ParamServerImpl::register_param_int(const std::string& name)
{
    _parent->subscribe_param_int(
        name,
        [this, &name](int new_value) {
            _param_int_changed_callback(
                ParamServer::IntParam{.name = name, .value = new_value});
        },
        this);
}

void ParamServerImpl::register_param_float(const std::string& name)
{
    _parent->subscribe_param_float(
        name,
        [this, &name](float new_value) {
            _param_float_changed_callback(
                ParamServer::FloatParam{.name = name, .value = new_value});
        },
        this);
}
} // namespace mavsdk
