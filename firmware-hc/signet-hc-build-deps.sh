#!/bin/bash

#
# Make common ct-ng if needed
#
if [ ! -e ../crosstool-ng/ct-ng ]; then
    pushd ../crosstool-ng &&
    ./bootstrap &&
    ./configure --enable-local &&
    make || exit -1
    popd
fi

#
#Build toolchain in "toolchain/" folder
#
pushd ct-ng
../../crosstool-ng/ct-ng build || exit -1
popd

#
#Environment
#
FW_SOURCE_ROOT=$PWD
TARGET_TUPLE=arm-none-eabi
TOOLCHAIN=$FW_SOURCE_ROOT/toolchain/$TARGET_TUPLE
SYSROOT=$TOOLCHAIN/$TARGET_TUPLE
CFLAGS="-nostartfiles -ffunction-sections -fdata-sections -O2 -Os -I$SYSROOT/include -L$SYSROOT/lib"
export PATH=$TOOLCHAIN/bin:$PATH

#
# Make sysroot folders writable so we can install mini-gmp and nettle
#
chmod u+w $SYSROOT/include $SYSROOT/lib

#
# Install mini-gmp
#
pushd external/mini-gmp/mini-gmp
$TARGET_TUPLE-gcc $CFLAGS -c mini-gmp.c || exit -1
rm libmini-gmp.a
$TARGET_TUPLE-ar -r libmini-gmp.a mini-gmp.o
cp libmini-gmp.a $SYSROOT/lib
cp mini-gmp.h $SYSROOT/include
popd

#
# Install nettle
#
mkdir -p external/nettle/build-hc
pushd external/nettle
autoconf &&
autoheader &&
pushd build-hc &&
../configure \
--disable-assembler \
--disable-documentation \
--disable-shared \
--disable-pic \
--enable-mini-gmp \
--enable-public-key \
--host=arm-none-eabi \
--prefix=$SYSROOT &&
make &&
make install &&
echo "Toolchain and dependecies are ready!"
popd
popd
