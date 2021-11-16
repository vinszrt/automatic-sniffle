#include "openglwindow.hpp"

#include <imgui.h>

#include <algorithm>
#include <cppitertools/itertools.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "camera.hpp"

// Controle o uso do teclado para controlar o movimento dentro do labirinto
// Teclas UP, DOWN, W, S controlam movimento para frente e pra tras.
// Teclas Q, E movimenta lateralmente.
// Teclas LEFT, RIGHT giram a camera.
void OpenGLWindow::handleEvent(SDL_Event &ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.5f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.5f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.5f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.5f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.5f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.5f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "depth.vert",
                                    getAssetsPath() + "depth.frag");

  // Create ground
  m_ground.initializeGL(m_program);

  // Load model
  m_model.loadObj(getAssetsPath() + "box.obj");

  m_model.setupVAO(m_program);

  // Setup maze walls
  loadMaze();
}

// Carrega as paredes que compõe o labirinto
// Considerando uma grade 19x19 e
// Considerando um bloco 1x1
// Se uma posição x,y da grade contém parede, um bloco é inserido centrado na
// posição
void OpenGLWindow::loadMaze() {
  // Representa os blocos que compõe o labirinto e suas posições
  std::vector<glm::vec2> walls_positions = {
      {-9.0f, -18.0f}, {-9.0f, -17.0f}, {-9.0f, -16.0f}, {-9.0f, -15.0f},
      {-9.0f, -14.0f}, {-9.0f, -13.0f}, {-9.0f, -12.0f}, {-9.0f, -11.0f},
      {-9.0f, -10.0f}, {-9.0f, -9.0f},  {-9.0f, -8.0f},  {-9.0f, -7.0f},
      {-9.0f, -6.0f},  {-9.0f, -5.0f},  {-9.0f, -4.0f},  {-9.0f, -3.0f},
      {-9.0f, -2.0f},  {-9.0f, -1.0f},  {-9.0f, -0.0f},  {-8.0f, -18.0f},
      {-8.0f, -0.0f},  {-7.0f, -18.0f}, {-7.0f, -16.0f}, {-7.0f, -15.0f},
      {-7.0f, -14.0f}, {-7.0f, -12.0f}, {-7.0f, -11.0f}, {-7.0f, -10.0f},
      {-7.0f, -9.0f},  {-7.0f, -7.0f},  {-7.0f, -6.0f},  {-7.0f, -4.0f},
      {-7.0f, -3.0f},  {-7.0f, -2.0f},  {-7.0f, -0.0f},  {-6.0f, -18.0f},
      {-6.0f, -16.0f}, {-6.0f, -9.0f},  {-6.0f, -4.0f},  {-6.0f, -2.0f},
      {-6.0f, -0.0f},  {-5.0f, -18.0f}, {-5.0f, -16.0f}, {-5.0f, -14.0f},
      {-5.0f, -13.0f}, {-5.0f, -12.0f}, {-5.0f, -11.0f}, {-5.0f, -9.0f},
      {-5.0f, -8.0f},  {-5.0f, -7.0f},  {-5.0f, -6.0f},  {-5.0f, -4.0f},
      {-5.0f, -2.0f},  {-5.0f, -1.0f},  {-5.0f, -0.0f},  {-4.0f, -18.0f},
      {-4.0f, -16.0f}, {-4.0f, -14.0f}, {-4.0f, -12.0f}, {-4.0f, -11.0f},
      {-4.0f, -0.0f},  {-3.0f, -18.0f}, {-3.0f, -16.0f}, {-3.0f, -14.0f},
      {-3.0f, -11.0f}, {-3.0f, -9.0f},  {-3.0f, -7.0f},  {-3.0f, -6.0f},
      {-3.0f, -5.0f},  {-3.0f, -4.0f},  {-3.0f, -3.0f},  {-3.0f, -2.0f},
      {-3.0f, -0.0f},  {-2.0f, -18.0f}, {-2.0f, -14.0f}, {-2.0f, -12.0f},
      {-2.0f, -11.0f}, {-2.0f, -9.0f},  {-2.0f, -7.0f},  {-2.0f, -2.0f},
      {-2.0f, -1.0f},  {-2.0f, -0.0f},  {-1.0f, -18.0f}, {-1.0f, -17.0f},
      {-1.0f, -16.0f}, {-1.0f, -11.0f}, {-1.0f, -9.0f},  {-1.0f, -7.0f},
      {-1.0f, -5.0f},  {-1.0f, -4.0f},  {-1.0f, -2.0f},  {-1.0f, -1.0f},
      {-1.0f, -0.0f},  {0.0f, -14.0f},  {0.0f, -13.0f},  {0.0f, -11.0f},
      {0.0f, -9.0f},   {0.0f, -7.0f},   {0.0f, -5.0f},   {1.0f, -18.0f},
      {1.0f, -17.0f},  {1.0f, -15.0f},  {1.0f, -14.0f},  {1.0f, -13.0f},
      {1.0f, -12.0f},  {1.0f, -11.0f},  {1.0f, -9.0f},   {1.0f, -7.0f},
      {1.0f, -5.0f},   {1.0f, -3.0f},   {1.0f, -2.0f},   {1.0f, -1.0f},
      {1.0f, -0.0f},   {2.0f, -18.0f},  {2.0f, -17.0f},  {2.0f, -15.0f},
      {2.0f, -13.0f},  {2.0f, -9.0f},   {2.0f, -7.0f},   {2.0f, -5.0f},
      {2.0f, -0.0f},   {3.0f, -18.0f},  {3.0f, -13.0f},  {3.0f, -11.0f},
      {3.0f, -10.0f},  {3.0f, -9.0f},   {3.0f, -7.0f},   {3.0f, -5.0f},
      {3.0f, -4.0f},   {3.0f, -2.0f},   {3.0f, -0.0f},   {4.0f, -18.0f},
      {4.0f, -16.0f},  {4.0f, -14.0f},  {4.0f, -13.0f},  {4.0f, -11.0f},
      {4.0f, -5.0f},   {4.0f, -4.0f},   {4.0f, -2.0f},   {4.0f, -0.0f},
      {5.0f, -18.0f},  {5.0f, -16.0f},  {5.0f, -14.0f},  {5.0f, -13.0f},
      {5.0f, -11.0f},  {5.0f, -9.0f},   {5.0f, -8.0f},   {5.0f, -7.0f},
      {5.0f, -5.0f},   {5.0f, -2.0f},   {5.0f, -0.0f},   {6.0f, -18.0f},
      {6.0f, -17.0f},  {6.0f, -16.0f},  {6.0f, -14.0f},  {6.0f, -13.0f},
      {6.0f, -5.0f},   {6.0f, -3.0f},   {6.0f, -2.0f},   {6.0f, -0.0f},
      {7.0f, -18.0f},  {7.0f, -13.0f},  {7.0f, -12.0f},  {7.0f, -11.0f},
      {7.0f, -9.0f},   {7.0f, -8.0f},   {7.0f, -7.0f},   {7.0f, -6.0f},
      {7.0f, -5.0f},   {7.0f, -3.0f},   {7.0f, -2.0f},   {7.0f, -0.0f},
      {8.0f, -18.0f},  {8.0f, -16.0f},  {8.0f, -15.0f},  {8.0f, -9.0f},
      {8.0f, -8.0f},   {8.0f, -3.0f},   {8.0f, -0.0f},   {9.0f, -18.0f},
      {9.0f, -17.0f},  {9.0f, -16.0f},  {9.0f, -15.0f},  {9.0f, -14.0f},
      {9.0f, -13.0f},  {9.0f, -12.0f},  {9.0f, -11.0f},  {9.0f, -10.0f},
      {9.0f, -9.0f},   {9.0f, -8.0f},   {9.0f, -7.0f},   {9.0f, -6.0f},
      {9.0f, -5.0f},   {9.0f, -4.0f},   {9.0f, -3.0f},   {9.0f, -2.0f},
      {9.0f, -1.0f},   {9.0f, -0.0f}};

  auto *position = &(m_wallPositions.at(0));

  // Insere os blocos nas posições
  for (unsigned long i = 0; i < walls_positions.size(); i++) {
    position = &(m_wallPositions.at(i));
    *position =
        glm::vec3(walls_positions.at(i)[0], 0.8f, walls_positions.at(i)[1]);
  }
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  // Render each wall
  for (const auto index : iter::range(m_walls)) {
    const auto &position{m_wallPositions.at(index)};

    // Compute model matrix of the current wall
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix =
        glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.87f, 1.6f, 0.87f));

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 0.4f, 1.0f);

    m_model.render();
  }

  // Draw ground
  m_ground.paintGL();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2(((float)m_viewportWidth - size.x) / 2.0f,
                               ((float)m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    // Se jogador chegou ao fim do labirinto, exibe *You Win!*
    if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ground.terminateGL();
  m_model.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state == State::Win && m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  if (m_gameData.m_state == State::Playing) {
    makeMovement(deltaTime);
    checkWinCondition();
  }
}

void OpenGLWindow::makeMovement(const float deltaTime) {
  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime, m_wallPositions);
  m_camera.truck(m_truckSpeed * deltaTime, m_wallPositions);
  m_camera.pan(m_panSpeed * deltaTime);
}

// Verifica se o jogador chegou ao fim do labirinto
void OpenGLWindow::checkWinCondition() {
  if (m_camera.m_eye[2] < -19.0f) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_camera.restart();
}