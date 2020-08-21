cmake_minimum_required(VERSION 3.1)
project(SimpleNativeAddon)

set(CMAKE_CXX_STANDARD 14)

SET(NODE_RUNTIME electron)
SET(NODE_RUNTIMEVERSION 9.2.1)
SET(NODE_ARCH x64)


SET(CMAKE_JS_INC
        $ENV{HOME}/.cmake-js/electron-x64/v${NODE_RUNTIMEVERSION}/include/node
        ${CMAKE_CURRENT_LIST_DIR}/node_modules/nan)

if (MSVC)
    SET(CMAKE_JS_LIB
            $ENV{HOME}/.cmake-js/electron-x64/v${NODE_RUNTIMEVERSION}/x64/node.lib)
    SET(CMAKE_JS_SRC
            ${CMAKE_CURRENT_LIST_DIR}/node_modules/cmake-js/lib/cpp/win_delay_load_hook.cc)
    set(CMAKE_SHARED_LINKER_FLAGS
            /DELAYLOAD:NODE.EXE)
endif()

if(APPLE)
    SET(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} "-undefined dynamic_lookup")
endif()

if (MSVC)
    add_compile_options(/MT /W3 /O2 /Oi /Ob2 /Ot /Oy /Gy /GL /EHa /GS /GF) # /Ob2 -> /Ob0 to fix the crash with inline functions
    add_link_options(/NOLOGO /LTCG:incremental /OPT:REF /OPT:ICF /NODEFAULTLIB:library)
endif()

if (UNIX)
    add_library(SimpleNativeAddon MODULE library.cpp ${CMAKE_JS_SRC})
else()
    add_library(SimpleNativeAddon SHARED library.cpp ${CMAKE_JS_SRC})
endif()

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(SimpleNativeAddon PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(SimpleNativeAddon PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(SimpleNativeAddon ${CMAKE_JS_LIB})
