// WARNING: THIS FILE IS AUTOGENERATED! As such, it should not be edited.
// Edits need to be made to the proto files
// (see https://github.com/mavlink/MAVSDK-Proto/blob/main/protos/info_server/info_server.proto)

#pragma once

#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mavsdk/plugin_base.h"

namespace mavsdk {

class System;
class InfoServerImpl;

/**
 * @brief Provide information about the hardware and/or software of a system.
 */
class InfoServer : public PluginBase {
public:
    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto info_server = InfoServer(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit InfoServer(System& system); // deprecated

    /**
     * @brief Constructor. Creates the plugin for a specific System.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto info_server = InfoServer(system);
     *     ```
     *
     * @param system The specific system associated with this plugin.
     */
    explicit InfoServer(std::shared_ptr<System> system); // new

    /**
     * @brief Destructor (internal use only).
     */
    ~InfoServer();

    /**
     * @brief Mavlink version information.
     */
    struct ProtocolVersion {
        uint32_t version{}; /**< @brief Currently active MAVLink version number * 100: v1.0 is 100,
                               v2.0 is 200, etc. */
        uint32_t min_version{}; /**< @brief Minimum MAVLink version supported */
        uint32_t max_version{}; /**< @brief Maximum MAVLink version supported (set to the same value
                                   as version by default) */
        std::string spec_version_hash{}; /**< @brief The first 8 bytes (not characters printed in
                                            hex!) of the git hash. */
        std::string library_version_hash{}; /**< @brief The first 8 bytes (not characters printed in
                                               hex!) of the git hash. */
    };

    /**
     * @brief Equal operator to compare two `InfoServer::ProtocolVersion` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const InfoServer::ProtocolVersion& lhs, const InfoServer::ProtocolVersion& rhs);

    /**
     * @brief Stream operator to print information about a `InfoServer::ProtocolVersion`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, InfoServer::ProtocolVersion const& protocol_version);

    /**
     * @brief Autopilot version information.
     */
    struct AutopilotVersion {
        uint64_t capabilities{}; /**< @brief MAV_PROTOCOL_CAPABILITYBitmap of capabilities */
        uint32_t flight_sw_version{}; /**< @brief Firmware version number */
        uint32_t middleware_sw_version{}; /**< @brief Middleware version number */
        uint32_t os_sw_version{}; /**< @brief Operating system version number */
        uint32_t
            board_version{}; /**< @brief HW / board version (last 8 bits should be silicon ID, if
                                any). The first 16 bits of this field specify
                                https://github.com/PX4/PX4-Bootloader/blob/master/board_types.txt */
        std::string
            flight_custom_version{}; /**< @brief Custom version field, commonly the first 8 bytes of
                                        the git hash. This is not an unique identifier, but should
                                        allow to identify the commit using the main version number
                                        even for very large code bases. */
        std::string
            middleware_custom_version{}; /**< @brief Custom version field, commonly the first 8
                                            bytes of the git hash. This is not an unique identifier,
                                            but should allow to identify the commit using the main
                                            version number even for very large code bases. */
        std::string os_custom_version{}; /**< @brief Custom version field, commonly the first 8
                                            bytes of the git hash. This is not an unique identifier,
                                            but should allow to identify the commit using the main
                                            version number even for very large code bases. */
        uint32_t vendor_id{}; /**< @brief ID of the board vendor */
        uint32_t product_id{}; /**< @brief ID of the product */
        uint64_t uid{}; /**< @brief UID if provided by hardware (see uid2) */
        std::string uid2{}; /**< @brief UID if provided by hardware (supersedes the uid field. If
                               this is non-zero, use this field, otherwise use uid) */
    };

    /**
     * @brief Equal operator to compare two `InfoServer::AutopilotVersion` objects.
     *
     * @return `true` if items are equal.
     */
    friend bool
    operator==(const InfoServer::AutopilotVersion& lhs, const InfoServer::AutopilotVersion& rhs);

    /**
     * @brief Stream operator to print information about a `InfoServer::AutopilotVersion`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream&
    operator<<(std::ostream& str, InfoServer::AutopilotVersion const& autopilot_version);

    /**
     * @brief Possible results returned for info requests.
     */
    enum class Result {
        Unknown, /**< @brief Unknown result. */
        Success, /**< @brief Request succeeded. */
        InformationNotReceivedYet, /**< @brief Information has not been received yet. */
        NoSystem, /**< @brief No system is connected. */
    };

    /**
     * @brief Stream operator to print information about a `InfoServer::Result`.
     *
     * @return A reference to the stream.
     */
    friend std::ostream& operator<<(std::ostream& str, InfoServer::Result const& result);

    /**
     * @brief Callback type for asynchronous InfoServer calls.
     */
    using ResultCallback = std::function<void(Result)>;

    /**
     * @brief Provide autopilot version of the system.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    std::pair<Result, InfoServer::AutopilotVersion> provide_autopilot_version() const;

    /**
     * @brief Provide protocol version of the system.
     *
     * This function is blocking.
     *
     * @return Result of request.
     */
    std::pair<Result, InfoServer::ProtocolVersion> provide_protocol_version() const;

    /**
     * @brief Copy constructor.
     */
    InfoServer(const InfoServer& other);

    /**
     * @brief Equality operator (object is not copyable).
     */
    const InfoServer& operator=(const InfoServer&) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<InfoServerImpl> _impl;
};

} // namespace mavsdk