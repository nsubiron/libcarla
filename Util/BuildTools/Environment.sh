#! /bin/bash

# Sets the environment for other shell scripts.

set -e

CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )"
source $(dirname "$0")/Vars.mk
unset CURDIR

function log {
  echo "`basename "$0"`: $1"
}

function fatal_error {
  echo -e >&2 "\033[0;31m`basename "$0"`: ERROR: $1\033[0m"
  exit 2
}
