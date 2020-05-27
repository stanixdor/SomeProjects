#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"

CollisionEvent::CollisionEvent(CollisionData& col): mData(col)
{

}

OnCollisionStarted::OnCollisionStarted(CollisionData& col) : CollisionEvent(col)
{

}

OnCollisionPersisted::OnCollisionPersisted(CollisionData& col) : CollisionEvent(col)
{

}

OnCollisionEnded::OnCollisionEnded(CollisionData& col) : CollisionEvent(col)
{

}