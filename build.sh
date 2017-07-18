echo "/-----------build project-------------/"
scons target=arm-android-linux build_config=Release

echo "/------------push bin to device-------/"
adb push Targets/arm-android-linux/Release/MicroMediaController system/bin
