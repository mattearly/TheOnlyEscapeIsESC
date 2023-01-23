cmake_minimum_required(VERSION 3.15)

project(TheOnlyEscape VERSION 1.0)

file(GLOB SRC src/*.cpp src/*.h)

# libs we need:
### -lGL -lGLU -lglut -lGLEW -lSDL2main -lSDL2 -lSDL2_mixer
find_package(GLEW REQUIRED)
find_package(FreeGLUT CONFIG REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

add_executable(escape ${SRC})

target_link_libraries(escape 
   PRIVATE
     $<IF:$<TARGET_EXISTS:FreeGLUT::freeglut>,FreeGLUT::freeglut,FreeGLUT::freeglut_static>
     GLUT::GLUT
     GLEW::GLEW
     glm::glm
     $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
     $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
     $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/sound/ DESTINATION ${CMAKE_BINARY_DIR}/resources/sound/)


#install for distribution
install(TARGETS escape)

file(GLOB runtime_dlls LIST_DIRECTORIES false ${CMAKE_CURRENT_BINARY_DIR}/*.dll)
install(FILES ${runtime_dlls} DESTINATION bin)

file(GLOB runtime_shaders LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
install(FILES ${runtime_shaders} DESTINATION bin/shaders)

file(GLOB runtime_sounds LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/resources/sound/*.wav)
install(FILES ${runtime_sounds} DESTINATION bin/resources/sound)
