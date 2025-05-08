module;

#include <cstddef>
#include <utility>
#include <tuple>
#include <string>
#include <string_view>
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <mutex>
#include <array>
#include <functional>
#include <stdarg.h>

export module Easy.Scripting.JniBindBasic;

export {
#include "jni_bind_release.h"
}

// #define JNI_FALSE 0
constexpr int M_JNI_FALSE = JNI_FALSE;
#undef JNI_FALSE
export constexpr int JNI_FALSE = 0;
// #define JNI_TRUE 1
constexpr int M_JNI_TRUE = JNI_TRUE;
#undef JNI_TRUE
export constexpr int JNI_TRUE = 1;
//
// /*
//  * possible return values for JNI functions.
//  */
//
// #define JNI_OK           0                 /* success */
constexpr int M_JNI_OK = JNI_OK;
#undef JNI_OK
export constexpr int JNI_OK = 0;
// #define JNI_ERR          (-1)              /* unknown error */
constexpr int M_JNI_ERR = JNI_ERR;
#undef JNI_ERR
export constexpr int JNI_ERR = -1;
// #define JNI_EDETACHED    (-2)              /* thread detached from the VM */
constexpr int M_JNI_EDETACHED = JNI_EDETACHED;
#undef JNI_EDETACHED
export constexpr int JNI_EDETACHED = -2;
// #define JNI_EVERSION     (-3)              /* JNI version error */
constexpr int M_JNI_EVERSION = JNI_EVERSION;
#undef JNI_EVERSION
export constexpr int JNI_EVERSION = -3;
// #define JNI_ENOMEM       (-4)              /* not enough memory */
constexpr int M_JNI_ENOMEM = JNI_ENOMEM;
#undef JNI_ENOMEM
export constexpr int JNI_ENOMEM = -4;
// #define JNI_EEXIST       (-5)              /* VM already created */
constexpr int M_JNI_EEXIST = JNI_EEXIST;
#undef JNI_EEXIST
export constexpr int JNI_EEXIST = -5;
// #define JNI_EINVAL       (-6)              /* invalid arguments */
constexpr int M_JNI_EINVAL = JNI_EINVAL;
#undef JNI_EINVAL
export constexpr int JNI_EINVAL = -6;

//
// /*
//  * used in ReleaseScalarArrayElements
//  */
//
// #define JNI_COMMIT 1
constexpr int M_JNI_COMMIT = JNI_COMMIT;
#undef JNI_COMMIT
export constexpr int JNI_COMMIT = 1;
// #define JNI_ABORT 2
constexpr int M_JNI_ABORT = JNI_ABORT;
#undef JNI_ABORT
export constexpr int JNI_ABORT = 2;

// #define JNI_VERSION_1_1 0x00010001
constexpr uint32_t M_JNI_VERSION_1_1 = JNI_VERSION_1_1;
#undef JNI_VERSION_1_1
export constexpr uint32_t JNI_VERSION_1_1 = 0x00010001;
// #define JNI_VERSION_1_2 0x00010002
constexpr uint32_t M_JNI_VERSION_1_2 = JNI_VERSION_1_2;
#undef JNI_VERSION_1_2
export constexpr uint32_t JNI_VERSION_1_2 = 0x00010002;
// #define JNI_VERSION_1_4 0x00010004
constexpr uint32_t M_JNI_VERSION_1_4 = JNI_VERSION_1_4;
#undef JNI_VERSION_1_4
export constexpr uint32_t JNI_VERSION_1_4 = 0x00010004;
// #define JNI_VERSION_1_6 0x00010006
constexpr uint32_t M_JNI_VERSION_1_6 = JNI_VERSION_1_6;
#undef JNI_VERSION_1_6
export constexpr uint32_t JNI_VERSION_1_6 = 0x00010006;
// #define JNI_VERSION_1_8 0x00010008
constexpr uint32_t M_JNI_VERSION_1_8 = JNI_VERSION_1_8;
#undef JNI_VERSION_1_8
export constexpr uint32_t JNI_VERSION_1_8 = 0x00010008;
// #define JNI_VERSION_9   0x00090000
constexpr uint32_t M_JNI_VERSION_9 = JNI_VERSION_9;
#undef JNI_VERSION_9
export constexpr uint32_t JNI_VERSION_9 = 0x00090000;
// #define JNI_VERSION_10  0x000a0000
constexpr uint32_t M_JNI_VERSION_10 = JNI_VERSION_10;
#undef JNI_VERSION_10
export constexpr uint32_t JNI_VERSION_10 = 0x000a0000;
// #define JNI_VERSION_19  0x00130000
constexpr uint32_t M_JNI_VERSION_19 = JNI_VERSION_19;
#undef JNI_VERSION_19
export constexpr uint32_t JNI_VERSION_19 = 0x00130000;
// #define JNI_VERSION_20  0x00140000
constexpr uint32_t M_JNI_VERSION_20 = JNI_VERSION_20;
#undef JNI_VERSION_20
export constexpr uint32_t JNI_VERSION_20 = 0x00140000;

namespace jni {
    // JT+ClassName is a constexpr Class of jni_bind function.
    export constexpr Class JTString{
        "java/lang/String",
        jni::Constructor{},
        jni::Constructor{jstring{}},
    };

    export constexpr Class JTClass{
        "java/lang/Class"
    };

    export constexpr Class JTMethod{
        "java/lang/reflect/Method"
    };

    export constexpr Class JTObject{
        "java/lang/Object",
        jni::Constructor{},
    };
}
