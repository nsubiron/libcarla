#! /bin/bash

source $(dirname "$0")/Environment.sh

# ==============================================================================
# -- Parse arguments -----------------------------------------------------------
# ==============================================================================

DOC_STRING="Run unit tests."

USAGE_STRING="Usage: $0 [-h|--help] [--all] [--carlalib-release] [--carlalib-debug] [--python-api-2] [--python-api-3]"

CARLALIB_RELEASE=false
CARLALIB_DEBUG=false
PYTHON_API_2=false
PYTHON_API_3=false

OPTS=`getopt -o h --long help,all,carlalib-release,carlalib-debug,python-api-2,python-api-3 -n 'parse-options' -- "$@"`

if [ $? != 0 ] ; then echo "$USAGE_STRING" ; exit 2 ; fi

eval set -- "$OPTS"

while true; do
  case "$1" in
    --all )
      CARLALIB_RELEASE=true;
      CARLALIB_DEBUG=true;
      PYTHON_API_2=true;
      PYTHON_API_3=true;
      shift ;;
    --carlalib-release )
      CARLALIB_RELEASE=true;
      shift ;;
    --carlalib-debug )
      CARLALIB_DEBUG=true;
      shift ;;
    --python-api-2 )
      PYTHON_API_2=true;
      shift ;;
    --python-api-3 )
      PYTHON_API_3=true;
      shift ;;
    -h | --help )
      echo "$DOC_STRING"
      echo "$USAGE_STRING"
      exit 1
      ;;
    * )
      break ;;
  esac
done

if ! { ${CARLALIB_RELEASE} || ${CARLALIB_DEBUG} || ${PYTHON_API_2} || ${PYTHON_API_3}; }; then
  fatal_error "Nothing selected to be done."
fi

# ==============================================================================
# -- Run CarlaLib tests --------------------------------------------------------
# ==============================================================================

if ${CARLALIB_DEBUG} ; then

  log "Running CarlaLib unit tests debug."

  LD_LIBRARY_PATH=${CARLALIB_INSTALL_SERVER_FOLDER}/lib ${CARLALIB_INSTALL_SERVER_FOLDER}/test/carlalib_test_debug

fi

if ${CARLALIB_RELEASE} ; then

  log "Running CarlaLib unit tests release."

  LD_LIBRARY_PATH=${CARLALIB_INSTALL_SERVER_FOLDER}/lib ${CARLALIB_INSTALL_SERVER_FOLDER}/test/carlalib_test_release

fi

# ==============================================================================
# -- Run Python API tests ------------------------------------------------------
# ==============================================================================

pushd "${CARLA_PYTHONAPI_ROOT_FOLDER}/test" >/dev/null

if ${PYTHON_API_2} ; then

  log "Running Python API for Python 2 unit tests."

  /usr/bin/env python2 -m nose2

fi

if ${PYTHON_API_3} ; then

  log "Running Python API for Python 3 unit tests."

  /usr/bin/env python3 -m nose2

fi

popd >/dev/null

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

log "Success!"
