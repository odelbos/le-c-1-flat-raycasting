#!/bin/sh


# On Mac OSX:
# brew install raylib glfw

CC=gcc
CFLAGS="-Wall -Wextra"

RAYLIB_FLAGS=`pkg-config --cflags raylib`
RAYLIB_LIBS=`pkg-config --libs raylib`

GLFW_FLAGS=`pkg-config --cflags glfw3`
GLFW_LIBS=`pkg-config --libs glfw3`

echo "RAYLIB flags: $RAYLIB_FLAGS"
echo "RAYLIB libs: $RAYLIB_LIBS"

echo "GLFW flags: $GLFW_FLAGS"
echo "GLFW libs: $GLFW_LIBS"

echo "Compiling ..."

set -xe

$CC $CFLAGS $RAYLIB_FLAGS \
  -I/opt/homebrew/Cellar/glfw/3.4/libs \
  -o raycast main.c $RAYLIB_LIBS \
  -I/opt/homebrew/Cellar/glfw/3.4/include \
  -lm -ldl -lpthread
