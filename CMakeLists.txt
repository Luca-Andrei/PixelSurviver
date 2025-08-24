cmake_minimum_required(VERSION 3.26)

# Set CMake policies to suppress warnings
cmake_policy(SET CMP0077 NEW)
# cmake_policy(SET CMP0177 NEW)  # Commented out due to compatibility issues

project(oop)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

set(WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(SFML)


find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()


add_executable(oop
        main.cpp
        include/Game.cpp
        include/Hero.cpp
        include/Monster.cpp
        include/Ability.cpp
        include/XP.cpp
        include/Entity.cpp
        include/Error.cpp
        include/GameObserver.cpp
        include/AbilityFactory.cpp
        include/GameLogger.cpp

        generated/src/Helper.cpp
)

include(cmake/CompilerFlags.cmake)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system Threads::Threads)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

if (WIN32 AND SFML_BUILD_AUDIO)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
         ${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll
         $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()


include(cmake/CopyHelper.cmake)

# Copy assets folder to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
        COMMENT "Copying assets folder to build directory"
)

# Copy tastatura.txt
configure_file(${CMAKE_SOURCE_DIR}/tastatura.txt ${CMAKE_BINARY_DIR}/tastatura.txt COPYONLY)