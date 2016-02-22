#!/bin/bash

mv src/bin "src/$1"
mv src/lib "src/lib$1"
sed -i "N;s/\\(APPNAME=\\).*/\\1$1/" config.mk
