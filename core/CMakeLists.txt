set(EXECUTABLE_NAME core)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE SOURCES bin/*.cpp src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

add_executable(${EXECUTABLE_NAME})

target_sources(${EXECUTABLE_NAME} 
    PRIVATE
        ${SOURCES}
    PUBLIC
        ${HEADERS}
)

target_link_libraries(${EXECUTABLE_NAME} 
    PRIVATE 
        ${OpenCV_LIBS}
)

target_include_directories(${EXECUTABLE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/${EXECUTABLE_NAME}
        >
        $<INSTALL_INTERFACE:include/>
)
