#! /bin/bash

if [ ! -d "${UE4_ROOT}" ]; then
  fatal_error "UE4_ROOT is not defined, or points to a non-existant directory, please set this environment variable."
else
  log "Using Unreal Engine at '$UE4_ROOT'"
fi

source $(dirname "$0")/Environment.sh

pushd "${CARLAUE4_ROOT_FOLDER}" >/dev/null

${UE4_ROOT}/Engine/Binaries/Linux/UE4Editor "${PWD}/CarlaUE4.uproject"

popd >/dev/null
