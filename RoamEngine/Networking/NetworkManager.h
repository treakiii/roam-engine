// NetworkManager.h - The internet wizard
// This makes your games talk to each other, hopefully without crashing

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

// Network states - are we online or just pretending?
enum class NetworkState {
    Disconnected,
    Connecting,
    Connected,
    Authenticating,
    InGame,
    Error
};

// Connection types - how are we connecting?
enum class ConnectionType {
    Host,
    Client,
    Peer,
    DedicatedServer
};

// Message types - what are we saying?
enum class MessageType {
    Handshake,
    GameData,
    Chat,
    Command,
    Ping,
    Disconnect,
    Custom
};

// Network message - the digital envelope
struct NetworkMessage {
    MessageType type;
    uint32_t senderId;
    uint32_t receiverId;
    std::vector<char> data;
    uint64_t timestamp;
    bool reliable;
    uint32_t sequenceNumber;

    NetworkMessage() : senderId(0), receiverId(0), timestamp(0), reliable(false), sequenceNumber(0) {}
};

// Player info - who's playing?
struct PlayerInfo {
    uint32_t playerId;
    std::string playerName;
    std::string ipAddress;
    uint16_t port;
    bool isHost;
    bool isReady;
    float ping;
    NetworkState state;

    PlayerInfo() : playerId(0), port(0), isHost(false), isReady(false), ping(0.0f), state(NetworkState::Disconnected) {}
};

// Lobby info - where the party is
struct LobbyInfo {
    std::string lobbyName;
    std::string gameMode;
    uint32_t maxPlayers;
    uint32_t currentPlayers;
    bool isPrivate;
    std::string password;
    std::vector<PlayerInfo> players;

    LobbyInfo() : maxPlayers(8), currentPlayers(0), isPrivate(false) {}
};

// Network statistics - how's the connection?
struct NetworkStats {
    uint32_t bytesSent;
    uint32_t bytesReceived;
    uint32_t packetsSent;
    uint32_t packetsReceived;
    uint32_t packetsLost;
    float averagePing;
    float packetLossRate;

    NetworkStats() : bytesSent(0), bytesReceived(0), packetsSent(0), packetsReceived(0),
                    packetsLost(0), averagePing(0.0f), packetLossRate(0.0f) {}
};

// The NetworkManager class - our digital social coordinator
class NetworkManager {
public:
    static NetworkManager& GetInstance();

    // Initialization - plug in the ethernet cable
    bool Initialize();
    void Shutdown();

    // Connection management - make friends online
    bool HostGame(const std::string& gameName, uint16_t port, uint32_t maxPlayers);
    bool JoinGame(const std::string& ipAddress, uint16_t port);
    void Disconnect();
    NetworkState GetState() const { return currentState; }

    // Lobby management - organize the party
    bool CreateLobby(const LobbyInfo& lobbyInfo);
    bool JoinLobby(const std::string& lobbyId);
    void LeaveLobby();
    std::vector<LobbyInfo> GetAvailableLobbies();

    // Player management - who's here?
    const std::vector<PlayerInfo>& GetConnectedPlayers() const { return connectedPlayers; }
    PlayerInfo* GetLocalPlayer();
    PlayerInfo* GetPlayerById(uint32_t playerId);

    // Message sending - talk to your friends
    void SendMessage(const NetworkMessage& message);
    void SendMessageToPlayer(uint32_t playerId, const NetworkMessage& message);
    void BroadcastMessage(const NetworkMessage& message);

    // Message receiving - listen to gossip
    void RegisterMessageHandler(MessageType type, std::function<void(const NetworkMessage&)> handler);
    void UnregisterMessageHandler(MessageType type);

    // NAT punchthrough - sneak through firewalls
    bool EnableNATTraversal();
    void DisableNATTraversal();

    // Matchmaking - find people to play with
    void StartMatchmaking(const std::string& gameMode, uint32_t maxPlayers);
    void StopMatchmaking();
    bool IsMatchmaking() const { return isMatchmaking; }

    // Voice chat - talk while you play
    bool EnableVoiceChat();
    void DisableVoiceChat();
    void SetVoiceVolume(float volume);
    void MutePlayer(uint32_t playerId, bool mute);

    // Network statistics - how's the internet?
    const NetworkStats& GetNetworkStats() const { return networkStats; }
    void ResetNetworkStats();

    // Error handling - when things go wrong
    std::string GetLastError() const { return lastError; }
    void ClearError();

    // Settings - tweak the connection
    void SetMaxPacketSize(uint32_t size) { maxPacketSize = size; }
    void SetTimeout(uint32_t milliseconds) { timeout = milliseconds; }
    void SetInterpolationDelay(uint32_t milliseconds) { interpolationDelay = milliseconds; }

    // Update - keep the conversation going
    void Update(float deltaTime);

    // Debug - see what's happening
    void EnableDebugLogging(bool enable) { debugLogging = enable; }
    void DrawDebugInfo();

private:
    NetworkManager();
    ~NetworkManager();

    // Prevent copying - one network manager is enough
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    // Internal state
    NetworkState currentState;
    ConnectionType connectionType;
    std::vector<PlayerInfo> connectedPlayers;
    std::unordered_map<MessageType, std::vector<std::function<void(const NetworkMessage&)>>> messageHandlers;
    NetworkStats networkStats;
    std::string lastError;

    // Settings
    uint32_t maxPacketSize;
    uint32_t timeout;
    uint32_t interpolationDelay;
    bool natTraversalEnabled;
    bool voiceChatEnabled;
    bool isMatchmaking;
    bool debugLogging;

    // Internal helpers
    void ProcessIncomingMessages();
    void SendHeartbeat();
    void UpdateNetworkStats();
    void HandleConnectionLost(uint32_t playerId);
    void HandleNewConnection(uint32_t playerId);
};

#endif // NETWORKMANAGER_H
