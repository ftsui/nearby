#ifndef CORE_INTERNAL_BLUETOOTH_BWU_HANDLER_H_
#define CORE_INTERNAL_BLUETOOTH_BWU_HANDLER_H_

#include <string>

#include "core/internal/base_bwu_handler.h"
#include "core/internal/client_proxy.h"
#include "core/internal/mediums/mediums.h"
#include "core/internal/mediums/utils.h"
#include "proto/connections/offline_wire_formats.pb.h"
#include "platform/public/bluetooth_classic.h"
#include "platform/public/count_down_latch.h"
#include "proto/connections_enums.pb.h"

namespace location {
namespace nearby {
namespace connections {

// Defines the set of methods that need to be implemented to handle the
// per-Medium-specific operations needed to upgrade an EndpointChannel.
class BluetoothBwuHandler : public BaseBwuHandler {
 public:
  BluetoothBwuHandler(Mediums& mediums, EndpointChannelManager& channel_manager,
                      BwuNotifications notifications);
  ~BluetoothBwuHandler() override = default;

 private:
  constexpr static const int kServiceIdLength = 10;

  // Implements BaseBwuHandler:
  // Reverts any changes made to the device in the process of upgrading
  // endpoints.
  void Revert() override;

  // Cleans up in-progress upgrades after endpoint disconnection.
  void OnEndpointDisconnect(ClientProxy* client,
                            const std::string& endpoint_id) override {}

  void OnIncomingBluetoothConnection(ClientProxy* client,
                                     const std::string& service_id,
                                     BluetoothSocket socket);

  class BluetoothIncomingSocket : public IncomingSocket {
   public:
    explicit BluetoothIncomingSocket(const std::string& name,
                                     BluetoothSocket socket)
        : name_(name), socket_(socket) {}
    ~BluetoothIncomingSocket() override = default;
    std::string ToString() override { return name_; }
    void Close() override { socket_.Close(); }

   private:
    std::string name_;
    BluetoothSocket socket_;
  };

  // First part of InitiateBwuForEndpoint implementation;
  // returns a BWU request to remote party as byte array.
  ByteArray InitializeUpgradedMediumForEndpoint(
      ClientProxy* client, const std::string& service_id,
      const std::string& endpoint_id) override;

  // Invoked from OnBwuNegotiationFrame.
  std::unique_ptr<EndpointChannel> CreateUpgradedEndpointChannel(
      ClientProxy* client, const std::string& service_id,
      const std::string& endpoint_id,
      const UpgradePathInfo& upgrade_path_info) override;

  // Returns the upgrade medium of the BwuHandler.
  // @BwuHandlerThread
  Medium GetUpgradeMedium() const override { return Medium::BLUETOOTH; }

  Mediums& mediums_;
  absl::flat_hash_set<std::string> active_service_ids_;
  BluetoothRadio& bluetooth_radio_{mediums_.GetBluetoothRadio()};
  BluetoothClassic& bluetooth_medium_{mediums_.GetBluetoothClassic()};
};

}  // namespace connections
}  // namespace nearby
}  // namespace location

#endif  // CORE_INTERNAL_BLUETOOTH_BWU_HANDLER_H_
