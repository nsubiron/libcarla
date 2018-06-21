from setuptools import setup, Extension

import os
import platform
import sys


def get_libcarla_extension():
    libraries = ['carla_client', 'rpc']

    if os.name == "posix":
        if platform.dist()[0] == "Ubuntu":
            libraries += ["boost_python-py%d%d" % (sys.version_info.major,
                                                   sys.version_info.minor)]
        else:
            libraries += ["boost_python"]
    else:
        raise NotImplementedError

    return Extension(
        'carla.libcarla',
        sources=['source/libcarla/Client.cpp'],
        include_dirs=[
            '/usr/local/include',
            'dependencies/include'],
        library_dirs=[
            '/usr/local/lib/boost',
            'dependencies/lib'],
        runtime_library_dirs=['/usr/local/lib/boost'],
        libraries=libraries,
        extra_compile_args=['-fPIC', '-std=c++17'],
        language='c++17')


setup(
    name='carla',
    version='0.9.0',
    package_dir={'': 'source'},
    packages=['carla'],
    ext_modules=[get_libcarla_extension()],
    license='MIT License',
    description='Python API for communicating with the CARLA server.',
    url='https://github.com/carla-simulator/carla',
    author='The CARLA team',
    author_email='carla.simulator@gmail.com',
    include_package_data=True)
