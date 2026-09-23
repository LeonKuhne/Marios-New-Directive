#include "trigger.h"

Trigger::Trigger(const Info &info)
  : Solid(info.solid)
{
  // use body as trigger
  body->setActivationState(DISABLE_DEACTIVATION);
  body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}