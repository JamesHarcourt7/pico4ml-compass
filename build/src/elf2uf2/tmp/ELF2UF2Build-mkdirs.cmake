# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/VSRAM/pico/pico-sdk/tools/elf2uf2"
  "C:/VSRAM/pico/projects/compass/build/elf2uf2"
  "C:/VSRAM/pico/projects/compass/build/src/elf2uf2"
  "C:/VSRAM/pico/projects/compass/build/src/elf2uf2/tmp"
  "C:/VSRAM/pico/projects/compass/build/src/elf2uf2/src/ELF2UF2Build-stamp"
  "C:/VSRAM/pico/projects/compass/build/src/elf2uf2/src"
  "C:/VSRAM/pico/projects/compass/build/src/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/VSRAM/pico/projects/compass/build/src/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
