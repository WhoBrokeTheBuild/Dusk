#!/bin/sh

DIR=${1:-$HOME/vulkan}
VERSION=1.2.154.0

mkdir -p $DIR

n=0
until [ "$n" -ge 10 ]; do
    wget -O $DIR/vulkan-sdk.tar.gz https://sdk.lunarg.com/sdk/download/$VERSION/linux/vulkansdk-linux-x86_64-$VERSION.tar.gz && break
    n=$((n+1))
    sleep 10
done

tar -C $DIR -xzf $DIR/vulkan-sdk.tar.gz
export VULKAN_SDK="$DIR/$VERSION/x86_64"
