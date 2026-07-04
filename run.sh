#!/usr/bin/env sh
set -eu
# Always rebuild locally so no precompiled Linux/macOS binary is reused.
make clean
make validate
