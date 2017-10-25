cmake_minimum_required(VERSION 2.6)
project(STG_Game)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${STG_Game_SOURCE_DIR}/cmake")
set(BIN_DIR ${STG_Game_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -g")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
                string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(include)


#if (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
#	file(COPY "${PROJECT_SOURCE_DIR}/res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#endif()


find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})


set(SOURCE_FILES
    include/res_path.h      include/cleanup.h
    include/SDL2_header.h   src/SDL2_header.cpp
    include/pointd.h
    src/main.cpp )

if (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
        file(COPY "${PROJECT_SOURCE_DIR}/res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(STG_Game ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS STG_Game RUNTIME DESTINATION ${BIN_DIR})

