#! /bin/bash

source $(dirname "$0")/Environment.sh

export CC=clang-5.0
export CXX=clang++-5.0

pushd "${CARLA_PYTHONAPI_ROOT_FOLDER}" >/dev/null

rm -Rf build dist carla.egg-info source/carla.egg-info
/usr/bin/env python2 setup.py bdist_egg
/usr/bin/env python3 setup.py bdist_egg

popd >/dev/null

log "Success!"
