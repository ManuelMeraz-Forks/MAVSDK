// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/master/protos/info_server/info_server.proto)

#include "info_server/info_server.grpc.pb.h"
#include "plugins/info_server/info_server.h"

#include "mavsdk.h"
#include "lazy_plugin.h"
#include "log.h"
#include <atomic>
#include <cmath>
#include <future>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

namespace mavsdk {
namespace mavsdk_server {

template<typename InfoServer = InfoServer, typename LazyPlugin = LazyPlugin<InfoServer>>
class InfoServerServiceImpl final : public rpc::info_server::InfoServerService::Service {
public:
    InfoServerServiceImpl(LazyPlugin& lazy_plugin) : _lazy_plugin(lazy_plugin) {}

    template<typename ResponseType>
    void fillResponseWithResult(ResponseType* response, mavsdk::InfoServer::Result& result) const
    {
        auto rpc_result = translateToRpcResult(result);

        auto* rpc_info_server_result = new rpc::info_server::InfoServerResult();
        rpc_info_server_result->set_result(rpc_result);
        std::stringstream ss;
        ss << result;
        rpc_info_server_result->set_result_str(ss.str());

        response->set_allocated_info_server_result(rpc_info_server_result);
    }

    static std::unique_ptr<rpc::info_server::ProtocolVersion>
    translateToRpcProtocolVersion(const mavsdk::InfoServer::ProtocolVersion& protocol_version)
    {
        auto rpc_obj = std::make_unique<rpc::info_server::ProtocolVersion>();

        rpc_obj->set_version(protocol_version.version);

        rpc_obj->set_min_version(protocol_version.min_version);

        rpc_obj->set_max_version(protocol_version.max_version);

        rpc_obj->set_spec_version_hash(protocol_version.spec_version_hash);

        rpc_obj->set_library_version_hash(protocol_version.library_version_hash);

        return rpc_obj;
    }

    static mavsdk::InfoServer::ProtocolVersion
    translateFromRpcProtocolVersion(const rpc::info_server::ProtocolVersion& protocol_version)
    {
        mavsdk::InfoServer::ProtocolVersion obj;

        obj.version = protocol_version.version();

        obj.min_version = protocol_version.min_version();

        obj.max_version = protocol_version.max_version();

        obj.spec_version_hash = protocol_version.spec_version_hash();

        obj.library_version_hash = protocol_version.library_version_hash();

        return obj;
    }

    static std::unique_ptr<rpc::info_server::AutopilotVersion>
    translateToRpcAutopilotVersion(const mavsdk::InfoServer::AutopilotVersion& autopilot_version)
    {
        auto rpc_obj = std::make_unique<rpc::info_server::AutopilotVersion>();

        rpc_obj->set_capabilities(autopilot_version.capabilities);

        rpc_obj->set_flight_sw_version(autopilot_version.flight_sw_version);

        rpc_obj->set_middleware_sw_version(autopilot_version.middleware_sw_version);

        rpc_obj->set_os_sw_version(autopilot_version.os_sw_version);

        rpc_obj->set_board_version(autopilot_version.board_version);

        rpc_obj->set_flight_custom_version(autopilot_version.flight_custom_version);

        rpc_obj->set_middleware_custom_version(autopilot_version.middleware_custom_version);

        rpc_obj->set_os_custom_version(autopilot_version.os_custom_version);

        rpc_obj->set_vendor_id(autopilot_version.vendor_id);

        rpc_obj->set_product_id(autopilot_version.product_id);

        rpc_obj->set_uid(autopilot_version.uid);

        rpc_obj->set_uid2(autopilot_version.uid2);

        return rpc_obj;
    }

    static mavsdk::InfoServer::AutopilotVersion
    translateFromRpcAutopilotVersion(const rpc::info_server::AutopilotVersion& autopilot_version)
    {
        mavsdk::InfoServer::AutopilotVersion obj;

        obj.capabilities = autopilot_version.capabilities();

        obj.flight_sw_version = autopilot_version.flight_sw_version();

        obj.middleware_sw_version = autopilot_version.middleware_sw_version();

        obj.os_sw_version = autopilot_version.os_sw_version();

        obj.board_version = autopilot_version.board_version();

        obj.flight_custom_version = autopilot_version.flight_custom_version();

        obj.middleware_custom_version = autopilot_version.middleware_custom_version();

        obj.os_custom_version = autopilot_version.os_custom_version();

        obj.vendor_id = autopilot_version.vendor_id();

        obj.product_id = autopilot_version.product_id();

        obj.uid = autopilot_version.uid();

        obj.uid2 = autopilot_version.uid2();

        return obj;
    }

    static rpc::info_server::InfoResult::Result
    translateToRpcResult(const mavsdk::InfoServer::Result& result)
    {
        switch (result) {
            default:
                LogErr() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case mavsdk::InfoServer::Result::Unknown:
                return rpc::info_server::InfoResult_Result_RESULT_UNKNOWN;
            case mavsdk::InfoServer::Result::Success:
                return rpc::info_server::InfoResult_Result_RESULT_SUCCESS;
            case mavsdk::InfoServer::Result::InformationNotReceivedYet:
                return rpc::info_server::InfoResult_Result_RESULT_INFORMATION_NOT_RECEIVED_YET;
            case mavsdk::InfoServer::Result::NoSystem:
                return rpc::info_server::InfoResult_Result_RESULT_NO_SYSTEM;
        }
    }

    static mavsdk::InfoServer::Result
    translateFromRpcResult(const rpc::info_server::InfoResult::Result result)
    {
        switch (result) {
            default:
                LogErr() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case rpc::info_server::InfoResult_Result_RESULT_UNKNOWN:
                return mavsdk::InfoServer::Result::Unknown;
            case rpc::info_server::InfoResult_Result_RESULT_SUCCESS:
                return mavsdk::InfoServer::Result::Success;
            case rpc::info_server::InfoResult_Result_RESULT_INFORMATION_NOT_RECEIVED_YET:
                return mavsdk::InfoServer::Result::InformationNotReceivedYet;
            case rpc::info_server::InfoResult_Result_RESULT_NO_SYSTEM:
                return mavsdk::InfoServer::Result::NoSystem;
        }
    }

    grpc::Status ProvideAutopilotVersion(
        grpc::ServerContext* /* context */,
        const rpc::info_server::ProvideAutopilotVersionRequest* /* request */,
        rpc::info_server::ProvideAutopilotVersionResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::InfoServer::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->provide_autopilot_version();

        if (response != nullptr) {
            fillResponseWithResult(response, result.first);

            response->set_allocated_protocol_version_info(
                translateToRpcAutopilotVersion(result.second).release());
        }

        return grpc::Status::OK;
    }

    grpc::Status ProvideProtocolVersion(
        grpc::ServerContext* /* context */,
        const rpc::info_server::ProvideProtocolVersionRequest* /* request */,
        rpc::info_server::ProvideProtocolVersionResponse* response) override
    {
        if (_lazy_plugin.maybe_plugin() == nullptr) {
            if (response != nullptr) {
                auto result = mavsdk::InfoServer::Result::NoSystem;
                fillResponseWithResult(response, result);
            }

            return grpc::Status::OK;
        }

        auto result = _lazy_plugin.maybe_plugin()->provide_protocol_version();

        if (response != nullptr) {
            fillResponseWithResult(response, result.first);

            response->set_allocated_protocol_version_info(
                translateToRpcProtocolVersion(result.second).release());
        }

        return grpc::Status::OK;
    }

    void stop()
    {
        _stopped.store(true);
        for (auto& prom : _stream_stop_promises) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        }
    }

private:
    void register_stream_stop_promise(std::weak_ptr<std::promise<void>> prom)
    {
        // If we have already stopped, set promise immediately and don't add it to list.
        if (_stopped.load()) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        } else {
            _stream_stop_promises.push_back(prom);
        }
    }

    void unregister_stream_stop_promise(std::shared_ptr<std::promise<void>> prom)
    {
        for (auto it = _stream_stop_promises.begin(); it != _stream_stop_promises.end();
             /* ++it */) {
            if (it->lock() == prom) {
                it = _stream_stop_promises.erase(it);
            } else {
                ++it;
            }
        }
    }

    LazyPlugin& _lazy_plugin;
    std::atomic<bool> _stopped{false};
    std::vector<std::weak_ptr<std::promise<void>>> _stream_stop_promises{};
};

} // namespace mavsdk_server
} // namespace mavsdk