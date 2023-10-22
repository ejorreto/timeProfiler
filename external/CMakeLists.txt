add_library(Unity_CMock STATIC Unity/src/unity.c CMock/src/cmock.c)

target_include_directories(Unity_CMock PUBLIC Unity/src CMock/src)