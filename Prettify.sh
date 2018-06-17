#! /bin/bash

set -e

# ==============================================================================
# -- Set up environment --------------------------------------------------------
# ==============================================================================

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

function log {
  echo "`basename "$0"`: $1"
}

# ==============================================================================
# -- Get latest version of uncrustify ------------------------------------------
# ==============================================================================

mkdir -p build
pushd build >/dev/null

UNCRUSTIFY_BASENAME=uncrustify-0.67

UNCRUSTIFY=${PWD}/${UNCRUSTIFY_BASENAME}-install/bin/uncrustify

if [[ -d "${UNCRUSTIFY_BASENAME}-install" ]] ; then
  log "${UNCRUSTIFY_BASENAME} already installed."
else
  rm -Rf ${UNCRUSTIFY_BASENAME}-source ${UNCRUSTIFY_BASENAME}-build

  log "Retrieving Uncrustify..."

  git clone --depth=1 -b uncrustify-0.67 https://github.com/uncrustify/uncrustify.git ${UNCRUSTIFY_BASENAME}-source

  log "Building Uncrustify..."

  mkdir -p ${UNCRUSTIFY_BASENAME}-build

  pushd ${UNCRUSTIFY_BASENAME}-build >/dev/null

  cmake -G "Ninja" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="../${UNCRUSTIFY_BASENAME}-install" \
      ../${UNCRUSTIFY_BASENAME}-source

  ninja

  ninja install

  popd >/dev/null

  rm -Rf ${UNCRUSTIFY_BASENAME}-source ${UNCRUSTIFY_BASENAME}-build

fi

log "Using `${UNCRUSTIFY} --version`"

popd >/dev/null

# ==============================================================================
# -- Run uncrustify on each source file ----------------------------------------
# ==============================================================================

UNCRUSTIFY_COMMAND="${UNCRUSTIFY} -c uncrustify.cfg --no-backup --replace"

find source -iregex '.*\.\(h\|cpp\)$' -exec ${UNCRUSTIFY_COMMAND} {} \;

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

popd >/dev/null

set +x
log "Success!"
