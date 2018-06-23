#! /bin/bash

# Sets the environment for other shell scripts.

set -e

CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )"
source $(dirname "$0")/Vars.mk
unset CURDIR

function log {
  echo -e "\033[1;35m`basename "$0"`: $1\033[0m"
}

function fatal_error {
  echo -e >&2 "\033[0;31m`basename "$0"`: ERROR: $1\033[0m"
  exit 2
}

function get_carla_version {
  git describe --tags --dirty --always
}

function copy_if_changed {
  mkdir -p $(dirname $2)
  rsync -cI --out-format="%n" $1 $2
}

function move_if_changed {
  copy_if_changed $1 $2
  rm -f $1
}
