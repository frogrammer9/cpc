# CMakeList.txt : CMake project for cpc, include source and define
# project specific logic here.
#

# Add source to this project's executable.
file(GLOB_RECURSE CPC_SRC "*.cpp" "*.hpp")

add_executable(cpc ${CPC_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpc PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
