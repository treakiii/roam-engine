// PhysicsWorld.h - The physics simulator
// This makes objects move and collide realistically

#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <vector>
#include <memory>
#include <unordered_set>
#include "Math/Vector3.h"

// Forward declarations - keeping it light
class RigidBody;
class Collider;
class PhysicsMaterial;

// Contact info - when things touch
struct ContactInfo {
    Vector3 point;
    Vector3 normal;
    float penetration;
    RigidBody* bodyA;
    RigidBody* bodyB;
};

// The PhysicsWorld class - our physics playground
class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    // Simulation control - start/stop the madness
    void Initialize(const Vector3& gravity = Vector3(0, -9.81f, 0));
    void Shutdown();
    void Update(float deltaTime);

    // Body management - add/remove objects
    void AddRigidBody(std::shared_ptr<RigidBody> body);
    void RemoveRigidBody(std::shared_ptr<RigidBody> body);

    // Collision detection - find the crashes
    void DetectCollisions();
    void ResolveCollisions();

    // Raycasting - shoot rays through space
    bool Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, ContactInfo& hitInfo);

    // Gravity control - change the rules
    void SetGravity(const Vector3& gravity) { this->gravity = gravity; }
    const Vector3& GetGravity() const { return gravity; }

    // Time control - slow motion or fast forward
    void SetTimeScale(float scale) { timeScale = scale; }
    float GetTimeScale() const { return timeScale; }

    // Debug visualization - see the invisible forces
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    bool IsDebugDrawEnabled() const { return debugDraw; }

    // Performance tuning - make it faster
    void SetMaxSubsteps(int max) { maxSubsteps = max; }
    void SetFixedTimeStep(float step) { fixedTimeStep = step; }

private:
    // Physics objects - the actors
    std::vector<std::shared_ptr<RigidBody>> rigidBodies;

    // Collision pairs - potential crash sites
    std::vector<std::pair<RigidBody*, RigidBody*>> collisionPairs;

    // Contacts - actual collisions
    std::vector<ContactInfo> contacts;

    // World settings
    Vector3 gravity;
    float timeScale;
    float fixedTimeStep;
    int maxSubsteps;

    // Debug settings
    bool debugDraw;
    bool initialized;
};

#endif // PHYSICSWORLD_H
