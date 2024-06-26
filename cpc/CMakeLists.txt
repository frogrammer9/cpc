# CMakeList.txt : CMake project for cpc, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cpc 
"cpc.cpp" 
"lexer.cpp"
"lexer.hpp"
"langSpecs.hpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpc PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
