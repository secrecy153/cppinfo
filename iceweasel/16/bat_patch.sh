#!/bin/bash
FF_SRC_ROOT=d:/mozilla-release
FF_PATCH=d:/works/iceweasel_v16
7z.exe x $FF_PATCH/betterpgo.7z  -o$FF_SRC_ROOT/build/pgo -aoy
7z.exe x $FF_PATCH/official.7z  -o$FF_SRC_ROOT/browser/branding -aoy
cd $FF_SRC_ROOT
for MY_PATCH in $( ls $FF_PATCH/tete009/*.patch ); do
	patch -p1 -u < $MY_PATCH
done
for MY_PATCH in $( ls $FF_PATCH/debian/*.patch ); do
	patch -p1 -u < $MY_PATCH
done
rm -f $FF_SRC_ROOT/media/libjpeg/config.h
for MY_PATCH in $( ls $FF_PATCH/up/*.patch ); do
	patch -p1 -u < $MY_PATCH
done