# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("ffmpegplayer")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../../../../prebuild_libs/${ANDROID_ABI})

aux_source_directory(. ffmpegplayer_source_dir)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(
        avcodec
        SHARED
        IMPORTED
)

#指定库的位置
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec.so
)

add_library(
        avfilter
        SHARED
        IMPORTED
)

#指定库的位置
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter.so
)

add_library(
        avformat
        SHARED
        IMPORTED
)

#指定库的位置
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavformat.so
)

add_library(
        avutil
        SHARED
        IMPORTED
)

#指定库的位置
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil.so
)

add_library(
        swresample
        SHARED
        IMPORTED
)

#指定库的位置
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample.so
)

add_library(
        swscale
        SHARED
        IMPORTED
)

#指定库的位置
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale.so
)


add_library( # Sets the name of the library.
        ffmpegplayer

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${ffmpegplayer_source_dir})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ffmpegplayer
        android
        GLESv2
        EGL
        OpenSLES
        avfilter
        avformat
        avcodec
        swresample
        swscale
        avutil
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})