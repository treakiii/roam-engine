// AudioEngine.h - The sound master
// This makes your games loud and proud

#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Math/Vector3.h"

// Audio states - what's playing?
enum class AudioState {
    Stopped,
    Playing,
    Paused,
    Loading
};

// Audio types - what kind of sound?
enum class AudioType {
    SoundEffect,
    Music,
    Voice,
    Ambient,
    UI
};

// Audio formats - how it's stored?
enum class AudioFormat {
    WAV,
    MP3,
    OGG,
    FLAC,
    Custom
};

// Audio source - where the sound comes from
class AudioSource {
public:
    AudioSource();
    ~AudioSource();

    // Playback control
    void Play();
    void Pause();
    void Stop();
    void SetLoop(bool loop);
    void SetVolume(float volume);
    void SetPitch(float pitch);

    // 3D positioning
    void SetPosition(const Vector3& position);
    void SetVelocity(const Vector3& velocity);
    void SetDirection(const Vector3& direction);

    // State
    AudioState GetState() const { return state; }
    bool IsPlaying() const { return state == AudioState::Playing; }
    bool IsLooping() const { return looping; }

    // Properties
    float GetVolume() const { return volume; }
    float GetPitch() const { return pitch; }
    const Vector3& GetPosition() const { return position; }

private:
    AudioState state;
    bool looping;
    float volume;
    float pitch;
    Vector3 position;
    Vector3 velocity;
    Vector3 direction;

    // Internal audio handle
    void* audioHandle;
};

// Audio listener - where you hear from
class AudioListener {
public:
    AudioListener();
    ~AudioListener();

    // Listener position/orientation
    void SetPosition(const Vector3& position);
    void SetVelocity(const Vector3& velocity);
    void SetOrientation(const Vector3& forward, const Vector3& up);

    // Properties
    const Vector3& GetPosition() const { return position; }
    const Vector3& GetVelocity() const { return velocity; }

private:
    Vector3 position;
    Vector3 velocity;
    Vector3 forward;
    Vector3 up;
};

// The AudioEngine class - our sound overlord
class AudioEngine {
public:
    static AudioEngine& GetInstance();

    // Engine lifecycle
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    // Audio source management
    AudioSource* CreateSource(const std::string& audioFile);
    void DestroySource(AudioSource* source);

    // Audio loading
    bool LoadAudio(const std::string& name, const std::string& filePath);
    bool UnloadAudio(const std::string& name);

    // Listener management
    AudioListener* GetListener() { return &listener; }

    // Global audio settings
    void SetMasterVolume(float volume);
    void SetMusicVolume(float volume);
    void SetSFXVolume(float volume);
    void SetVoiceVolume(float volume);

    float GetMasterVolume() const { return masterVolume; }
    float GetMusicVolume() const { return musicVolume; }
    float GetSFXVolume() const { return sfxVolume; }
    float GetVoiceVolume() const { return voiceVolume; }

    // Audio effects
    void SetReverb(float amount);
    void SetEcho(float delay, float decay);
    void SetLowPassFilter(float cutoff);
    void SetHighPassFilter(float cutoff);

    // Performance
    void SetMaxSources(int maxSources);
    int GetActiveSources() const;

    // Debug
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    void DrawDebugInfo();

private:
    AudioEngine();
    ~AudioEngine();

    // Prevent copying
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    // Audio sources
    std::vector<std::unique_ptr<AudioSource>> sources;

    // Loaded audio data
    std::unordered_map<std::string, void*> loadedAudio;

    // Listener
    AudioListener listener;

    // Global settings
    float masterVolume;
    float musicVolume;
    float sfxVolume;
    float voiceVolume;

    // Effects
    float reverbAmount;
    float echoDelay;
    float echoDecay;
    float lowPassCutoff;
    float highPassCutoff;

    // Performance
    int maxSources;

    // Debug
    bool debugDraw;
    bool initialized;
};

#endif // AUDIOENGINE_H
