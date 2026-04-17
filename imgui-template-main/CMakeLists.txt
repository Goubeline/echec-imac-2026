# # ## -----------------------------------------------------------------------
# # ##
# # ## If you want to learn CMake:
# # ## https://julesfouchy.github.io/Learn--Clean-Code-With-Cpp/lessons/cmake/
# # ##
# # ## If you want to see how to add libraries to your project:
# # ## https://julesfouchy.github.io/Learn--Clean-Code-With-Cpp/lessons/use-libraries/#how-to
# # ##
# # ## -----------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(ImGuiTemplate)

# --- AUTORISATION GLM ---
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
    CXX_EXTENSIONS OFF)

# --- FICHIERS SOURCES ---
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})

# --- DOSSIERS D'INCLUSION ---
target_include_directories(${PROJECT_NAME} PRIVATE src)


# --- SOUS-DOSSIERS ---
add_subdirectory(lib/quick_imgui)


# --- GLM ---
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1  
)
FetchContent_MakeAvailable(glm)

# --- LIAISON ---
target_link_libraries(${PROJECT_NAME} PRIVATE quick_imgui::quick_imgui glm )