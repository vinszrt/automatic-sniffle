#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <fmt/core.h>

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class OpenGLWindow;

class Camera {
 public:
  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);

  template <std::size_t SIZE>
  void dolly(float speed, std::array<glm::vec3, SIZE> m_wallPositions);

  template <std::size_t SIZE>
  void truck(float speed, std::array<glm::vec3, SIZE> m_wallPositions);

  void pan(float speed);
  void restart();

 private:
  friend OpenGLWindow;

  glm::vec3 m_eye{glm::vec3(0.0f, 0.5f, 2.0f)};  // Camera position
  glm::vec3 m_at{glm::vec3(0.0f, 0.5f, 0.0f)};   // Look-at point
  glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};   // "up" direction

  // Matrix to change from world space to camera soace
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;
};

template <std::size_t SIZE>
void Camera::dolly(float speed, std::array<glm::vec3, SIZE> m_wallPositions) {
  // Compute forward vector (view direction)
  const glm::vec3 forward{glm::normalize(m_at - m_eye)};

  const glm::vec3 m_next_eye = m_eye + forward * speed;
  const glm::vec2 proj_eye = {m_next_eye[0], m_next_eye[2]};

  glm::vec2 proj_wall;
  int collision = 0;

  // Check each wall to see if will collide
  for (long unsigned int i = 0; i < m_wallPositions.size(); i++) {
    proj_wall = {m_wallPositions.at(i)[0], m_wallPositions.at(i)[2]};

    if (glm::distance(proj_eye, proj_wall) < 0.71) {
      collision = 1;
    }
  }

  // Check if camera is out of maze
  if (m_next_eye[0] > 9 || m_next_eye[0] < -9) collision = 1;

  // Move eye and center forward (speed > 0) or backward (speed < 0) if not
  // collided
  if (collision == 0) {
    m_eye += forward * speed;
    m_at += forward * speed;
  }

  computeViewMatrix();
}

template <std::size_t SIZE>
void Camera::truck(float speed, std::array<glm::vec3, SIZE> m_wallPositions) {
  // Compute forward vector (view direction)
  const glm::vec3 forward{glm::normalize(m_at - m_eye)};
  // Compute vector to the left
  const glm::vec3 left{glm::cross(m_up, forward)};

  const glm::vec3 m_next_eye = m_eye - left * speed;
  const glm::vec2 proj_eye = {m_next_eye[0], m_next_eye[2]};

  glm::vec2 proj_wall;
  int collision = 0;

  // Check each wall to see if will collide
  for (long unsigned int i = 0; i < m_wallPositions.size(); i++) {
    proj_wall = {m_wallPositions.at(i)[0], m_wallPositions.at(i)[2]};

    if (glm::distance(proj_eye, proj_wall) < 0.71) {
      collision = 1;
    }
  }

  // Move eye and center to the left (speed < 0) or to the right (speed > 0) if
  // not collided
  if (collision == 0) {
    m_eye -= left * speed;
    m_at -= left * speed;
  }

  computeViewMatrix();
}

#endif