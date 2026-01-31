// AIController.h - The brain behind the bots
// This makes NPCs think and act smart

#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "Math/Vector3.h"

// AI state - what is the AI doing?
enum class AIState {
    Idle,
    Patrolling,
    Chasing,
    Attacking,
    Fleeing,
    Searching,
    Dead
};

// AI behavior type - how does it think?
enum class AIBehavior {
    Passive,
    Aggressive,
    Defensive,
    Cowardly,
    Curious,
    Guard
};

// Decision - what the AI chooses to do
struct AIDecision {
    std::string action;
    Vector3 targetPosition;
    float priority;
    float confidence;
};

// Sensor data - what the AI can sense
struct AISensorData {
    Vector3 position;
    Vector3 velocity;
    float health;
    bool canSeePlayer;
    bool canHearPlayer;
    Vector3 lastKnownPlayerPosition;
    float distanceToPlayer;
    std::vector<Vector3> visibleEnemies;
    std::vector<Vector3> visibleAllies;
};

// The AIController class - our artificial intelligence
class AIController {
public:
    AIController();
    ~AIController();

    // AI lifecycle - birth to death
    void Initialize();
    void Update(float deltaTime);
    void Shutdown();

    // State management - change behavior
    void SetState(AIState newState);
    AIState GetCurrentState() const { return currentState; }

    // Behavior settings - personality
    void SetBehavior(AIBehavior behavior) { this->behavior = behavior; }
    AIBehavior GetBehavior() const { return behavior; }

    // Sensor input - feed the brain
    void UpdateSensorData(const AISensorData& data);
    const AISensorData& GetSensorData() const { return sensorData; }

    // Decision making - think and choose
    AIDecision MakeDecision();

    // Movement - where to go
    Vector3 CalculateMovementTarget();
    Vector3 CalculateFleeDirection();

    // Combat - fight or flight
    bool ShouldAttack();
    bool ShouldFlee();
    Vector3 CalculateAttackPosition();

    // Pathfinding - get from A to B
    std::vector<Vector3> FindPath(const Vector3& start, const Vector3& end);
    bool IsPathBlocked(const std::vector<Vector3>& path);

    // Memory - remember things
    void RememberPosition(const std::string& key, const Vector3& position);
    Vector3 RecallPosition(const std::string& key) const;
    void Forget(const std::string& key);

    // Learning - get smarter
    void LearnFromExperience(const std::string& situation, bool success);
    float GetLearningScore(const std::string& situation) const;

    // Communication - talk to other AIs
    void SendMessage(const std::string& message, const std::string& recipient = "all");
    void ReceiveMessage(const std::string& message, const std::string& sender);

    // Group behavior - work together
    void JoinGroup(const std::string& groupName);
    void LeaveGroup();
    std::vector<AIController*> GetGroupMembers() const;
    Vector3 CalculateGroupFormationPosition();

    // Debug - see what the AI is thinking
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    void DrawDebugInfo();

private:
    // Decision making helpers
    float EvaluateAction(const std::string& action);
    std::vector<std::string> GetAvailableActions();

    // State machine - manage behavior
    void UpdateStateMachine(float deltaTime);
    void OnStateEnter(AIState state);
    void OnStateExit(AIState state);

    // Internal state
    AIState currentState;
    AIState previousState;
    AIBehavior behavior;
    AISensorData sensorData;

    // Memory system
    std::unordered_map<std::string, Vector3> memory;

    // Learning system
    std::unordered_map<std::string, float> learnedExperiences;

    // Group system
    std::string groupName;
    std::vector<AIController*> groupMembers;

    // Timers and counters
    float stateTimer;
    float decisionTimer;
    int consecutiveFailures;

    // Settings
    float aggressionLevel;
    float fearLevel;
    float curiosityLevel;
    float memorySpan;

    // Debug
    bool debugDraw;
};

#endif // AICONTROLLER_H
