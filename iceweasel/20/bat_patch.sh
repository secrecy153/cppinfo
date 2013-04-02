#!/bin/bash
FF_SRC_ROOT=F:/works/ff-release
FF_PATCH=F:/works/v20
for MY_PATCH in $( ls $FF_PATCH/tete009/*.patch ); do
	patch -p1 -u < $MY_PATCH
done
for MY_PATCH in $( ls $FF_PATCH/up/*.patch ); do
        patch -p1 -u < $MY_PATCH
done
echo enter fix directory.
for MY_PATCH in $( ls $FF_PATCH/fix/*.patch ); do
        patch -p1 -u < $MY_PATCH
done
7z.exe x $FF_PATCH/betterpgo.7z  -o$FF_SRC_ROOT/build/pgo -aoy
7z.exe x $FF_PATCH/official.7z  -o$FF_SRC_ROOT/browser/branding -aoy