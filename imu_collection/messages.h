#include <stdint.h>
#include <assert.h>

const uint32_t HASH_PRIME = 59359;

typedef struct {
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  float qx, qy, qz, qw;
  uint32_t hash;
} IMU_Message;

uint32_t recast(float v) {
  uint32_t * val = reinterpret_cast<uint32_t *>(&v);
  return *val;
}

uint32_t hash_msg(IMU_Message * msg) {
  assert(sizeof(float) == sizeof(uint32_t));
  uint32_t s = recast(msg->ax) + recast(msg->ay) + recast(msg->az) +
               recast(msg->gx) + recast(msg->gy) + recast(msg->gz) +
               recast(msg->mx) + recast(msg->my) + recast(msg->mz) +
               recast(msg->qx) + recast(msg->qy) + recast(msg->qz) + recast(msg->qw);
  return s % HASH_PRIME;
}
