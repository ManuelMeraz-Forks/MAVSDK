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
    //    _parent->register_mavlink_message_handler(
    //        MAVLINK_MSG_ID_PARAM_SET,
    //        [this](const mavlink_message_t& message) { process_message_set_param(message); },
    //        this);
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
    _parent->provide_server_param_int(name, value);
    register_param_int(name);
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
    _parent->provide_server_param_float(name, value);
    register_param_float(name);
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
        [this, name](int32_t new_value) {
            LogDebug() << "Param Server Int: " << new_value;
            auto p = ParamServer::IntParam{.name = name, .value = new_value};
            LogDebug() << "Construct Param Server Int: " << p;

            LogDebug() << "Done copying";
            _parent->call_user_callback([this, p] {
              LogDebug() << "Check param int changed callback";
              if (_param_int_changed_callback) {
                    LogDebug() << "Scheduled Param Int: " << p;
                    // TODO(manny): Figure out why this callback segfaults.
                    // _param_int_changed_callback(p);
                }
            });

            LogDebug() << "Done Param callback";
        },
        this);
}

void ParamServerImpl::register_param_float(const std::string& name)
{
    _parent->subscribe_param_float(
        name,
        [this, name](float new_value) {
            _param_float_changed_callback(
                ParamServer::FloatParam{.name = name, .value = new_value});
        },
        this);
}

void ParamServerImpl::process_message_set_param(const mavlink_message_t& message)
{
    mavlink_param_set_t param_set;
    mavlink_msg_param_set_decode(&message, &param_set);

    switch (static_cast<MAV_PARAM_TYPE>(param_set.param_type)) {
        case MAV_PARAM_TYPE_UINT8:
        case MAV_PARAM_TYPE_INT8:
        case MAV_PARAM_TYPE_UINT16:
        case MAV_PARAM_TYPE_INT16:
        case MAV_PARAM_TYPE_UINT32:
        case MAV_PARAM_TYPE_INT32:
        case MAV_PARAM_TYPE_UINT64:
        case MAV_PARAM_TYPE_INT64:
            _parent->provide_server_param_int(
                param_set.param_id, static_cast<int>(param_set.param_value));
            break;
        case MAV_PARAM_TYPE_REAL32:
        case MAV_PARAM_TYPE_REAL64:
            _parent->provide_server_param_float(param_set.param_id, param_set.param_value);
            break;
        case MAV_PARAM_TYPE_ENUM_END:
            return;
    }

    auto all_params = _parent->get_all_params();
    auto param_count = all_params.size();
    auto param_index = param_count - 1;

    mavlink_message_t ack;
    mavlink_msg_param_value_pack(
        _parent->get_own_system_id(),
        _parent->get_own_component_id(),
        &ack,
        param_set.param_id,
        param_set.param_value,
        param_set.param_type,
        param_count,
        param_index);

    _parent->send_message(ack);
}
} // namespace mavsdk
