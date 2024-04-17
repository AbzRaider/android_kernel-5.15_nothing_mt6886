#!/bin/bash
#

export KBUILD_BUILD_USER="AbzRaider"
export KBUILD_BUILD_HOST=NOVA

SECONDS=0
CLANG_DIR="$PWD/clang"
GKI_DEFCONFIG="gki_defconfig"
DEVICE_FRAGMENT_DEFCONFIG="pacman.config"
ENTRY_LEVEL_DEFCONFIG="entry_level.config"


if [ ! -d "clang" ]; then
	git clone https://github.com/LineageOS/android_prebuilts_clang_kernel_linux-x86_clang-r416183b clang
fi

export PATH="$CLANG_DIR/bin:$PATH"

if [[ $1 = "-d" || $1 = "--defconfig" ]]; then
	KCONFIG_CONFIG=arch/arm64/configs/$DEVICE_DEFCONFIG \
		scripts/kconfig/merge_config.sh -m -r \
  		arch/arm64/configs/$GKI_DEFCONFIG \
  		arch/arm64/configs/$ENTRY_LEVEL_DEFCONFIG \
  		arch/arm64/configs/$DEVICE_FRAGMENT_DEFCONFIG
	exit
fi

if [[ $1 = "-r" || $1 = "--regendefconfig" ]]; then
	make O=out ARCH=arm64 $DEVICE_DEFCONFIG savedefconfig
	cp out/defconfig arch/arm64/configs/$DEVICE_DEFCONFIG
	echo -e "\nSuccessfully regenerated defconfig at $DEVICE_DEFCONFIG"
	exit
fi

if [[ $1 = "-c" || $1 = "--clean" ]]; then
	make ARCH=arm64 mrproper
	rm -rf out
fi

mkdir -p out

if [[ $1 = "-g" || $1 = "--gki" ]]; then
	echo -e "\nUsing $GKI_DEFCONFIG for compilation"
	make O=out ARCH=arm64 $GKI_DEFCONFIG
else
	echo -e "\nUsing MTK Fragment for compilation"
	make O=out ARCH=arm64 $GKI_DEFCONFIG $DEVICE_FRAGMENT_DEFCONFIG $ENTRY_LEVEL_DEFCONFIG
fi

echo -e "\nStarting compilation...\n"
make -j$(nproc --all) O=out ARCH=arm64 CC=clang LLVM_IAS=1 LD=$CLANG_DIR/bin/ld.lld AR=$CLANG_DIR/bin/llvm-ar NM=$CLANG_DIR/bin/llvm-nm OBJCOPY=$CLANG_DIR/bin/llvm-objcopy OBJDUMP=$CLANG_DIR/bin/llvm-objdump STRIP=$CLANG_DIR/bin/llvm-strip

kernel="out/arch/arm64/boot/Image.gz"

if [ -f "$kernel" ]; then
	echo -e "\nCompleted in $((SECONDS / 60)) minute(s) and $((SECONDS % 60)) second(s) !"
	rm -rf AnyKernel
	git clone https://github.com/AbzRaider/AnyKernel3 -b pacman AnyKernel --depth=1
	cp out/arch/arm64/boot/Image.gz AnyKernel
	cd AnyKernel
	zip -r9 TEST-OSS-KERNEL-PACMAN-T.zip *
else
	echo -e "\nCompilation failed!"
	exit 1
fi
