///
/// @file Camera.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Camera class
///

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace KuchCraft {

	class Camera
	{
	public:
		/// Default constructor that calculates default view and projection matrixes
		Camera()
		{
			UpdateView();
			UpdateProjection();
		}

		/// Default destructor
		~Camera() = default;

		/// Gets the camera's projection matrix.
		/// @return A constant reference to the projection matrix.
		inline [[nodiscard]] const glm::mat4& GetProjection() const { return m_Projection; }

		/// Gets the camera's view matrix.
		/// @return A constant reference to the view matrix.
		inline [[nodiscard]] const glm::mat4& GetView() const { return m_View; }

		/// Gets the combined view-projection matrix.
		/// @return A constant reference to the view-projection matrix.
		inline [[nodiscard]] const glm::mat4& GetViewProjection() const { return m_ViewProjection; }

		/// Gets the camera's position in 3D space.
		/// @return A constant reference to the position vector.
		inline [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }

		/// Gets the camera's rotation in Euler angles.
		/// @return A constant reference to the rotation vector.
		inline [[nodiscard]] const glm::vec3& GetRotation() const { return m_Rotation; }

		/// Gets the camera's aspect ratio.
		/// @return The aspect ratio of the camera.
		inline [[nodiscard]] float GetAspectRatio() const { return m_AspectRatio; }

		/// Gets the camera's field of view.
		/// @return The field of view of the camera in radians.
		inline [[nodiscard]] float GetFov() const { return m_Fov; }

		/// Gets the camera's near clip.
		/// @return The near clip of the camera.
		inline [[nodiscard]] float GetNearClip() const { return m_NearClip; }

		/// Gets the camera's far clip.
		/// @return The far clip of the camera.
		inline [[nodiscard]] float GetFarClip() const { return m_FarClip; }

		/// Gets the camera's up direction vector based on its rotation.
		/// @return The up direction vector.
		inline [[nodiscard]] glm::vec3 GetUpDirection() const { return glm::quat(m_Rotation) * glm::vec3(0.0f, 1.0f, 0.0f); }

		/// Gets the camera's right direction vector based on its rotation.
		/// @return The right direction vector.
		inline [[nodiscard]] glm::vec3 GetRightDirection() const { return glm::quat(m_Rotation) * glm::vec3(1.0f, 0.0f, 0.0f); }

		/// Gets the camera's forward direction vector based on its rotation.
		/// @return The forward direction vector.
		inline [[nodiscard]] glm::vec3 GetForwardDirection() const { return glm::quat(m_Rotation) * glm::vec3(0.0f, 0.0f, -1.0f); }

		/// Sets the camera's position and rotation.
		/// This function updates the camera's position and rotation, then recalculates the view matrix.
		/// @param position - the position of the camera in 3D space.
		/// @param rotation - the rotation of the camera as a vec3 (Euler angles).
		inline void SetData(const glm::vec3& position, const glm::vec3& rotation) { m_Position = position; m_Rotation = rotation; UpdateView(); }

		/// Sets the camera's position
		/// This function updates the camera's position, then recalculates the view matrix.
		/// @param position - the position of the camera in 3D space.
		inline void SetPosition(const glm::vec3& position) {
			m_Position = position;
			UpdateView();
		}

		/// Sets the camera's rotation
		/// This function updates the camera's rotation, then recalculates the view matrix.
		/// @param rotation - the rotation of the camera as a vec3 (Euler angles).
		inline void SetRotation(const glm::vec3& rotation) {
			m_Rotation = rotation;
			UpdateView();
		}

		/// Sets the camera's projection parameters and updates the projection matrix.
		/// @param fov - the field of view in radians.
		/// @param aspectRatio - the aspect ratio of the camera (width/height).
		/// @param nearClip - the near clipping plane distance.
		/// @param farClip - the far clipping plane distance.
		inline void SetProjectionData(float fov, float aspectRatio, float nearClip, float farClip) {
			m_Fov         = fov;
			m_AspectRatio = aspectRatio;
			m_NearClip    = nearClip;
			m_FarClip     = farClip;
			UpdateProjection();
		}

		/// Sets the camera's field of view and updates the projection matrix.
		/// @param fov - the field of view in radians.
		inline void SetFov(float fov) { 
			m_Fov = fov;
			UpdateProjection();
		}

		/// Sets the camera's aspect ratio and updates the projection matrix.
		/// @param aspectRatio - the aspect ratio of the camera (width/height).
		inline void SetAspectRatio(float aspectRatio) {
			m_AspectRatio = aspectRatio;
			UpdateProjection(); 
		}

		/// Sets the near clipping plane and updates the projection matrix.
		/// @param nearClip - the near clipping plane distance.
		inline void SetNearClip(float nearClip) {
			m_NearClip = nearClip; 
			UpdateProjection(); 
		}

		/// Sets the far clipping plane and updates the projection matrix.
		/// @param farClip - the far clipping plane distance.
		inline void SetFarClip(float farClip) {
			m_FarClip = farClip; 
			UpdateProjection();
		}

	private:
		/// Updates the camera projection and view-projection matrixes.
		inline void UpdateProjection() {
			m_Projection     = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
			m_ViewProjection = m_Projection * m_View;
		}

		/// Updates the camera view and view-projection matrixes.
		inline void UpdateView() {
			m_View = glm::lookAt(m_Position, m_Position + GetForwardDirection(), GetUpDirection()); 
			m_ViewProjection = m_Projection * m_View;
		}

	private:
		/// The field of view in radians.
		float m_Fov = glm::radians(45.0f);

		/// The aspect ratio (width/height).
		float m_AspectRatio = 16.0f / 9.0f;

		/// The near clipping plane distance.
		float m_NearClip = 0.01f;

		/// The far clipping plane distance.
		float m_FarClip = 1000.0f;

		/// The position of the camera.
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		/// The rotation of the camera (Euler angles).
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

		/// The projection matrix.
		glm::mat4 m_Projection = { 1.0f };

		/// The view matrix.
		glm::mat4 m_View = { 1.0f };

		/// The combined view-projection matrix.
		glm::mat4 m_ViewProjection = { 1.0f };

	};

}