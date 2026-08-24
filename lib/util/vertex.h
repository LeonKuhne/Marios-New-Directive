#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

static glm::vec3 random_vertex()
{
  return glm::vec3(
      ((float)rand() / (float)RAND_MAX) - 0.5f,
      ((float)rand() / (float)RAND_MAX) - 0.5f,
      ((float)rand() / (float)RAND_MAX) - 0.5f);
}

static btVector3 asBtVector3(glm::vec3 vec)
{
  return btVector3(vec.x, vec.y, vec.z);
}

static glm::vec3 asGlmVec3(btVector3 vec)
{
  return glm::vec3(vec.x(), vec.y(), vec.z());
}

static btQuaternion asBtQuaternion(glm::quat quat)
{
  return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

static glm::quat asGlmQuat(btQuaternion quat)
{
  return glm::quat(quat.w(), quat.x(), quat.y(), quat.z());
}