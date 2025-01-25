---
This is supposed to be a minimalistic first release of the code powering my personal website. Feel free to contact me about any questions you may have.
   
```
   - cd $CI_PROJECT_DIR/public_api_website
    - mkdir requirements
    - cd requirements
    - mkdir drogontrantor
    - cd drogontrantor
    - jf rt dl "drogon/linux/drogon/(*)" "{1}"
    - cd ..
    - mkdir other
    - cd other
    - jf rt dl "drogon/linux/dependencies/(*)" "{1}"
    - jf rt dl "follyCICD/linux/libsodium/(*)" "libsodium/{1}"
    - jf rt dl "re2_abseil/linux/(*)" "re2_abseil/{1}"
    - jf rt dl "follyCICD/linux/folly/(*)" "folly/{1}"
    - jf rt dl "follyCICD/linux/boost/lib/(*)" "follyboost/lib/{1}"
    - jf rt dl "follyCICD/linux/boost/include/boost-1_87/(*)" "follyboost/include/{1}"
    - jf rt dl "follyCICD/linux/glog/(*)" "follyglog/{1}"
    - jf rt dl "follyCICD/linux/gflags/(*)" "follygflags/{1}"
    - jf rt dl "follyCICD/linux/libdoubleconv/(*)" "doubleconv/{1}"
    - jf rt dl "follyCICD/linux/libevent/(*)" "libevent/{1}"
    - jf rt dl "follyCICD/linux/liburing/(*)" "liburing/{1}"
    - cd $CI_PROJECT_DIR/public_api_website
    - mkdir build
    - cd build
    - cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++  -DCMAKE_CXX_STANDARD=20  -DCMAKE_CXX_FLAGS_RELEASE="${CMAKE_CXX_FLAGS_RELEASE} -stdlib=libc++ -mavx512f -march=znver4 -O2 -flto -fuse-ld=lld" -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -flto" -DCMAKE_PREFIX_PATH="$CI_PROJECT_DIR/public_api_website/requirements/other/follyglog;$CI_PROJECT_DIR/public_api_website/requirements/other/follygflags;/usr/local/lib/x86_64-unknown-linux-gnu" ..
    - ninja
    - ninja install    

```
