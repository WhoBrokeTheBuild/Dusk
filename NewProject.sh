#!/bin/bash

BASE_DIR="$(dirname $(readlink -f $0))"

if [ ! -f "$HOME/.duskconfig" ]; then
    echo $BASE_DIR > "$HOME/.duskconfig"
fi

cp -r $BASE_DIR/template/* ./
