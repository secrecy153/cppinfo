#!/bin/bash
FF_SRC_ROOT=d:/works/ff-esr
FF_PATCH=d:/works/v17/patch
for MY_PATCH in $( ls $FF_PATCH/tete009/*.patch ); do
	patch -p1 -u < $MY_PATCH
done
for MY_PATCH in $( ls $FF_PATCH/up/*.patch ); do
        patch -p1 -u < $MY_PATCH
done
patch -p1 -u < $FF_PATCH/fpfast.patch
patch -p1 -u < $FF_PATCH/sqlite3_2.patch
7z.exe x $FF_PATCH/betterpgo.7z  -o$FF_SRC_ROOT/build/pgo -aoy
7z.exe x $FF_PATCH/official.7z  -o$FF_SRC_ROOT/browser/branding -aoy