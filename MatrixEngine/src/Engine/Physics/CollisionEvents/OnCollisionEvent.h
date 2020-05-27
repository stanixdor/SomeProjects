#pragma once
#include "Engine\Platform\EventSystem\event.h"
#include "Engine\Physics\CollisionData.h"

class CollisionEvent : public Messaging::Event
{
public:
    CollisionEvent(CollisionData& col);
    virtual ~CollisionEvent() = default;
    CollisionData mData;
};

class OnCollisionStarted : public CollisionEvent
{
public:
    OnCollisionStarted(CollisionData& col);
};

class OnCollisionPersisted : public CollisionEvent
{
public:
    OnCollisionPersisted(CollisionData& col);
};

class OnCollisionEnded : public CollisionEvent
{
public:
    OnCollisionEnded(CollisionData& col);
};
