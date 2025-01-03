cmake_minimum_required(VERSION 3.10)

# Project name and version
project(InferenceC VERSION 1.0 LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# Find all source files in the src directory
file(GLOB_RECURSE SOURCES "src/*.c")
# Define the executable
add_executable(InferenceC ${SOURCES})

# Include FAISS
set(FAISS_DIR "D:/AI/libs/faiss")
include_directories(${FAISS_DIR})
link_directories(${FAISS_DIR}/build/faiss/Release)

# Include MKL
set(MKLROOT "D:/AI/libs/Intel/oneAPI/mkl/latest")
include_directories(${MKLROOT}/include)
link_directories(${MKLROOT}/lib)

# Include gflags
set(gflags_DIR "D:/AI/libs/gflags/custom-build")
include_directories(${gflags_DIR}/include)
link_directories(${gflags_DIR}/lib)

# Include directories for llama and ggml
set(LLamaDir D:/AI/libs/llama-c/llama.cpp)
include_directories(
    ${LLamaDir}/include
    ${LLamaDir}/src
    ${LLamaDir}/ggml/include
    ${LLamaDir}/ggml/src
)

# OpenAI API
set(CURL_PATH D:/AI/libs/curl-8.11.1/builds/libcurl-vc-x64-release-dll-ipv6-sspi-schannel)
set(JSONC_PATH D:/AI/libs)

# Include directories for libcurl and json-c
include_directories(${CURL_PATH}/include)
include_directories(${JSONC_PATH}/json-c)
include_directories(${JSONC_PATH}/json-c-build)

# Link directories for libcurl and json-c
link_directories(${CURL_PATH}/lib)
link_directories(${JSONC_PATH}/json-c-build/Release)

# Link libraries
target_link_libraries(InferenceC
    # faiss libraries
    ${FAISS_DIR}/build/c_api/Release/faiss_c.lib
	${FAISS_DIR}/build/faiss/Release/faiss.lib
    ${MKLROOT}/lib/mkl_intel_lp64.lib
    ${MKLROOT}/lib/mkl_sequential.lib
    ${MKLROOT}/lib/mkl_core.lib
    ${gflags_DIR}/lib/Release/gflags_static.lib
    # llama and ggml libraries
    ${LLamaDir}/build/src/Release/llama.lib
    ${LLamaDir}/build/ggml/src/Release/ggml.lib
    # OpenAI libraries
    ${CURL_PATH}/lib/libcurl.lib
    ${JSONC_PATH}/json-c-build/Release/json-c.lib
)

# Specify the DLL files to copy
set(DLL_FILES
    # curl
    "${CURL_PATH}/bin/libcurl.dll"
    # json-c
    "${JSONC_PATH}/json-c-build/Release/json-c.dll"
    # llama.cpp
    "${LLamaDir}/build/bin/Release/llama.dll"
    "${LLamaDir}/build/bin/Release/ggml.dll"
    "${LLamaDir}/build/bin/Release/ggml-cuda.dll"
    "${LLamaDir}/build/bin/Release/ggml-cpu.dll"
    "${LLamaDir}/build/bin/Release/ggml-base.dll"
)

# Add a custom target to copy the DLL file
add_custom_target(copy_dlls ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${DLL_FILES} ${CMAKE_BINARY_DIR}
    DEPENDS ${DLL_FILES}
)

# Ensure the DLLs are copied before building the executable
add_dependencies(InferenceC copy_dlls)
