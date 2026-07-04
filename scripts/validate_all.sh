#!/usr/bin/env bash
set -euo pipefail

make clean test

echo "OK: full validation completed."
