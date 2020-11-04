cmake_minimum_required(VERSION 3.1)
project(SimpleNativeAddon)

set(CMAKE_CXX_STANDARD 14)

SET(NODE_RUNTIME electron)
SET(NODE_RUNTIMEVERSION 10.1.5)
SET(NODE_ARCH x64)

if (UNIX)
    SET(CMAKE_JS_INC
            $ENV{HOME}/.cmake-js/electron-x64/v${NODE_RUNTIMEVERSION}/include/node
            ${CMAKE_CURRENT_LIST_DIR}/node_modules/node-addon-api)
endif()

if (MSVC)
    file(TO_CMAKE_PATH "$ENV{USERPROFILE}" ENV_USERPROFILE)

    SET(CMAKE_JS_INC
            ${ENV_USERPROFILE}/.cmake-js/electron-x64/v${NODE_RUNTIMEVERSION}/include/node
            ${ENV_USERPROFILE}/Development/badlionclient/badlionclient/native-modules/launcher/x64/node_modules/nan)

    SET(CMAKE_JS_LIB
            ${ENV_USERPROFILE}/.cmake-js/electron-x64/v${NODE_RUNTIMEVERSION}/x64/node.lib)

    # Will need to be fixed for your local environment
    SET(CMAKE_JS_SRC
            ${ENV_USERPROFILE}/AppData/Roaming/nvm/v12.18.1/node_modules/cmake-js/lib/cpp/win_delay_load_hook.cc)

    set(CMAKE_SHARED_LINKER_FLAGS
            /DELAYLOAD:NODE.EXE)
endif()

if(APPLE)
    SET(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} "-undefined dynamic_lookup")
endif()

if (MSVC)
    # TEST AREA
    add_definitions(-DUSING_UV_SHARED=1 -DUSING_V8_SHARED=1 -DV8_DEPRECATION_WARNINGS=1 -DV8_DEPRECATION_WARNINGS -DV8_IMMINENT_DEPRECATION_WARNINGS -DWIN32 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_HAS_EXCEPTIONS=0 -DOPENSSL_NO_PINSHARED -DOPENSSL_THREADS -DBUILDING_NODE_EXTENSION)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/INCREMENTAL:YES")
    # END TEST AREA

    message("Clearing compiler flags...")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")

    # TEST - /O2 -> /Ox, Added /GR-
    add_compile_options(/MT /W3 /Ox /Oi /Ob2 /Ot /Oy /Gy /GL /EHa /GS /GF /GR-) # /Ob2 -> /Ob0 to fix the crash with inline functions
    add_link_options(/NOLOGO /LTCG:incremental /OPT:REF /OPT:ICF /NODEFAULTLIB:library)
endif()

if (UNIX)
    add_library(SimpleNativeAddon MODULE)
else()
    add_library(SimpleNativeAddon SHARED)
endif()

target_sources(SimpleNativeAddon PRIVATE
        library.cpp ${CMAKE_JS_SRC}
)

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(SimpleNativeAddon PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(SimpleNativeAddon PRIVATE ${CMAKE_JS_INC})

if (MSVC)
    # Essential library files to link to a node addon
    # You should add this line in every CMake.js based project
    target_link_libraries(SimpleNativeAddon PRIVATE ${CMAKE_JS_LIB})

    # TEST AREA
    target_link_libraries(SimpleNativeAddon PRIVATE odbc32.lib)
    target_link_libraries(SimpleNativeAddon PRIVATE DelayImp.lib)
    # END TEST AREA
endif()

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

add_definitions(-DNAPI_VERSION=3)
