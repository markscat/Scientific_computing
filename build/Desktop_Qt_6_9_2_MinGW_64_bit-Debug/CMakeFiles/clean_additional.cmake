# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Scientific_computing_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Scientific_computing_autogen.dir\\ParseCache.txt"
  "Scientific_computing_autogen"
  )
endif()
