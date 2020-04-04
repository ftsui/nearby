#ifndef CORE_INTERNAL_MEDIUMS_BLE_PACKET_H_
#define CORE_INTERNAL_MEDIUMS_BLE_PACKET_H_

#include "platform/byte_array.h"
#include "platform/ptr.h"

namespace location {
namespace nearby {
namespace connections {
namespace mediums {

// Represents the format of data sent over BLE sockets.
//
// [SERVICE_ID_HASH][DATA]
//
// See go/nearby-ble-design for more information.
class BLEPacket {
 public:
  static ConstPtr<BLEPacket> fromBytes(ConstPtr<ByteArray> ble_packet_bytes);

  static ConstPtr<ByteArray> toBytes(ConstPtr<ByteArray> service_id_hash,
                                     ConstPtr<ByteArray> data);

  static const std::uint32_t kServiceIdHashLength;

  ~BLEPacket();

  ConstPtr<ByteArray> getServiceIdHash() const;
  ConstPtr<ByteArray> getData() const;

 private:
  static size_t computeDataSize(ConstPtr<ByteArray> ble_packet_bytes);
  static size_t computePacketLength(ConstPtr<ByteArray> data);

  static const std::uint32_t kMinPacketLength;
  static const std::uint32_t kMaxDataSize;

  BLEPacket(ConstPtr<ByteArray> service_id_hash, ConstPtr<ByteArray> data);

  ScopedPtr<ConstPtr<ByteArray> > service_id_hash_;
  ScopedPtr<ConstPtr<ByteArray> > data_;
};

// Represents the format of data sent over BLE sockets.
//
// [SERVICE_ID_HASH][DATA]
//
// See go/nearby-ble-design for more information.
class BlePacket {
 public:
  static BlePacket FromBytes(const ByteArray& bytes);

  static ByteArray ToBytes(const ByteArray& service_id_hash,
                           const ByteArray& data);

  static const uint32_t kServiceIdHashLength;

  ~BlePacket();

  ByteArray GetServiceIdHash() const;
  ByteArray GetData() const;

 private:
  static size_t ComputeDataSize(const ByteArray& ble_packet_bytes);
  static size_t ComputePacketLength(const ByteArray& data);

  static const uint32_t kMinPacketLength;
  static const uint32_t kMaxDataSize;

  BlePacket(const ByteArray& service_id_hash, const ByteArray& data);

  ByteArray service_id_hash_;
  ByteArray data_;
};

}  // namespace mediums
}  // namespace connections
}  // namespace nearby
}  // namespace location

#endif  // CORE_INTERNAL_MEDIUMS_BLE_PACKET_H_
