#!/usr/bin/env bash

# First, create a new project using :Pioinit
# Next, delete the old 'compile_commands.json' file.
# Then, copy this script into the project root, mark as executable, and run!

echo 'CompileFlags:                    
  Add:
    [
      # -mlong-calls,
      -DSSIZE_MAX,
      -DLWIP_NO_UNISTD_H=1,
      -Dssize_t=long,
      -D_SSIZE_T_DECLARED,
    ]
  Remove:
    [
      # -fno-shrink-wrap,           #uncomment if using ESP-IDF
      -fno-tree-switch-conversion,
      -mtext-section-literals,
      -mlongcalls,
      -fstrict-volatile-bitfields,
      -free,
      -fipa-pta,
    ]' > .clangd

echo "Checks: '-*, -misc-definitions-in-headers' " > .clangd-tidy

echo 'import os
Import("env")

# include toolchain paths
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=True)

# override compilation DB path
env.Replace(COMPILATIONDB_PATH="compile_commands.json")' > gen_compile_commands.py

echo 'extra_scripts = pre:gen_compile_commands.py' >> platformio.ini

pio run -t compiledb

echo '.cache
compile_commands.json
include/credentials.h' > .gitignore

# echo "monitor_speed = 115200" > platformio.ini    #uncomment if using ESP-IDF

exit 0
