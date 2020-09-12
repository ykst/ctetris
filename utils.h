#pragma once
#include <stddef.h>
#ifndef swap
#define swap(l, r)                                                             \
    ({                                                                         \
        typeof(l) __tmp = (l);                                                 \
        (l) = (r);                                                             \
        (r) = __tmp;                                                           \
    })
#endif

#ifndef max
#define max(a, b)                                                              \
    ({                                                                         \
        typeof(a) __ma = (a);                                                  \
        typeof(b) __mb = (b);                                                  \
        __ma > __mb ? __ma : __mb;                                             \
    })
#endif

#ifndef min
#define min(a, b)                                                              \
    ({                                                                         \
        typeof(a) __ma = (a);                                                  \
        typeof(b) __mb = (b);                                                  \
        __ma < __mb ? __ma : __mb;                                             \
    })
#endif

#ifndef range
#define range(l, x, r) (min(max(l, x), r))
#endif

#ifndef likely
#ifdef __builtin_expect
#define likely(x) __builtin_expect((x), 1)
#else
#define likely(x) (x)
#endif
#endif

#ifndef unlikely
#ifdef __builtin_expect
#define unlikely(x) __builtin_expect((x), 0)
#else
#define unlikely(x) (x)
#endif
#endif

#ifndef container_of
#define container_of(type, ptr, member)                                        \
    ({                                                                         \
        const typeof(((type *)0)->member) *__mptr = (ptr);                     \
        (type *)((char *)__mptr - offsetof(type, member));                     \
    })
#endif

#define WARN(fmt, ...)                                                         \
    do {                                                                       \
        fprintf(stderr, "\x1b[35mWARN :%s:%s:%d: " fmt "\x1b[0m",              \
                __BASE_FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);         \
    } while (0)

#define INFO(fmt, ...)                                                         \
    do {                                                                       \
        fprintf(stderr, "\x1b[36mINFO :%s:%s:%d: " fmt "\x1b[0m",              \
                __BASE_FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);         \
    } while (0)

#define ERROR(fmt, ...)                                                        \
    do {                                                                       \
        fprintf(stderr, "\x1b[1;31mERROR:%s:%s:%d: " fmt "\x1b[0m",            \
                __BASE_FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);         \
    } while (0)

#define MASSERT(b, action, fmt, ...)                                           \
    ({                                                                         \
        bool __b = (bool)(b);                                                  \
        if (unlikely(!(__b))) {                                                \
            ERROR("failed (%s): " fmt, #b, ##__VA_ARGS__);                     \
            action;                                                            \
        }                                                                      \
        __b;                                                                   \
    })
#define MEXPECT(b, action, fmt, ...)                                           \
    ({                                                                         \
        bool __b = (bool)(b);                                                  \
        if (unlikely(!(__b))) {                                                \
            WARN("failed (%s): " fmt, #b, ##__VA_ARGS__);                      \
            action;                                                            \
        }                                                                      \
        __b;                                                                   \
    })
#define MCHECK(b, action, fmt, ...)                                            \
    ({                                                                         \
        bool __b = (bool)(b);                                                  \
        if (unlikely(!(__b))) {                                                \
            DBG("failed (%s): " fmt, #b, ##__VA_ARGS__);                       \
            action;                                                            \
        }                                                                      \
        __b;                                                                   \
    })

#define ASSERT(b, action) MASSERT(b, action, "\n")
#define EXPECT(b, action) MEXPECT(b, action, "\n")
#define CHECK(b, action) MCHECK(b, action, "\n")

#ifdef __DEBUG
#define DBG(fmt, args...)                                                      \
    do {                                                                       \
        fprintf(stderr, "DBG  :%s:%s:%d: " fmt, __BASE_FILE__, __FUNCTION__,   \
                __LINE__, ##args);                                             \
    } while (0)
#define DASSERT(b, action) MASSERT(b, action, "(debug)\n")
#else
#define DBG(fmt, args...)
#define DASSERT(b, action)
#endif
