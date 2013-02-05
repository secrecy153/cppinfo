#!/bin/bash
FF_SRC_ROOT=d:/works/ff-release
FF_PATCH=d:/works/18
#find $FF_PATCH/tete009 -name "*.patch" | xargs dos2unix
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