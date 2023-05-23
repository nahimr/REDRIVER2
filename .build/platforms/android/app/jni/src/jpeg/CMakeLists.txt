cmake_minimum_required(VERSION 3.6)

project(jpeg C CXX)

set(JPEG_SOURCES
    		${JPEG_SOURCE_DIR}/jaricom.c
    		${JPEG_SOURCE_DIR}/jcapimin.c
    		${JPEG_SOURCE_DIR}/jcapistd.c
    		${JPEG_SOURCE_DIR}/jcarith.c
    		${JPEG_SOURCE_DIR}/jccoefct.c
    		${JPEG_SOURCE_DIR}/jccolor.c
    		${JPEG_SOURCE_DIR}/jcdctmgr.c
    		${JPEG_SOURCE_DIR}/jchuff.c
    		${JPEG_SOURCE_DIR}/jcinit.c
    		${JPEG_SOURCE_DIR}/jcmainct.c
    		${JPEG_SOURCE_DIR}/jcmarker.c
    		${JPEG_SOURCE_DIR}/jcmaster.c
    		${JPEG_SOURCE_DIR}/jcomapi.c
    		${JPEG_SOURCE_DIR}/jcparam.c
    		${JPEG_SOURCE_DIR}/jcprepct.c
    		${JPEG_SOURCE_DIR}/jcsample.c
    		${JPEG_SOURCE_DIR}/jctrans.c
    		${JPEG_SOURCE_DIR}/jdapimin.c
    		${JPEG_SOURCE_DIR}/jdapistd.c
    		${JPEG_SOURCE_DIR}/jdarith.c
    		${JPEG_SOURCE_DIR}/jdatadst.c
    		${JPEG_SOURCE_DIR}/jdatasrc.c
    		${JPEG_SOURCE_DIR}/jdcoefct.c
    		${JPEG_SOURCE_DIR}/jdcolor.c
    		${JPEG_SOURCE_DIR}/jddctmgr.c
    		${JPEG_SOURCE_DIR}/jdhuff.c
    		${JPEG_SOURCE_DIR}/jdinput.c
    		${JPEG_SOURCE_DIR}/jdmainct.c
    		${JPEG_SOURCE_DIR}/jdmarker.c
    		${JPEG_SOURCE_DIR}/jdmaster.c
    		${JPEG_SOURCE_DIR}/jdmerge.c
    		${JPEG_SOURCE_DIR}/jdpostct.c
    		${JPEG_SOURCE_DIR}/jdsample.c
    		${JPEG_SOURCE_DIR}/jdtrans.c
    		${JPEG_SOURCE_DIR}/jerror.c
    		${JPEG_SOURCE_DIR}/jfdctflt.c
    		${JPEG_SOURCE_DIR}/jfdctfst.c
    		${JPEG_SOURCE_DIR}/jfdctint.c
    		${JPEG_SOURCE_DIR}/jidctflt.c
    		${JPEG_SOURCE_DIR}/jidctfst.c
    		${JPEG_SOURCE_DIR}/jidctint.c
    		${JPEG_SOURCE_DIR}/jmemmgr.c
    		${JPEG_SOURCE_DIR}/jmemnobs.c
    		${JPEG_SOURCE_DIR}/jquant1.c
    		${JPEG_SOURCE_DIR}/jquant2.c
    		${JPEG_SOURCE_DIR}/jutils.c

)

add_library(jpeg STATIC
            ${JPEG_SOURCES}
           )