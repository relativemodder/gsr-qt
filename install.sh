#!/bin/sh

cp ./build/bin/gsr-qt-daemon-service /usr/bin/
cp ./build/overlay/gsr-qt-overlay /usr/bin/
cp ./build/notification/gsr-qt-notification /usr/bin/

mkdir -p /usr/share/gsr-qt
cp ./daemon/kwin/gsrkwinhelper.js /usr/share/gsr-qt/