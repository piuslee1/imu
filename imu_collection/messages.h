#include <stdint.h>
#include <assert.h>

const uint32_t HASH_PRIME = 59359;

typedef struct {
  float ax, ay, az, gx, gy, gz;
  float qx, qy, qz, qw;
  uint32_t hash;
} IMU_Message;

uint32_t hash_msg(IMU_Message * msg) {
  assert(sizeof(float) == sizeof(uint32_t));
  return (reinterpret_cast<uint32_t*>(&msg->ax) + reinterpret_cast<uint32_t*>(&msg->ay) + reinterpret_cast<uint32_t*>(&msg->az) +
          reinterpret_cast<uint32_t*>(&msg->gx) + reinterpret_cast<uint32_t*>(&msg->gy) + reinterpret_cast<uint32_t*>(&msg->gz) +
          reinterpret_cast<uint32_t*>(&msg->qx) + reinterpret_cast<uint32_t*>(&msg->qy) + reinterpret_cast<uint32_t*>(&msg->qz) + reinterpret_cast<uint32_t*>(&msg->qw)) % HASH_PRIME;
}
