#include "kun/core/object/object.h"

namespace kun
{
Object::~Object() = default;

Name Object::name() { return {}; }
void Object::rename(Name name) {}

void Object::onInit() {}
void Object::onShutdown() {}
void Object::onSerialize(Archive& ar) {}
}// namespace kun
