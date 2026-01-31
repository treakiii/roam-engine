// Animator.h - The animation director
// This brings your characters to life with smooth movements

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

// Animation states - what's the character doing?
enum class AnimationState {
    Idle,
    Walking,
    Running,
    Jumping,
    Falling,
    Attacking,
    Defending,
    Dying,
    Custom
};

// Animation types - how are we animating?
enum class AnimationType {
    Skeletal,
    BlendShape,
    Procedural,
    SpriteSheet,
    Custom
};

// Interpolation types - how smooth is the transition?
enum class InterpolationType {
    Linear,
    Cubic,
    Bezier,
    Step,
    Custom
};

// Loop modes - does it repeat?
enum class LoopMode {
    Once,
    Loop,
    PingPong,
    Clamp
};

// Keyframe - a single point in time
struct Keyframe {
    float time;
    std::unordered_map<std::string, float> values; // bone names or property names to values
    InterpolationType interpolation;
    std::vector<float> tangents; // for bezier curves
};

// Animation clip - a collection of keyframes
class AnimationClip {
public:
    AnimationClip(const std::string& name);
    ~AnimationClip();

    // Clip properties
    const std::string& GetName() const { return name; }
    void SetDuration(float duration) { this->duration = duration; }
    float GetDuration() const { return duration; }
    void SetLoopMode(LoopMode mode) { loopMode = mode; }
    LoopMode GetLoopMode() const { return loopMode; }

    // Keyframes
    void AddKeyframe(const Keyframe& keyframe);
    const std::vector<Keyframe>& GetKeyframes() const { return keyframes; }
    void RemoveKeyframe(size_t index);
    void ClearKeyframes();

    // Sampling
    std::unordered_map<std::string, float> Sample(float time) const;
    float SampleProperty(const std::string& property, float time) const;

    // Events
    void AddEvent(float time, const std::string& eventName);
    std::vector<std::pair<float, std::string>> GetEventsInRange(float startTime, float endTime) const;

private:
    std::string name;
    float duration;
    LoopMode loopMode;
    std::vector<Keyframe> keyframes;
    std::vector<std::pair<float, std::string>> events;

    // Interpolation helpers
    float InterpolateLinear(float start, float end, float t) const;
    float InterpolateCubic(float start, float end, float t, float tangentStart, float tangentEnd) const;
    float InterpolateBezier(float start, float end, float t, const std::vector<float>& controlPoints) const;
};

// Animation transition - how we move between animations
struct AnimationTransition {
    std::string fromState;
    std::string toState;
    float duration;
    InterpolationType interpolation;
    std::function<bool()> condition; // when to trigger this transition
    bool hasExitTime;
    float exitTime;
};

// Animation layer - for blending multiple animations
class AnimationLayer {
public:
    AnimationLayer(const std::string& name, float weight = 1.0f);
    ~AnimationLayer();

    const std::string& GetName() const { return name; }
    void SetWeight(float weight) { this->weight = weight; }
    float GetWeight() const { return weight; }

    void SetCurrentClip(std::shared_ptr<AnimationClip> clip) { currentClip = clip; }
    std::shared_ptr<AnimationClip> GetCurrentClip() const { return currentClip; }

    void SetPlaybackTime(float time) { playbackTime = time; }
    float GetPlaybackTime() const { return playbackTime; }

    void SetPlaybackSpeed(float speed) { playbackSpeed = speed; }
    float GetPlaybackSpeed() const { return playbackSpeed; }

    void Update(float deltaTime);
    std::unordered_map<std::string, float> GetCurrentValues() const;

private:
    std::string name;
    float weight;
    std::shared_ptr<AnimationClip> currentClip;
    float playbackTime;
    float playbackSpeed;
    bool isPlaying;
};

// The Animator class - our animation maestro
class Animator {
public:
    Animator();
    ~Animator();

    // Animation clips
    void AddClip(std::shared_ptr<AnimationClip> clip);
    std::shared_ptr<AnimationClip> GetClip(const std::string& name) const;
    void RemoveClip(const std::string& name);
    std::vector<std::string> GetClipNames() const;

    // States and transitions
    void AddState(const std::string& stateName, std::shared_ptr<AnimationClip> clip);
    void RemoveState(const std::string& stateName);
    void AddTransition(const AnimationTransition& transition);
    void SetCurrentState(const std::string& stateName);
    const std::string& GetCurrentState() const { return currentState; }

    // Layers
    void AddLayer(std::shared_ptr<AnimationLayer> layer);
    void RemoveLayer(const std::string& layerName);
    std::shared_ptr<AnimationLayer> GetLayer(const std::string& layerName) const;

    // Playback control
    void Play(const std::string& clipName = "");
    void Pause();
    void Stop();
    void SetPlaybackSpeed(float speed);
    float GetPlaybackSpeed() const { return playbackSpeed; }

    // Time control
    void SetTime(float time);
    float GetTime() const { return currentTime; }
    float GetDuration() const;

    // Update
    void Update(float deltaTime);

    // Current values
    std::unordered_map<std::string, float> GetCurrentValues() const;

    // Events
    void SetAnimationEventCallback(std::function<void(const std::string&)> callback);
    void TriggerEvent(const std::string& eventName);

    // Blending
    void SetBlendParameter(const std::string& parameter, float value);
    float GetBlendParameter(const std::string& parameter) const;

    // IK (Inverse Kinematics)
    void EnableIK(bool enable) { ikEnabled = enable; }
    bool IsIKEnabled() const { return ikEnabled; }
    void SetIKTarget(const std::string& boneName, const class Vector3& target);
    void SolveIK();

    // Root motion
    void EnableRootMotion(bool enable) { rootMotionEnabled = enable; }
    bool IsRootMotionEnabled() const { return rootMotionEnabled; }
    class Vector3 GetRootMotionDelta() const;

    // Debugging
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    void DrawDebugInfo();

private:
    // Animation data
    std::unordered_map<std::string, std::shared_ptr<AnimationClip>> clips;
    std::unordered_map<std::string, std::shared_ptr<AnimationClip>> states;
    std::vector<AnimationTransition> transitions;
    std::unordered_map<std::string, std::shared_ptr<AnimationLayer>> layers;

    // Current state
    std::string currentState;
    float currentTime;
    float playbackSpeed;
    bool isPlaying;
    bool isPaused;

    // Blending
    std::unordered_map<std::string, float> blendParameters;

    // IK
    bool ikEnabled;
    std::unordered_map<std::string, class Vector3> ikTargets;

    // Root motion
    bool rootMotionEnabled;
    class Vector3 lastRootPosition;

    // Events
    std::function<void(const std::string&)> eventCallback;

    // Debug
    bool debugDraw;

    // Internal helpers
    void UpdateTransitions();
    void UpdateLayers(float deltaTime);
    std::unordered_map<std::string, float> BlendLayerValues() const;
    void ProcessEvents(float deltaTime);
    void UpdateRootMotion();
};

#endif // ANIMATOR_H
