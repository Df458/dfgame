#!/bin/bash

mv src/editor "src/$1-editor"
mv src/libeditor "src/lib$1-editor"
mv src/bin "src/$1"
mv src/lib "src/lib$1"
mv vapi/libgame.vapi "vapi/lib${1}.vapi"
mv vapi/libeditor.vapi "vapi/lib${1}-editor.vapi"
sed -i "N;s/\\(APPNAME=\\).*/\\1$1/" "config.mk"
sed -i "N;s/\\(namespace \\).*/\\1$1 {/" "src/lib${1}.vapi"
sed -i "N;s/\\(namespace \\).*/\\1$1 {/" "src/lib${1}-editor.vapi"
