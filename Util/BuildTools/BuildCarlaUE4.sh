#! /bin/bash

# ==============================================================================
# -- Set up environment --------------------------------------------------------
# ==============================================================================

source $(dirname "$0")/Environment.sh

if [ ! -d "${UE4_ROOT}" ]; then
  fatal_error "UE4_ROOT is not defined, or points to a non-existant directory, please set this environment variable."
else
  log "Using Unreal Engine at '$UE4_ROOT'"
fi

# ==============================================================================
# -- Build CarlaUE4 ------------------------------------------------------------
# ==============================================================================

pushd "${CARLAUE4_ROOT_FOLDER}" >/dev/null

# This command usually fails but normally we can continue anyway.
set +e
log "Generate Unreal project files..."
${UE4_ROOT}/GenerateProjectFiles.sh -project="${PWD}/CarlaUE4.uproject" -game -engine -makefiles
set -e

log "Build CarlaUE4 project..."
make CarlaUE4Editor

popd >/dev/null

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

log "Success!"
