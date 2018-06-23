#! /bin/bash

# ==============================================================================
# -- Set up environment --------------------------------------------------------
# ==============================================================================

source $(dirname "$0")/Environment.sh

# ==============================================================================
# -- Get latest version of uncrustify ------------------------------------------
# ==============================================================================

mkdir -p ${CARLA_BUILD_FOLDER}
pushd ${CARLA_BUILD_FOLDER} >/dev/null

UNCRUSTIFY_BASENAME=uncrustify-0.67

UNCRUSTIFY=${PWD}/${UNCRUSTIFY_BASENAME}-install/bin/uncrustify

if [[ -d "${UNCRUSTIFY_BASENAME}-install" ]] ; then
  log "${UNCRUSTIFY_BASENAME} already installed."
else
  rm -Rf ${UNCRUSTIFY_BASENAME}-source ${UNCRUSTIFY_BASENAME}-build

  log "Retrieving Uncrustify."

  git clone --depth=1 -b uncrustify-0.67 https://github.com/uncrustify/uncrustify.git ${UNCRUSTIFY_BASENAME}-source

  log "Building Uncrustify."

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

command -v ${UNCRUSTIFY} >/dev/null 2>&1 || {
  fatal_error "Failed to install Uncrustify!";
}

popd >/dev/null

# ==============================================================================
# -- Run uncrustify on each source file ----------------------------------------
# ==============================================================================

UNCRUSTIFY_CONFIG=${CARLA_BUILD_TOOLS_FOLDER}/uncrustify.cfg
UNCRUSTIFY_COMMAND="${UNCRUSTIFY} -c ${UNCRUSTIFY_CONFIG} --no-backup --replace"

pushd ${CARLALIB_ROOT_FOLDER} >/dev/null
find source -iregex '.*\.\(h\|cpp\)$' -exec ${UNCRUSTIFY_COMMAND} {} \;
popd >/dev/null

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

log "Success!"
