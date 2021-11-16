project(projeto-labirinto)
add_executable(${PROJECT_NAME} main.cpp model.cpp camera.cpp ground.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})
