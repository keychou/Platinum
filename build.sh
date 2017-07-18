echo "## STEP 1-----------build project-------------"
scons target=arm-android-linux build_config=Release
echo ""
echo ""
echo ""

echo "## STEP 2------------push bin to device-------"
adb push Targets/arm-android-linux/Release/MicroMediaController system/bin
echo ""
echo ""
echo ""


echo "## STEP 3---------cd to Source/Platform/Android/module/platinum/jni/-----"
cd Source/Platform/Android/module/platinum/jni/
echo ""
echo ""
echo ""

echo "## STEP 4-----------excute make.sh------------"
./make.sh
echo ""
echo ""
echo ""

cd -

