#pragma once

#include <stddef.h>
#include <string>
#include <vector>
#include <limits.h>

namespace facebook {
namespace wdt {

/// checkpoint is a pair of port number and number of successfully transferred
/// blocks
typedef std::pair<int32_t, int64_t> Checkpoint;

class Protocol {
 public:
  /// Both version, magic number and command byte
  enum CMD_MAGIC {
    DONE_CMD = 0x44,      // D)one
    FILE_CMD = 0x4C,      // L)oad
    WAIT_CMD = 0x57,      // W)ait
    ERR_CMD = 0x45,       // E)rr
    SETTINGS_CMD = 0x53,  // S)ettings
    ABORT_CMD = 0x41,     // A)bort
    EXIT_CMD = 0x65,      // e)xit
    SIZE_CMD = 0x5A,      // Si(Z)e
  };

  /// Max size of sender or receiver id
  static const size_t kMaxTransferIdLength = 50;
  /// Max size of filename + 4 max varints + 1 byte for cmd + 1 byte for status
  static const size_t kMaxHeader = PATH_MAX + 5 * 10 + 2 + 2;
  /// min number of bytes that must be send to unblock receiver
  static const size_t kMinBufLength = 256;
  /// max size of local checkpoint encoding
  static const size_t kMaxLocalCheckpoint = 10 + 2 * 10;
  /// max size of done command encoding
  static const size_t kMaxDone = 2 + 10;
  /// max length of the size cmd encoding
  static const size_t kMaxSize = 1 + 10;
  /// max size of settings command encoding
  static const size_t kMaxSettings = 1 + 4 * 10 + kMaxTransferIdLength;

  /// encodes id, sequence-id, block-size, block-offset and file-size
  /// into dest+off
  /// moves the off into dest pointer, not going past max
  /// @return false if there isn't enough room to encode
  static bool encodeHeader(char *dest, size_t &off, size_t max, std::string id,
                           uint64_t seqId, int64_t size, int64_t offset,
                           int64_t fileSize);
  /// decodes from src+off and consumes/moves off but not past max
  /// sets id, sequence-id, block-size, block-offset and file-size
  /// @return false if there isn't enough data in src+off to src+max
  static bool decodeHeader(char *src, size_t &off, size_t max, std::string &id,
                           uint64_t &seqId, int64_t &size, int64_t &offset,
                           int64_t &fileSize);

  /// encodes checkpoints into dest+off
  /// moves the off into dest pointer, not going past max
  /// @return false if there isn't enough room to encode
  static bool encodeCheckpoints(char *dest, size_t &off, size_t max,
                                const std::vector<Checkpoint> &checkpoints);

  /// decodes from src+off and consumes/moves off but not past max
  /// sets checkpoints
  /// @return false if there isn't enough data in src+off to src+max
  static bool decodeCheckpoints(char *src, size_t &off, size_t max,
                                std::vector<Checkpoint> &checkpoints);

  /// encodes numBlocks into dest+off
  /// moves the off into dest pointer, not going past max
  /// @return false if there isn't enough room to encode
  static bool encodeDone(char *dest, size_t &off, size_t max,
                         int64_t numBlocks);

  /// decodes from src+off and consumes/moves off but not past max
  /// sets numBlocks
  /// @return false if there isn't enough data in src+off to src+max
  static bool decodeDone(char *src, size_t &off, size_t max,
                         int64_t &numBlocks);

  /// encodes protocol version, read and write timeout, sender-id into dest+off
  /// moves the off into dest pointer, not going past max
  /// @return false if there isn't enough room to encode
  static bool encodeSettings(char *dest, size_t &off, size_t max,
                             int32_t protocolVersion, int64_t readTimeoutMillis,
                             int64_t writeTimeoutMillis,
                             const std::string &senderId);

  /// decodes from src+off and consumes/moves off but not past max
  /// sets protocolVersion, readTimeoutMillis, writeTimeoutMillis and senderId
  /// @return false if there isn't enough data in src+off to src+max
  static bool decodeSettings(char *src, size_t &off, size_t max,
                             int32_t &protocolVersion,
                             int64_t &readTimeoutMillis,
                             int64_t &writeTimeoutMillis,
                             std::string &senderId);

  /// encodes totalNumBytes into dest+off
  /// moves the off into dest pointer, not going past max
  /// @return false if there isn't enough room to encode
  static bool encodeSize(char *dest, size_t &off, size_t max,
                         int64_t totalNumBytes);

  /// decodes from src+off and consumes/moves off but not past max
  /// sets totalNumBytes
  /// @return false if there isn't enough data in src+off to src+max
  static bool decodeSize(char *src, size_t &off, size_t max,
                         int64_t &totalNumBytes);
};
}
}  // namespace facebook::wdt
