# CMakeList.txt: проект CMake для ControlPanel; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (ControlPanel "ControlPanel.cpp" "ControlPanel.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ControlPanel PROPERTY CXX_STANDARD 20)
endif()

# Установите переменные для библиотек SFML
set(SFML_LIBRARIES
    ${CMAKE_SOURCE_DIR}/library/sfml-graphics.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-audio.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-network.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-system.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-window.lib
)

set(SFML_LIBRARIES_DEBUG
    ${CMAKE_SOURCE_DIR}/library/sfml-graphics-d.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-audio-d.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-network-d.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-system-d.lib
    ${CMAKE_SOURCE_DIR}/library/sfml-window-d.lib
)

# Привяжите библиотеки в зависимости от конфигурации
target_link_libraries(ControlPanel optimized ${SFML_LIBRARIES} debug ${SFML_LIBRARIES_DEBUG})

include_directories(${CMAKE_SOURCE_DIR}/include)

set_property(TARGET ControlPanel PROPERTY CXX_STANDARD 17)
set_property(TARGET ControlPanel PROPERTY CXX_STANDARD_REQUIRED ON)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
