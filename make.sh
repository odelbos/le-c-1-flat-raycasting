#!/bin/sh

MAC_OSX=false
[ `uname` = "Darwin"  ] && MAC_OSX=true

CC=gcc
CFLAGS="-Wall -Wextra"

RAYLIB_FLAGS="-I/opt/raylib-5.5/include"
RAYLIB_LIBS="-L/opt/raylib-5.5/lib -lraylib"

if $MAC_OSX; then
  RAYLIB_FLAGS=`pkg-config --cflags raylib`
  RAYLIB_LIBS=`pkg-config --libs raylib`
fi

echo "RAYLIB flags: $RAYLIB_FLAGS"
echo "RAYLIB libs: $RAYLIB_LIBS"

echo "Compiling ..."

set -xe

$CC $CFLAGS $RAYLIB_FLAGS \
  -o raycast main.c \
  $RAYLIB_LIBS \
  -lm -lpthread
