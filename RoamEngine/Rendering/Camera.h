// Camera.h - The eye of the engine
// This defines how we view the world

#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

// Camera types - perspective or orthographic
enum class CameraType {
    Perspective,
    Orthographic
};

// The Camera class - our window to the world
class Camera {
public:
    Camera();
    ~Camera();

    // Set camera type - perspective or ortho
    void SetType(CameraType type) { cameraType = type; UpdateProjectionMatrix(); }
    CameraType GetType() const { return cameraType; }

    // Perspective settings - for 3D views
    void SetFieldOfView(float fovDegrees) { fieldOfView = fovDegrees; UpdateProjectionMatrix(); }
    void SetAspectRatio(float aspect) { aspectRatio = aspect; UpdateProjectionMatrix(); }
    void SetNearClip(float nearClip) { this->nearClip = nearClip; UpdateProjectionMatrix(); }
    void SetFarClip(float farClip) { this->farClip = farClip; UpdateProjectionMatrix(); }

    // Orthographic settings - for 2D views
    void SetOrthoSize(float size) { orthoSize = size; UpdateProjectionMatrix(); }

    // Position and orientation - where and how we look
    void SetPosition(const Vector3& position) { this->position = position; UpdateViewMatrix(); }
    void SetRotation(const Vector3& rotation) { this->rotation = rotation; UpdateViewMatrix(); }
    void LookAt(const Vector3& target, const Vector3& up = Vector3(0, 1, 0));

    // Movement - fly around
    void MoveForward(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);

    // Rotation - look around
    void Rotate(float yaw, float pitch);

    // Get matrices - for rendering
    const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
    const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }
    Matrix4x4 GetViewProjectionMatrix() const { return viewMatrix * projectionMatrix; }

    // Get vectors - for calculations
    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;

    // Get position - where are we?
    const Vector3& GetPosition() const { return position; }

    // Get frustum - for culling
    void GetFrustumPlanes(std::vector<Vector4>& planes) const;

    // World to screen - project points
    Vector3 WorldToScreen(const Vector3& worldPoint) const;

    // Screen to world - unproject points
    Vector3 ScreenToWorld(const Vector3& screenPoint) const;

private:
    // Update matrices - recalculate when settings change
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();

    // Camera type
    CameraType cameraType;

    // Perspective parameters
    float fieldOfView;
    float aspectRatio;
    float nearClip;
    float farClip;

    // Orthographic parameters
    float orthoSize;

    // Position and rotation
    Vector3 position;
    Vector3 rotation;

    // Matrices
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
};

#endif // CAMERA_H
