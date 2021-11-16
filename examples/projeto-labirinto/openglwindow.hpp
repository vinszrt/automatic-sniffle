#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>

#include "abcg.hpp"
#include "camera.hpp"
#include "ground.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_walls{2};

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  Ground m_ground;

  // std::default_random_engine m_randomEngine;

  Model m_model;

  std::array<glm::vec3, m_walls> m_wallPositions;

  void loadMaze();
  void update();
};

#endif