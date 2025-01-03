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
      -fno-tree-switch-conversion,
      -mtext-section-literals,
      -mlongcalls,
      -fstrict-volatile-bitfields,
      -free,
      -fipa-pta,
      -fno-shrink-wrap,
    ]' > .clangd

echo "Checks: '-*, -misc-definitions-in-headers' " > .clangd-tidy

pio run -t compiledb

exit 0
