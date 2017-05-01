#!/bin/sh
#
# This script is used in .travis.yml to install the dependencies before building.
# Notice that WX_PACKAGE is supposed to be defined before running it.

set -e

case "$TRAVIS_OS_NAME" in
    linux)
        echo -n 'Architecture: '
        uname -m
        sudo apt-get -qq update
        case "$HOST" in
            i686-w64-mingw32)
                # Set up the repository containing wxMSW packages.
                curl http://apt.tt-solutions.com/tt-apt.gpg.key | sudo apt-key add -
                echo 'deb [arch=amd64] http://apt.tt-solutions.com/ trusty main' | sudo tee -a /etc/apt/sources.list.d/tt-solutions.list
                sudo apt-get update -qq
                # And also install the compiler we're going to use.
                sudo apt-get install --no-install-recommends g++-mingw-w64-i686
                ;;
        esac

        sudo apt-get install -y $WX_PACKAGE
    ;;

    osx)
        brew update
        brew install wxmac
    ;;

    *)
        echo "Add commands to install wxWidgets on this platform!"
        exit 1
    ;;
esac
