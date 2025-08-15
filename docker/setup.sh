#!/bin/sh
set -eu

echo "[INFO] Installing build dependencies and ARM toolchain..."

apk update
apk add --no-cache \
  bash git cmake ninja build-base \
  gcc-arm-none-eabi \
  g++-arm-none-eabi \
  newlib-arm-none-eabi \
  bash coreutils findutils tar gzip xz \
  curl ca-certificates git openssh-client \
  libstdc++ \
  gdb-multiarch

echo "[INFO] Setup complete."