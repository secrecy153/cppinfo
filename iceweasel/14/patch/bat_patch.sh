#!/bin/bash
for MY_PATCH in $( ls patch_ex/*.patch ); do
	patch -p1 -u < $MY_PATCH
done