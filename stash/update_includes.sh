#! /bin/bash --debug

# YEJ 2011/10/10
# This script is used to create a local folder
# with links to the needed shared lib includes folders.
# (ex. standard lib, etc.)
#
# It's useful for giving them to eclipse so
# the code analysis will work,
# while not giving the ful /usr/include folder
# which contains a lot of unneeded files.
#
# We use a script to be able to easily replicate this
# on the 3 infinity dev machines.


# the dir where we will put the include files
#LOCAL_INCLUDE_DIR='/srv/dev/wt/includes_wt'
LOCAL_INCLUDE_DIR='/Home/ut1kab/xp/utcov/includes'
#GCC_VERSION=4.4
GCC_VERSION=4.1.2
#LINUX_VERSION=x86_64-linux-gnu
LINUX_VERSION=x86_64-redhat-linux


echo "" # for display clarity

echo '--- Project includes rebuild... ---'


# does the target folder exist ?
if [ ! -d $LOCAL_INCLUDE_DIR ] ; then
   mkdir $LOCAL_INCLUDE_DIR
fi

# now, just in case, remove everything in the target folder
rm -rf $LOCAL_INCLUDE_DIR/*

# write a small readme file with directions.
fullname="`pwd`/`basename $0`"
echo "" >  $INF_INCLUDE_DIR/ReadMe.txt
echo "This folder contains symbolic links to all third party includes"                   >> $LOCAL_INCLUDE_DIR/ReadMe.txt
echo "needed by Infinity. This is for the eclipse indexer to work properly."             >> $LOCAL_INCLUDE_DIR/ReadMe.txt
echo "This directory's content was automatically created with this script : $fullname."  >>  $LOCAL_INCLUDE_DIR/ReadMe.txt
echo "Do NOT edit this directory manually."                                              >> $LOCAL_INCLUDE_DIR/ReadMe.txt



############
# C standard library
# those includes are mixed with a lot of things in /usr/include
# make it clean by welecting only needed files
CSDTLIB_INCLUDE_DIR=$LOCAL_INCLUDE_DIR/cstdlib
if [ ! -d $CSDTLIB_INCLUDE_DIR ] ; then
   mkdir $CSDTLIB_INCLUDE_DIR
fi
# now the files, list taken from wikipedia
ln -s /usr/include/assert.h                                     $CSDTLIB_INCLUDE_DIR/assert.h
ln -s /usr/include/complex.h                                    $CSDTLIB_INCLUDE_DIR/complex.h
ln -s /usr/include/ctype.h                                      $CSDTLIB_INCLUDE_DIR/ctype.h
ln -s /usr/include/errno.h                                      $CSDTLIB_INCLUDE_DIR/errno.h
ln -s /usr/include/fenv.h                                       $CSDTLIB_INCLUDE_DIR/fenv.h
ln -s /usr/lib/gcc/$LINUX_VERSION/$GCC_VERSION/include/float.h    $CSDTLIB_INCLUDE_DIR/float.h
ln -s /usr/include/inttypes.h                                   $CSDTLIB_INCLUDE_DIR/inttypes.h
ln -s /usr/lib/gcc/$LINUX_VERSION/$GCC_VERSION/include/iso646.h   $CSDTLIB_INCLUDE_DIR/iso646.h
ln -s /usr/lib/gcc/$LINUX_VERSION/$GCC_VERSION/include/limits.h   $CSDTLIB_INCLUDE_DIR/limits.h
ln -s /usr/include/locale.h                                     $CSDTLIB_INCLUDE_DIR/locale.h
ln -s /usr/include/math.h                                       $CSDTLIB_INCLUDE_DIR/math.h
ln -s /usr/include/setjmp.h                                     $CSDTLIB_INCLUDE_DIR/setjmp.h
ln -s /usr/include/signal.h                                     $CSDTLIB_INCLUDE_DIR/signal.h
ln -s /usr/lib/gcc/$LINUX_VERSION/$GCC_VERSION/include/stdarg.h   $CSDTLIB_INCLUDE_DIR/stdarg.h
ln -s /usr/lib/gcc/$LINUX_VERSION/$GCC_VERSION/include/stdbool.h  $CSDTLIB_INCLUDE_DIR/stdbool.h
ln -s /usr/include/stdint.h                                     $CSDTLIB_INCLUDE_DIR/stdint.h
ln -s /usr/lib/gcc/$LINUX_VERSION/$GCC_VERSION/include/stddef.h   $CSDTLIB_INCLUDE_DIR/stddef.h
ln -s /usr/include/stdio.h                                      $CSDTLIB_INCLUDE_DIR/stdio.h
ln -s /usr/include/stdlib.h                                     $CSDTLIB_INCLUDE_DIR/stdlib.h
ln -s /usr/include/string.h                                     $CSDTLIB_INCLUDE_DIR/string.h
ln -s /usr/include/tgmath.h                                     $CSDTLIB_INCLUDE_DIR/tgmath.h
ln -s /usr/include/time.h                                       $CSDTLIB_INCLUDE_DIR/time.h
ln -s /usr/include/wchar.h                                      $CSDTLIB_INCLUDE_DIR/wchar.h
ln -s /usr/include/wctype.h                                     $CSDTLIB_INCLUDE_DIR/wctype.h

# other files not listed but apparently needed 
ln -s /usr/include/features.h                                   $CSDTLIB_INCLUDE_DIR/features.h
ln -s /usr/include/memory.h                                     $CSDTLIB_INCLUDE_DIR/memory.h

# it seems that cstdlib also needs a "bits" folder
BITS_INCLUDE_DIR=$CSDTLIB_INCLUDE_DIR/bits
ln -s /usr/include/bits   $BITS_INCLUDE_DIR


############
# C++ Language library (including C stdlib wrap, STL, etc.)
CPPLIB_INCLUDE_DIR=$LOCAL_INCLUDE_DIR/c++lib
if [ ! -d $CPPLIB_INCLUDE_DIR ] ; then
   mkdir $CPPLIB_INCLUDE_DIR
fi
ln -s /usr/include/c++/$GCC_VERSION   $CPPLIB_INCLUDE_DIR


############
# boost
#ln -s /srv/dev/tools/gcc44/boost/include/boost   $LOCAL_INCLUDE_DIR/boost
ln -s /Soft/infinityFO/boost/include/boost   $LOCAL_INCLUDE_DIR/boost


############
# Wt
#ln -s /usr/include/Wt   $LOCAL_INCLUDE_DIR/Wt
ln -s /srv/dev/tools/gcc44/wt/include/Wt   $LOCAL_INCLUDE_DIR/Wt

############
# UnitTest++
#ln -s /srv/dev/tools/build44/UnitTest++/src/   $LOCAL_INCLUDE_DIR/UnitTest++
ln -s /Home/ut1kab/xp/UnitTest++-32/src/   $LOCAL_INCLUDE_DIR/UnitTest++


echo '--- includes rebuild done. ---'

echo "" # for display clarity

exit 0
