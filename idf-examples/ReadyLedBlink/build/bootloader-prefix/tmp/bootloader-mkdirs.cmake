# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/uj/esp/esp-idf-v4.4.4/components/bootloader/subproject"
  "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader"
  "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader-prefix"
  "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader-prefix/tmp"
  "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader-prefix/src"
  "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/uj/esp/examples/ReadyLedBlink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
