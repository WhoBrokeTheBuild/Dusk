#!/bin/sh

DIR=${1:-$HOME/cmake}
VERSION=3.18.4

mkdir -p $DIR

n=0
until [ "$n" -ge 10 ]; do
    wget -O $DIR/install.sh https://github.com/Kitware/CMake/releases/download/v$VERSION/cmake-$VERSION-Linux-x86_64.sh && break
    n=$((n+1))
    sleep 10
done

bash $DIR/install.sh --prefix=$DIR --skip-license --exclude-subdir
export PATH="$DIR/bin:$PATH"
