cmake_minimum_required(VERSION 3.10)
project(LastFMApi)

find_package(glog CONFIG REQUIRED)


add_executable(LastFMApi
    ${CMAKE_SOURCE_DIR}/src/Drogon.cpp
	${CMAKE_SOURCE_DIR}/src/LastFMApi.cpp
	${CMAKE_SOURCE_DIR}/src/LastFMSocket.cpp
	${CMAKE_SOURCE_DIR}/src/MatrixPresenceReceiver.cpp
	${CMAKE_SOURCE_DIR}/src/MatrixSocket.cpp
	${CMAKE_SOURCE_DIR}/src/SecretVariablesInit.cpp
	${CMAKE_SOURCE_DIR}/src/ScrobbleUpdate.cpp
	

)


target_include_directories(LastFMApi PRIVATE
    ${CMAKE_SOURCE_DIR}/requirements/drogontrantor/include
	${CMAKE_SOURCE_DIR}/requirements/other/jsoncpp/include
	${CMAKE_SOURCE_DIR}/requirements/other/c-ares/include
	${CMAKE_SOURCE_DIR}/requirements/other/hiredis/include
	${CMAKE_SOURCE_DIR}/requirements/other/openssl/include
	${CMAKE_SOURCE_DIR}/requirements/other/postgres/include
	${CMAKE_SOURCE_DIR}/requirements/other/zlib/include
	${CMAKE_SOURCE_DIR}/requirements/other/zstd/include
	${CMAKE_SOURCE_DIR}/requirements/other/mimalloc/include
	${CMAKE_SOURCE_DIR}/requirements/other/jwt-cpp/include
	${CMAKE_SOURCE_DIR}/requirements/other/libsodium/include
	${CMAKE_SOURCE_DIR}/requirements/other/re2_abseil/include
	${CMAKE_SOURCE_DIR}/requirements/other/folly/include
	${CMAKE_SOURCE_DIR}/requirements/other/follyboost/include
	${CMAKE_SOURCE_DIR}/requirements/other/doubleconv/include
	${CMAKE_SOURCE_DIR}/requirements/other/libevent/include
	${CMAKE_SOURCE_DIR}/requirements/other/fmt/include
	${CMAKE_SOURCE_DIR}/requirements/other/liburing/include
	
	
	
	

    
)

find_library(LIBUUID uuid REQUIRED)

file (GLOB ABSLLIBS "${CMAKE_SOURCE_DIR}/requirements/other/re2_abseil/lib/*.a")


target_link_libraries(LastFMApi
    "${CMAKE_SOURCE_DIR}/requirements/drogontrantor/lib/libdrogon.a"
	"${CMAKE_SOURCE_DIR}/requirements/drogontrantor/lib/libtrantor.a"
    "${CMAKE_SOURCE_DIR}/requirements/other/openssl/lib64/libssl.a"
    "${CMAKE_SOURCE_DIR}/requirements/other/openssl/lib64/libcrypto.a"
    "${CMAKE_SOURCE_DIR}/requirements/other/jsoncpp/lib/libjsoncpp.a"
    "${CMAKE_SOURCE_DIR}/requirements/other/postgres/lib/libpq.so.5"
    "${CMAKE_SOURCE_DIR}/requirements/other/c-ares/lib/libcares.a"
    "${CMAKE_SOURCE_DIR}/requirements/other/hiredis/lib/libhiredis.a"
    "${CMAKE_SOURCE_DIR}/requirements/other/zlib/lib/libz.a"
	"${CMAKE_SOURCE_DIR}/requirements/other/zstd/lib/libzstd.a"
	"${CMAKE_SOURCE_DIR}/requirements/other/mimalloc/lib/libmimalloc.so.2"
	"${CMAKE_SOURCE_DIR}/requirements/other/libsodium/lib/libsodium.a"
	"${CMAKE_SOURCE_DIR}/requirements/other/re2_abseil/lib/libre2.a"
	"${CMAKE_SOURCE_DIR}/requirements/other/fmt/lib/libfmt.a"
	${ABSLLIBS}
	"${CMAKE_SOURCE_DIR}/requirements/other/folly/lib/libfolly.a"
	"${CMAKE_SOURCE_DIR}/requirements/other/doubleconv/lib/libdouble-conversion.a"
	glog::glog
	${LIBUUID}
    "${CMAKE_SOURCE_DIR}/requirements/other/libevent/lib/libevent-2.1.so.7"
	"${CMAKE_SOURCE_DIR}/requirements/other/libevent/lib/libevent_core-2.1.so.7"
	"${CMAKE_SOURCE_DIR}/requirements/other/libevent/lib/libevent_extra-2.1.so.7"
	"${CMAKE_SOURCE_DIR}/requirements/other/libevent/lib/libevent_pthreads-2.1.so.7"
	"${CMAKE_SOURCE_DIR}/requirements/other/liburing/lib/liburing.so.2"

	
    
	
    
)



set_target_properties(LastFMApi PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build/installationdir")


install(TARGETS LastFMApi DESTINATION release)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
