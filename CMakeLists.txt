cmake_minimum_required(VERSION 3.20)

Include(ExternalProject)
ExternalProject_Add(
    rapidjson
    PREFIX "rapidjson"
    GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
    GIT_TAG 6089180ecb704cb2b136777798fa1be303618975
    CMAKE_ARGS
        -DRAPIDJSON_BUILD_TESTS=OFF
        -DRAPIDJSON_BUILD_DOC=OFF
        -DRAPIDJSON_BUILD_EXAMPLES=OFF
	CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)
ExternalProject_Get_Property(rapidjson source_dir)
include_directories(${source_dir}/include)

ExternalProject_Add(
    curl
    PREFIX "curl"
    GIT_REPOSITORY "https://github.com/curl/curl.git"
    GIT_TAG 7161cb17c01dcff1dc5bf89a18437d9d729f1ecd
	CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)
ExternalProject_Get_Property(curl source_dir)
find_package(CURL REQUIRED)
link_directories(${CURL_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 23)

project(wda VERSION 1.0 LANGUAGES CXX)
add_executable(wda src/main.cpp)

include(CTest)
add_executable(wda-test test/main.cpp)
add_test(NAME wda-test COMMAND wda-test)
enable_testing()

add_dependencies(wda rapidjson)
target_link_libraries(wda CURL::libcurl)
target_link_libraries(wda-test CURL::libcurl)
