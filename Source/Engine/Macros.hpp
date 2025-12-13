/*
 *  Filename: Macros.hpp
 *  This code is part of the Astera core library
 *  Copyright 2025 Jake Rieger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>

/* ============================================================================
 * GENERAL PURPOSE MACROS
 * ============================================================================ */

/* Array size calculation */
#define ASTERA_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Min/Max macros with type safety */
#define ASTERA_MIN(a, b)                                                                                               \
    ({                                                                                                                 \
        __typeof__(a) _a = (a);                                                                                        \
        __typeof__(b) _b = (b);                                                                                        \
        _a < _b ? _a : _b;                                                                                             \
    })

#define ASTERA_MAX(a, b)                                                                                               \
    ({                                                                                                                 \
        __typeof__(a) _a = (a);                                                                                        \
        __typeof__(b) _b = (b);                                                                                        \
        _a > _b ? _a : _b;                                                                                             \
    })

/* Clamp a value between min and max */
#define ASTERA_CLAMP(x, min, max) ASTERA_MAX((min), ASTERA_MIN((x), (max)))

/* Swap two values */
#define ASTERA_SWAP(a, b)                                                                                              \
    do {                                                                                                               \
        __typeof__(a) _tmp = (a);                                                                                      \
        (a)                = (b);                                                                                      \
        (b)                = _tmp;                                                                                     \
    } while (0)

/* Absolute value */
#define ASTERA_ABS(x)                                                                                                  \
    ({                                                                                                                 \
        __typeof__(x) _x = (x);                                                                                        \
        _x < 0 ? -_x : _x;                                                                                             \
    })

/* Check if number is power of 2 */
#define ASTERA_IS_POW2(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))

/* Align value up to nearest power of 2 boundary */
#define ASTERA_ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

/* Align value down to nearest power of 2 boundary */
#define ASTERA_ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

/* Get offset of a field in a struct */
#define ASTERA_OFFSET_OF(type, member) offsetof(type, member)

/* Get container struct from member pointer */
#define ASTERA_CONTAINER_OF(ptr, type, member)                                                                         \
    ({                                                                                                                 \
        const __typeof__(((type*)0)->member)* _mptr = (ptr);                                                           \
        (type*)((char*)_mptr - N_OFFSET_OF(type, member));                                                             \
    })

/* Bit manipulation */
#define ASTERA_BIT(n) (1U << (n))
#define ASTERA_BIT_SET(x, n) ((x) |= N_BIT(n))
#define ASTERA_BIT_CLEAR(x, n) ((x) &= ~N_BIT(n))
#define ASTERA_BIT_TOGGLE(x, n) ((x) ^= N_BIT(n))
#define ASTERA_BIT_CHECK(x, n) (((x) & N_BIT(n)) != 0)

/* Create bitmask */
#define ASTERA_BITMASK(nbits) ((1U << (nbits)) - 1)

/* Safe boolean conversion */
#define ASTERA_BOOL(x) (!!(x))

/* Stringify */
#define ASTERA_STRINGIFY_IMPL(x) #x
#define ASTERA_STRINGIFY(x) ASTERA_STRINGIFY_IMPL(x)

/* Concatenate */
#define ASTERA_CONCAT_IMPL(a, b) a##b
#define ASTERA_CONCAT(a, b) ASTERA_CONCAT_IMPL(a, b)

/* Unused variable/parameter marker */
#define ASTERA_UNUSED(x) ((void)(x))

/* Static assertion with message */
#define ASTERA_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

/* Likely/Unlikely branch prediction hints */
#if defined(__GNUC__) || defined(__clang__)
    #define ASTERA_LIKELY(x) __builtin_expect(!!(x), 1)
    #define ASTERA_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define ASTERA_LIKELY(x) (x)
    #define ASTERA_UNLIKELY(x) (x)
#endif

/* Force inline */
#if defined(__GNUC__) || defined(__clang__)
    #define ASTERA_FORCE_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    #define ASTERA_FORCE_INLINE __forceinline
#else
    #define ASTERA_FORCE_INLINE inline
#endif

/* No inline */
#if defined(__GNUC__) || defined(__clang__)
    #define ASTERA_NO_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define ASTERA_NO_INLINE __declspec(noinline)
#else
    #define ASTERA_NO_INLINE
#endif

/* ============================================================================
 * MEMORY MANAGEMENT MACROS (SAFETY FOCUSED)
 * ============================================================================ */

/* Safe malloc with type */
#define ASTERA_MALLOC(type, count) ((type*)malloc(sizeof(type) * (count)))

/* Safe calloc with type (zeroed memory) */
#define ASTERA_CALLOC(type, count) ((type*)calloc((count), sizeof(type)))

/* Safe realloc with type */
#define ASTERA_REALLOC(ptr, type, count) ((type*)realloc((ptr), sizeof(type) * (count)))

/* Allocate single object */
#define ASTERA_NEW(type) ASTERA_CALLOC(type, 1)

/* Safe free that nullifies pointer */
#define ASTERA_FREE(ptr)                                                                                               \
    do {                                                                                                               \
        free(ptr);                                                                                                     \
        (ptr) = NULL;                                                                                                  \
    } while (0)

/* Safe free with type check */
#define ASTERA_DELETE(ptr)                                                                                             \
    do {                                                                                                               \
        if ((ptr) != NULL) {                                                                                           \
            free(ptr);                                                                                                 \
            (ptr) = NULL;                                                                                              \
        }                                                                                                              \
    } while (0)

/* Zero memory */
#define ASTERA_ZERO(ptr, type) memset((ptr), 0, sizeof(type))

/* Zero array */
#define ASTERA_ZERO_ARRAY(arr) memset((arr), 0, sizeof(arr))

/* Zero struct by pointer */
#define ASTERA_ZERO_STRUCT(ptr) memset((ptr), 0, sizeof(*(ptr)))

/* Safe memcpy with bounds checking */
#define ASTERA_MEMCPY_SAFE(dest, src, dest_size, src_size)                                                             \
    do {                                                                                                               \
        size_t _copy_size = N_MIN((dest_size), (src_size));                                                            \
        memcpy((dest), (src), _copy_size);                                                                             \
    } while (0)

/* Duplicate memory */
#define ASTERA_MEMDUP(src, size)                                                                                       \
    ({                                                                                                                 \
        void* _new = malloc(size);                                                                                     \
        if (_new != NULL) memcpy(_new, (src), (size));                                                                 \
        _new;                                                                                                          \
    })

/* Check if pointer is aligned */
#define ASTERA_IS_ALIGNED(ptr, alignment) ((((uintptr_t)(ptr)) & ((alignment) - 1)) == 0)

/* Safe pointer arithmetic */
#define ASTERA_PTR_ADD(ptr, offset) ((void*)((char*)(ptr) + (offset)))
#define ASTERA_PTR_SUB(ptr, offset) ((void*)((char*)(ptr) - (offset)))
#define ASTERA_PTR_DIFF(ptr1, ptr2) ((size_t)((char*)(ptr1) - (char*)(ptr2)))

/* ============================================================================
 * GRAPHICS & GAME SPECIFIC MACROS
 * ============================================================================ */

/* Color manipulation (RGBA) */
#define ASTERA_COLOR_RGBA(r, g, b, a)                                                                                  \
    ((uint32_t)(((uint8_t)(r)) | ((uint8_t)(g) << 8) | ((uint8_t)(b) << 16) | ((uint8_t)(a) << 24)))

#define ASTERA_COLOR_RGB(r, g, b) N_COLOR_RGBA(r, g, b, 255)

#define ASTERA_COLOR_GET_R(color) ((uint8_t)((color) & 0xFF))
#define ASTERA_COLOR_GET_G(color) ((uint8_t)(((color) >> 8) & 0xFF))
#define ASTERA_COLOR_GET_B(color) ((uint8_t)(((color) >> 16) & 0xFF))
#define ASTERA_COLOR_GET_A(color) ((uint8_t)(((color) >> 24) & 0xFF))

/* Convert float color (0.0-1.0) to byte (0-255) */
#define ASTERA_COLOR_F2B(f) ((uint8_t)N_CLAMP((f) * 255.0f, 0.0f, 255.0f))

/* Convert byte color (0-255) to float (0.0-1.0) */
#define ASTERA_COLOR_B2F(b) ((float)(b) / 255.0f)

/* Rectangle operations */
#define ASTERA_RECT_CONTAINS(rect_x, rect_y, rect_w, rect_h, px, py)                                                   \
    (((px) >= (rect_x)) && ((px) < (rect_x) + (rect_w)) && ((py) >= (rect_y)) && ((py) < (rect_y) + (rect_h)))

/* Check if two rectangles intersect */
#define ASTERA_RECT_INTERSECTS(x1, y1, w1, h1, x2, y2, w2, h2)                                                         \
    (!((x1) + (w1) <= (x2) || (x2) + (w2) <= (x1) || (y1) + (h1) <= (y2) || (y2) + (h2) <= (y1)))

/* Degrees to radians */
#define ASTERA_DEG2RAD(deg) ((deg) * 0.017453292519943295f)

/* Radians to degrees */
#define ASTERA_RAD2DEG(rad) ((rad) * 57.29577951308232f)

/* Linear interpolation */
#define ASTERA_LERP(a, b, t) ((a) + ((b) - (a)) * (t))

/* Inverse lerp (find t given a, b, and value) */
#define ASTERA_INV_LERP(a, b, v) (((v) - (a)) / ((b) - (a)))

/* Remap value from one range to another */
#define ASTERA_REMAP(val, in_min, in_max, out_min, out_max)                                                            \
    ASTERA_LERP((out_min), (out_max), ASTERA_INV_LERP((in_min), (in_max), (val)))

/* Smoothstep interpolation */
#define ASTERA_SMOOTHSTEP(t) ((t) * (t) * (3.0f - 2.0f * (t)))

/* Check if float values are approximately equal */
#define ASTERA_FLOAT_EQ(a, b, epsilon) (ASTERA_ABS((a) - (b)) < (epsilon))

/* Default epsilon for float comparison */
#define ASTERA_EPSILON 1e-6f

/* Normalize value to 0.0-1.0 range */
#define ASTERA_NORMALIZE(val, min, max) ASTERA_CLAMP(((val) - (min)) / ((max) - (min)), 0.0f, 1.0f)

/* Convert normalized value back to range */
#define ASTERA_DENORMALIZE(val, min, max) ((val) * ((max) - (min)) + (min))

/* 2D Vector operations (requires vec2 struct with x, y) */
#define ASTERA_VEC2_DOT(v1, v2) ((v1).x * (v2).x + (v1).y * (v2).y)
#define ASTERA_VEC2_LENGTH_SQ(v) ((v).x * (v).x + (v).y * (v).y)

/* 3D Vector operations (requires vec3 struct with x, y, z) */
#define ASTERA_VEC3_DOT(v1, v2) ((v1).x * (v2).x + (v1).y * (v2).y + (v1).z * (v2).z)
#define ASTERA_VEC3_LENGTH_SQ(v) ((v).x * (v).x + (v).y * (v).y + (v).z * (v).z)

/* ============================================================================
 * ASSET & RESOURCE MANAGEMENT
 * ============================================================================ */

/* Safe string duplication */
#define ASTERA_STRDUP_SAFE(str)                                                                                        \
    ({                                                                                                                 \
        const char* _s = (str);                                                                                        \
        char* _dup     = NULL;                                                                                         \
        if (_s != NULL) {                                                                                              \
            size_t _len = strlen(_s) + 1;                                                                              \
            _dup        = (char*)malloc(_len);                                                                         \
            if (_dup != NULL) memcpy(_dup, _s, _len);                                                                  \
        }                                                                                                              \
        _dup;                                                                                                          \
    })

/* Safe string copy with null termination guarantee */
#define ASTERA_STRCPY_SAFE(dest, dest_size, src)                                                                       \
    do {                                                                                                               \
        if ((dest) != NULL && (src) != NULL && (dest_size) > 0) {                                                      \
            strncpy((dest), (src), (dest_size) - 1);                                                                   \
            (dest)[(dest_size) - 1] = '\0';                                                                            \
        }                                                                                                              \
    } while (0)

/* String comparison shortcuts */
#define ASTERA_STREQ(s1, s2) (strcmp((s1), (s2)) == 0)
#define ASTERA_STRNEQ(s1, s2, n) (strncmp((s1), (s2), (n)) == 0)

/* ============================================================================
 * DEBUG & LOGGING MACROS
 * ============================================================================ */

#ifndef NDEBUG
    #define ASTERA_DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

    #define ASTERA_ASSERT(cond)                                                                                        \
        do {                                                                                                           \
            if (ASTERA_UNLIKELY(!(cond))) {                                                                            \
                fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #cond, __FILE__, __LINE__);                \
                abort();                                                                                               \
            }                                                                                                          \
        } while (0)

    #define ASTERA_ASSERT_MSG(cond, msg)                                                                               \
        do {                                                                                                           \
            if (ASTERA_UNLIKELY(!(cond))) {                                                                            \
                fprintf(stderr, "Assertion failed: %s (%s), file %s, line %d\n", #cond, msg, __FILE__, __LINE__);      \
                abort();                                                                                               \
            }                                                                                                          \
        } while (0)
#else
    #define ASTERA_DEBUG_PRINT(fmt, ...) ((void)0)
    #define ASTERA_ASSERT(cond) ((void)0)
    #define ASTERA_ASSERT_MSG(cond, msg) ((void)0)
#endif

#define ASTERA_PRINT_ERROR(fmt, ...) fprintf(stderr, "[ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/* Runtime error checking (always active) */
#define ASTERA_CHECK(cond)                                                                                             \
    do {                                                                                                               \
        if (ASTERA_UNLIKELY(!(cond))) {                                                                                \
            fprintf(stderr, "Check failed: %s, file %s, line %d\n", #cond, __FILE__, __LINE__);                        \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

#define ASTERA_CHECK_MSG(cond, msg)                                                                                    \
    do {                                                                                                               \
        if (ASTERA_UNLIKELY(!(cond))) {                                                                                \
            fprintf(stderr, "Check failed: %s (%s), file %s, line %d\n", #cond, msg, __FILE__, __LINE__);              \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

/* Verify allocation succeeded */
#define ASTERA_CHECK_ALLOC(ptr) ASTERA_CHECK_MSG((ptr) != NULL, "Memory allocation failed")

/* ============================================================================
 * PERFORMANCE & OPTIMIZATION
 * ============================================================================ */

/* Prefetch memory (compiler-specific) */
#if defined(__GNUC__) || defined(__clang__)
    #define ASTERA_PREFETCH(addr) __builtin_prefetch(addr)
    #define ASTERA_PREFETCH_WRITE(addr) __builtin_prefetch(addr, 1)
#else
    #define ASTERA_PREFETCH(addr) ((void)0)
    #define ASTERA_PREFETCH_WRITE(addr) ((void)0)
#endif

/* Restrict pointer keyword */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define ASTERA_RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__)
    #define ASTERA_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define ASTERA_RESTRICT __restrict
#else
    #define ASTERA_RESTRICT
#endif

/* Function attributes */
#if defined(__GNUC__) || defined(__clang__)
    #define ASTERA_PURE __attribute__((pure))
    #define ASTERA_CONST __attribute__((const))
    #define ASTERA_HOT __attribute__((hot))
    #define ASTERA_COLD __attribute__((cold))
#else
    #define ASTERA_PURE
    #define ASTERA_CONST
    #define ASTERA_HOT
    #define ASTERA_COLD
#endif

/* ============================================================================
 * FOREACH LOOP HELPERS
 * ============================================================================ */

/* Iterate over array with automatic size detection */
#define ASTERA_FOREACH(type, var, array) for (type* var = (array); var < (array) + ASTERA_ARRAY_SIZE(array); ++var)

/* Iterate with index */
#define ASTERA_FOREACH_I(type, var, array, index_var)                                                                  \
    for (size_t index_var = 0, _keep = 1; _keep && index_var < ASTERA_ARRAY_SIZE(array); _keep = !_keep, ++index_var)  \
        for (type* var = &(array)[index_var]; _keep; _keep = !_keep)

/* ============================================================================
 * TIMER MACROS
 * ============================================================================ */

/* Simple frame time calculation helper */
#define ASTERA_DELTA_TIME(last_time, current_time) ((float)((current_time) - (last_time)))

/* Convert seconds to milliseconds */
#define ASTERA_SEC_TO_MS(sec) ((sec) * 1000.0f)

/* Convert milliseconds to seconds */
#define ASTERA_MS_TO_SEC(ms) ((ms) / 1000.0f)

/* FPS calculation from delta time */
#define ASTERA_FPS_FROM_DT(dt) (1.0f / (dt))

/// @brief Deletes both the move/copy assignment operator and constructor
#define ASTERA_CLASS_PREVENT_MOVES_COPIES(CLASS_NAME)                                                                  \
    CLASS_NAME(const CLASS_NAME&)            = delete;                                                                 \
    CLASS_NAME(CLASS_NAME&&)                 = delete;                                                                 \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;                                                                 \
    CLASS_NAME& operator=(CLASS_NAME&&)      = delete;

/// @brief Deletes the copy constructor and assignment operator
#define ASTERA_CLASS_PREVENT_COPIES(CLASS_NAME)                                                                        \
    CLASS_NAME(const CLASS_NAME&)            = delete;                                                                 \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;

/// @brief Deletes the move constructor and assignment operator
#define ASTERA_CLASS_PREVENT_MOVES(CLASS_NAME)                                                                         \
    CLASS_NAME(CLASS_NAME&&)            = delete;                                                                      \
    CLASS_NAME& operator=(CLASS_NAME&&) = delete;

#define ASTERA_KEEP [[nodiscard]]

#define ASTERA_DO_NOTHING ((void)0)