echo "/----------build jni libs-------------------/"
ndk-build NDK_DEBUG=0

echo "/------------renew libs---------------------/"
rm -rf /work/github/laboratory/Collections_of_wifidisplay/PlatinumDLNA/platinumcore/libs/*
cp -rf ../libs/* /work/github/laboratory/Collections_of_wifidisplay/PlatinumDLNA/platinumcore/libs/

echo "/------------check libs---------------------/"
tree -D /work/github/laboratory/Collections_of_wifidisplay/PlatinumDLNA/platinumcore/libs/
