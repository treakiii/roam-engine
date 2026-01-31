// ClothSimulator.h - The fabric physicist
// This makes your clothes behave like real fabric, hopefully

#ifndef CLOTHSIMULATOR_H
#define CLOTHSIMULATOR_H

#include <vector>
#include <memory>
#include "Math/Vector3.h"

// Cloth types - what kind of fabric?
enum class ClothType {
    SoftBody,     // General soft body
    Hair,         // Hair simulation
    Fabric,       // Clothing fabric
    Flag,         // Flag waving
    Custom
};

// Simulation modes - CPU or GPU?
enum class SimulationMode {
    CPU,          // Run on CPU
    GPU,          // Run on GPU
    Hybrid        // Mix of both
};

// Constraint types - how particles connect
enum class ConstraintType {
    Distance,     // Fixed distance
    Bend,         // Bending resistance
    Volume,       // Volume preservation
    Collision,    // Collision detection
    Custom
};

// Cloth particle - a point in the cloth mesh
struct ClothParticle {
    Vector3 position;
    Vector3 previousPosition;
    Vector3 velocity;
    Vector3 force;
    float mass;
    bool fixed; // can't move
    std::vector<int> neighbors; // connected particles

    ClothParticle() : position(0, 0, 0), previousPosition(0, 0, 0), velocity(0, 0, 0),
                     force(0, 0, 0), mass(1), fixed(false) {}
};

// Cloth constraint - connection between particles
struct ClothConstraint {
    int particleA;
    int particleB;
    ConstraintType type;
    float restLength;
    float stiffness;
    float damping;

    ClothConstraint() : particleA(-1), particleB(-1), type(ConstraintType::Distance),
                       restLength(1), stiffness(1), damping(0.1f) {}
};

// Collision object - things cloth can collide with
struct CollisionObject {
    enum class ObjectType { Sphere, Box, Plane, Mesh };
    ObjectType type;
    Vector3 position;
    Vector3 size; // for box/sphere
    std::vector<Vector3> vertices; // for mesh
    bool isStatic;

    CollisionObject() : type(ObjectType::Sphere), position(0, 0, 0),
                       size(1, 1, 1), isStatic(true) {}
};

// Wind force - makes cloth move in the breeze
struct WindForce {
    Vector3 direction;
    float strength;
    float turbulence;
    float frequency;

    WindForce() : direction(1, 0, 0), strength(1), turbulence(0.1f), frequency(1) {}
};

// The ClothSimulator class - our fabric expert
class ClothSimulator {
public:
    ClothSimulator();
    ~ClothSimulator();

    // Setup - create the cloth
    bool Initialize(int width, int height, float spacing = 0.1f);
    void Shutdown();

    // Simulation control
    void Update(float deltaTime);
    void Reset();

    // Particle access
    ClothParticle* GetParticle(int x, int y);
    const ClothParticle* GetParticle(int x, int y) const;
    int GetParticleIndex(int x, int y) const { return y * width + x; }

    // Constraints
    void AddConstraint(const ClothConstraint& constraint);
    void RemoveConstraint(int index);
    const std::vector<ClothConstraint>& GetConstraints() const { return constraints; }

    // Collision objects
    void AddCollisionObject(const CollisionObject& object);
    void RemoveCollisionObject(size_t index);
    void ClearCollisionObjects();

    // Forces
    void SetGravity(const Vector3& gravity) { this->gravity = gravity; }
    void AddWindForce(const WindForce& wind) { windForces.push_back(wind); }
    void ClearWindForces() { windForces.clear(); }
    void ApplyForce(int particleIndex, const Vector3& force);

    // Material properties
    void SetStiffness(float stiffness) { this->stiffness = stiffness; }
    void SetDamping(float damping) { this->damping = damping; }
    void SetMass(float mass);
    void SetFriction(float friction) { this->friction = friction; }

    // Fixed particles - anchor points
    void SetParticleFixed(int x, int y, bool fixed);
    void SetParticleFixed(int index, bool fixed);
    bool IsParticleFixed(int index) const;

    // Simulation settings
    void SetSimulationMode(SimulationMode mode) { simulationMode = mode; }
    void SetTimeStep(float step) { timeStep = step; }
    void SetIterations(int iterations) { solverIterations = iterations; }
    void EnableSelfCollision(bool enable) { selfCollision = enable; }

    // Performance
    void SetSubsteps(int substeps) { this->substeps = substeps; }
    int GetParticleCount() const { return particles.size(); }

    // Debug
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    void DrawDebugInfo();

    // Serialization
    bool SaveToFile(const std::string& filename) const;
    bool LoadFromFile(const std::string& filename);

private:
    // Cloth mesh
    int width;
    int height;
    float spacing;
    std::vector<ClothParticle> particles;

    // Constraints
    std::vector<ClothConstraint> constraints;

    // Collision
    std::vector<CollisionObject> collisionObjects;

    // Forces
    Vector3 gravity;
    std::vector<WindForce> windForces;

    // Material properties
    float stiffness;
    float damping;
    float friction;

    // Simulation settings
    SimulationMode simulationMode;
    float timeStep;
    int solverIterations;
    int substeps;
    bool selfCollision;

    // Debug
    bool debugDraw;

    // Internal helpers
    void InitializeParticles();
    void InitializeConstraints();
    void ApplyForces(float deltaTime);
    void SolveConstraints(float deltaTime);
    void HandleCollisions();
    void Integrate(float deltaTime);
    void UpdateNormals();
    bool CheckCollision(const ClothParticle& particle, const CollisionObject& object);
    void ResolveCollision(ClothParticle& particle, const CollisionObject& object);
    Vector3 CalculateWindForce(const ClothParticle& particle, const WindForce& wind);
};

#endif // CLOTHSIMULATOR_H
