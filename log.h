#ifndef WATCH_LOG_H_
#define WATCH_LOG_H_
#ifndef WATCH_LOG_CORE_H_
#define WATCH_LOG_CORE_H_
#ifndef WATCH_LOG_MSG_H_
#define WATCH_LOG_MSG_H_

#include <kernel.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include i2c.h
#ifndef WATCH_ATOMIC_H_
#define WATCH_ATOMIC_H_
#ifndef WATCH_TOOLCHAIN_H_
#define WATCH_TOOLCHAIN_H_
#ifndef WATCH_TOOLCHAIN_GCC_H_
#define WATCH_TOOLCHAIN_GCC_H_
#ifndef WATCH_TOOLCHAIN_COMMON_H_
#define WATCH_TOOLCHAIN_COMMON_H_

#if defined(__XCC__)
#include <toolchain/xcc.h>
#elif defined(__GNUC__) || (defined(_LINKER) && defined(__GCC_LINKER_CMD__))

#ifdef __cplusplus
#define BUILD_ASSERT(EXPR) static_assert(EXPR, "")
#define BUILD_ASSERT_MSG(EXPR, MSG) static_assert(EXPR, MSG)

#elif (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) || \
	(__STDC_VERSION__) >= 201100
#define BUILD_ASSERT(EXPR) _Static_assert(EXPR, "")
#define BUILD_ASSERT_MSG(EXPR, MSG) _Static_assert(EXPR, MSG)
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

/* Use TASK_ENTRY_CPP to tag task entry points defined in C++ files. */

#ifdef __cplusplus
#define TASK_ENTRY_CPP  extern "C"
#endif

#ifdef _ASMLANGUAGE
  #define REQUIRES(sym) .set sym ## _Requires, sym
#else
  #define REQUIRES(sym) __asm__ (".set " # sym "_Requires, " # sym "\n\t");
#endif

#ifdef _ASMLANGUAGE
  #define SECTION .section
#endif

#ifdef _ASMLANGUAGE

  #if defined(CONFIG_X86) || defined(CONFIG_X86_64)

    #ifdef PERF_OPT
      #define PERFOPT_ALIGN .balign 16
    #else
      #define PERFOPT_ALIGN .balign  1
    #endif

  #elif defined(CONFIG_ARM)

    #define PERFOPT_ALIGN .balign  4

  #elif defined(CONFIG_ARC)

    #define PERFOPT_ALIGN .balign  4

  #elif defined(CONFIG_NIOS2) || defined(CONFIG_RISCV32) || \
	  defined(CONFIG_XTENSA)
    #define PERFOPT_ALIGN .balign 4

  #elif defined(CONFIG_ARCH_POSIX)

  #else

    #error Architecture unsupported

  #endif

  #define GC_SECTION(sym) SECTION .text.##sym, "ax"

#endif /* _ASMLANGUAGE */

/* force inlining a function */

#if !defined(_ASMLANGUAGE)
  #ifdef CONFIG_COVERAGE
    
    #define ALWAYS_INLINE __attribute__((unused))
  #else
    #define ALWAYS_INLINE inline __attribute__((always_inline))
  #endif
#endif

#define Z_STRINGIFY(x) #x
#define STRINGIFY(s) Z_STRINGIFY(s)

/* concatenate the values of the arguments into one */
#define _DO_CONCAT(x, y) x ## y
#define _CONCAT(x, y) _DO_CONCAT(x, y)

#ifndef ZTEST_UNITTEST
#define __syscall static inline
#else
#define __syscall
#endif /* #ifndef ZTEST_UNITTEST */

#ifndef BUILD_ASSERT
/* compile-time assertion that makes the build fail */
#define BUILD_ASSERT(EXPR) \
	enum _CONCAT(__build_assert_enum, __COUNTER__) { \
		_CONCAT(__build_assert, __COUNTER__) = 1 / !!(EXPR) \
	}
#endif
#ifndef BUILD_ASSERT_MSG
/* build assertion with message -- common implementation swallows message. */
#define BUILD_ASSERT_MSG(EXPR, MSG) BUILD_ASSERT(EXPR)
#endif

#endif /* WATCH_TOOLCHAIN_COMMON_H_ */


#define ALIAS_OF(of) __attribute__((alias(#of)))

#define FUNC_ALIAS(real_func, new_alias, return_type) \
	return_type new_alias() ALIAS_OF(real_func)

#if defined(CONFIG_ARCH_POSIX)
/*let's not segfault if this were to happen for some reason*/
#define CODE_UNREACHABLE \
{\
	posix_print_error_and_exit("CODE_UNREACHABLE reached from %s:%d\n",\
		__FILE__, __LINE__);\
	__builtin_unreachable(); \
}
#else
#define CODE_UNREACHABLE __builtin_unreachable()
#endif
#define FUNC_NORETURN    __attribute__((__noreturn__))

#if defined(CONFIG_ARM)
#define _NODATA_SECTION(segment)  __attribute__((section(#segment)))
#else
#define _NODATA_SECTION(segment)				\
	__attribute__((section(#segment ",\"wa\",@nobits#")))
#endif

/* Unaligned access */
#define UNALIGNED_GET(p)						\
__extension__ ({							\
	struct  __attribute__((__packed__)) {				\
		__typeof__(*(p)) __v;					\
	} *__p = (__typeof__(__p)) (p);					\
	__p->__v;							\
})


#if __GNUC__ >= 7 && defined(CONFIG_ARM)

#define UNALIGNED_PUT(v, p)                                             \
do {                                                                    \
	struct __attribute__((__packed__)) {                            \
		__typeof__(*p) __v;                                     \
	} *__p = (__typeof__(__p)) (p);                                 \
	__p->__v = (v);                                                 \
	compiler_barrier();                                             \
} while (false)

#else

#define UNALIGNED_PUT(v, p)                                             \
do {                                                                    \
	struct __attribute__((__packed__)) {                            \
		__typeof__(*p) __v;                                     \
	} *__p = (__typeof__(__p)) (p);                                 \
	__p->__v = (v);                                               \
} while (false)

#endif

#define __GENERIC_SECTION(segment) __attribute__((section(STRINGIFY(segment))))
#define Z_GENERIC_SECTION(segment) __GENERIC_SECTION(segment)

#define ___in_section(a, b, c) \
	__attribute__((section("." Z_STRINGIFY(a)			\
				"." Z_STRINGIFY(b)			\
				"." Z_STRINGIFY(c))))
#define __in_section(a, b, c) ___in_section(a, b, c)

#define __in_section_unique(seg) ___in_section(seg, __FILE__, __COUNTER__)

#if !defined(CONFIG_XIP)
#define __ramfunc
#elif defined(CONFIG_ARCH_HAS_RAMFUNC_SUPPORT)
#define __ramfunc	__attribute__((noinline))			\
			__attribute__((long_call, section(".ramfunc")))
#endif /* !CONFIG_XIP */

#ifndef __packed
#define __packed        __attribute__((__packed__))
#endif
#ifndef __aligned
#define __aligned(x)	__attribute__((__aligned__(x)))
#endif
#define __may_alias     __attribute__((__may_alias__))
#ifndef __printf_like
#define __printf_like(f, a)   __attribute__((format (printf, f, a)))
#endif
#define __used		__attribute__((__used__))
#define __deprecated	__attribute__((deprecated))
#define ARG_UNUSED(x) (void)(x)

#define likely(x)   __builtin_expect((bool)!!(x), true)
#define unlikely(x) __builtin_expect((bool)!!(x), false)

#define popcount(x) __builtin_popcount(x)

#ifndef __weak
#define __weak __attribute__((__weak__))
#endif
#define __unused __attribute__((__unused__))

#define __DEPRECATED_MACRO _Pragma("GCC warning \"Macro is deprecated\"")

/* These macros allow having ARM asm functions callable from thumb */

#if defined(_ASMLANGUAGE) && !defined(_LINKER)

#ifdef CONFIG_ARM

#if defined(CONFIG_ISA_THUMB2)

#define FUNC_CODE() .thumb;
#define FUNC_INSTR(a)

#else

#error unknown instruction set

#endif /* ISA */

#else

#define FUNC_CODE()
#define FUNC_INSTR(a)

#endif /* !CONFIG_ARM */

#endif /* _ASMLANGUAGE && !_LINKER */


#if defined(_ASMLANGUAGE) && !defined(_LINKER)

#if defined(CONFIG_ARM) || defined(CONFIG_NIOS2) || defined(CONFIG_RISCV32) \
	|| defined(CONFIG_XTENSA)
#define GTEXT(sym) .global sym; .type sym, %function
#define GDATA(sym) .global sym; .type sym, %object
#define WTEXT(sym) .weak sym; .type sym, %function
#define WDATA(sym) .weak sym; .type sym, %object
#elif defined(CONFIG_ARC)

.macro glbl_text symbol
	.globl \symbol
	.type \symbol, %function
.endm

.macro glbl_data symbol
	.globl \symbol
	.type \symbol, %object
.endm

.macro weak_data symbol
	.weak \symbol
	.type \symbol, %object
.endm

#define GTEXT(sym) glbl_text sym
#define GDATA(sym) glbl_data sym
#define WDATA(sym) weak_data sym

#else  /* !CONFIG_ARM && !CONFIG_ARC */
#define GTEXT(sym) .globl sym; .type sym, @function
#define GDATA(sym) .globl sym; .type sym, @object
#endif

#if defined(CONFIG_ARC)

.macro section_var section, symbol
	.section .\section\().\symbol
	\symbol :
.endm

.macro section_func section, symbol
	.section .\section\().\symbol, "ax"
	FUNC_CODE()
	PERFOPT_ALIGN
	\symbol :
	FUNC_INSTR(\symbol)
.endm

.macro section_subsec_func section, subsection, symbol
	.section .\section\().\subsection, "ax"
	PERFOPT_ALIGN
	\symbol :
.endm

#define SECTION_VAR(sect, sym) section_var sect, sym
#define SECTION_FUNC(sect, sym) section_func sect, sym
#define SECTION_SUBSEC_FUNC(sect, subsec, sym) \
	section_subsec_func sect, subsec, sym
#else /* !CONFIG_ARC */

#define SECTION_VAR(sect, sym)  .section .sect.##sym; sym :
#define SECTION_FUNC(sect, sym)						\
	.section .sect.sym, "ax";					\
				FUNC_CODE()				\
				PERFOPT_ALIGN; sym :		\
							FUNC_INSTR(sym)
#define SECTION_SUBSEC_FUNC(sect, subsec, sym)				\
		.section .sect.subsec, "ax"; PERFOPT_ALIGN; sym :

#endif /* CONFIG_ARC */

#endif /* _ASMLANGUAGE && !_LINKER */

#if defined(CONFIG_ARM) && defined(_ASMLANGUAGE)
#if defined(CONFIG_ISA_THUMB2)
/* '.syntax unified' is a gcc-ism used in thumb-2 asm files */
#define _ASM_FILE_PROLOGUE .text; .syntax unified; .thumb
#else
#define _ASM_FILE_PROLOGUE .text; .code 32
#endif
#endif

#define GEN_OFFSET_EXTERN(name) extern const char name[]

#define GEN_ABS_SYM_BEGIN(name) \
	EXTERN_C void name(void); \
	void name(void)         \
	{

#define GEN_ABS_SYM_END }

#if defined(CONFIG_ARM)

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%B0"                              \
		"\n\t.type\t" #name ",%%object" :  : "n"(~(value)))

#elif defined(CONFIG_X86) || defined(CONFIG_ARC)

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%c0"                              \
		"\n\t.type\t" #name ",@object" :  : "n"(value))

#elif defined(CONFIG_X86_64)

#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%0"                               \
		"\n\t.type\t" #name ",@object" :  : "n"(value))

#elif defined(CONFIG_NIOS2) || defined(CONFIG_RISCV32) || defined(CONFIG_XTENSA)

/* No special prefixes necessary for constants in this arch AFAICT */
#define GEN_ABSOLUTE_SYM(name, value)		\
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%0"                              \
		"\n\t.type\t" #name ",%%object" :  : "n"(value))

#elif defined(CONFIG_ARCH_POSIX)
#define GEN_ABSOLUTE_SYM(name, value)               \
	__asm__(".globl\t" #name "\n\t.equ\t" #name \
		",%c0"                              \
		"\n\t.type\t" #name ",@object" :  : "n"(value))
#else
#error processor architecture not supported
#endif

#define compiler_barrier() do { \
	__asm__ __volatile__ ("" ::: "memory"); \
} while (false)

#endif /* WATCH_TOOLCHAIN_GCC_H_ */
#else
#include <toolchain/other.h>
#endif

#endif /* WATCH_TOOLCHAIN_H_ */

#ifdef __cplusplus
extern "C" {
#endif

typedef int atomic_t;
typedef atomic_t atomic_val_t;

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline bool atomic_cas(atomic_t *target, atomic_val_t old_value,
			  atomic_val_t new_value)
{
	return __atomic_compare_exchange_n(target, &old_value, new_value,
					   0, __ATOMIC_SEQ_CST,
					   __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall int atomic_cas(atomic_t *target, atomic_val_t old_value,
			 atomic_val_t new_value);

#else
extern int atomic_cas(atomic_t *target, atomic_val_t old_value,
		      atomic_val_t new_value);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_add(atomic_t *target, atomic_val_t value)
{
	return __atomic_fetch_add(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_add(atomic_t *target, atomic_val_t value);
#else
extern atomic_val_t atomic_add(atomic_t *target, atomic_val_t value);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_sub(atomic_t *target, atomic_val_t value)
{
	return __atomic_fetch_sub(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_sub(atomic_t *target, atomic_val_t value);
#else
extern atomic_val_t atomic_sub(atomic_t *target, atomic_val_t value);
#endif

#if defined(CONFIG_ATOMIC_OPERATIONS_BUILTIN) || defined (CONFIG_ATOMIC_OPERATIONS_C)
static inline atomic_val_t atomic_inc(atomic_t *target)
{
	return atomic_add(target, 1);
}
#else
extern atomic_val_t atomic_inc(atomic_t *target);
#endif

#if defined(CONFIG_ATOMIC_OPERATIONS_BUILTIN) || defined (CONFIG_ATOMIC_OPERATIONS_C)
static inline atomic_val_t atomic_dec(atomic_t *target)
{
	return atomic_sub(target, 1);
}
#else
extern atomic_val_t atomic_dec(atomic_t *target);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_get(const atomic_t *target)
{
	return __atomic_load_n(target, __ATOMIC_SEQ_CST);
}
#else
extern atomic_val_t atomic_get(const atomic_t *target);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_set(atomic_t *target, atomic_val_t value)
{
	/* This builtin, as described by Intel, is not a traditional
	 * test-and-set operation, but rather an atomic exchange operation. It
	 * writes value into *ptr, and returns the previous contents of *ptr.
	 */
	return __atomic_exchange_n(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_set(atomic_t *target, atomic_val_t value);
#else
extern atomic_val_t atomic_set(atomic_t *target, atomic_val_t value);
#endif

#if defined(CONFIG_ATOMIC_OPERATIONS_BUILTIN) || defined (CONFIG_ATOMIC_OPERATIONS_C)
static inline atomic_val_t atomic_clear(atomic_t *target)
{
	return atomic_set(target, 0);
}
#else
extern atomic_val_t atomic_clear(atomic_t *target);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_or(atomic_t *target, atomic_val_t value)
{
	return __atomic_fetch_or(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_or(atomic_t *target, atomic_val_t value);

#else
extern atomic_val_t atomic_or(atomic_t *target, atomic_val_t value);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_xor(atomic_t *target, atomic_val_t value)
{
	return __atomic_fetch_xor(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_xor(atomic_t *target, atomic_val_t value);
#else
extern atomic_val_t atomic_xor(atomic_t *target, atomic_val_t value);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_and(atomic_t *target, atomic_val_t value)
{
	return __atomic_fetch_and(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_and(atomic_t *target, atomic_val_t value);
#else
extern atomic_val_t atomic_and(atomic_t *target, atomic_val_t value);
#endif

#ifdef CONFIG_ATOMIC_OPERATIONS_BUILTIN
static inline atomic_val_t atomic_nand(atomic_t *target, atomic_val_t value)
{
	return __atomic_fetch_nand(target, value, __ATOMIC_SEQ_CST);
}
#elif defined(CONFIG_ATOMIC_OPERATIONS_C)
__syscall atomic_val_t atomic_nand(atomic_t *target, atomic_val_t value);
#else
extern atomic_val_t atomic_nand(atomic_t *target, atomic_val_t value);
#endif

#define ATOMIC_INIT(i) (i)

#define ATOMIC_BITS (sizeof(atomic_val_t) * 8)
#define ATOMIC_MASK(bit) (1 << ((u32_t)(bit) & (ATOMIC_BITS - 1)))
#define ATOMIC_ELEM(addr, bit) ((addr) + ((bit) / ATOMIC_BITS))

#define ATOMIC_DEFINE(name, num_bits) \
	atomic_t name[1 + ((num_bits) - 1) / ATOMIC_BITS]

static inline bool atomic_test_bit(const atomic_t *target, int bit)
{
	atomic_val_t val = atomic_get(ATOMIC_ELEM(target, bit));

	return (1 & (val >> (bit & (ATOMIC_BITS - 1)))) != 0;
}

static inline bool atomic_test_and_clear_bit(atomic_t *target, int bit)
{
	atomic_val_t mask = ATOMIC_MASK(bit);
	atomic_val_t old;

	old = atomic_and(ATOMIC_ELEM(target, bit), ~mask);

	return (old & mask) != 0;
}

static inline bool atomic_test_and_set_bit(atomic_t *target, int bit)
{
	atomic_val_t mask = ATOMIC_MASK(bit);
	atomic_val_t old;

	old = atomic_or(ATOMIC_ELEM(target, bit), mask);

	return (old & mask) != 0;
}

static inline void atomic_clear_bit(atomic_t *target, int bit)
{
	atomic_val_t mask = ATOMIC_MASK(bit);

	(void)atomic_and(ATOMIC_ELEM(target, bit), ~mask);
}

static inline void atomic_set_bit(atomic_t *target, int bit)
{
	atomic_val_t mask = ATOMIC_MASK(bit);

	(void)atomic_or(ATOMIC_ELEM(target, bit), mask);
}

static inline void atomic_set_bit_to(atomic_t *target, int bit, bool val)
{
	atomic_val_t mask = ATOMIC_MASK(bit);

	if (val) {
		(void)atomic_or(ATOMIC_ELEM(target, bit), mask);
	} else {
		(void)atomic_and(ATOMIC_ELEM(target, bit), ~mask);
	}
}

#ifdef CONFIG_ATOMIC_OPERATIONS_C
#include <syscalls/atomic.h>
#endif

#ifdef __cplusplus
}
#endif

#endif /* WATCH_ATOMIC_H_ */

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_MAX_NARGS 15

/** @brief Number of arguments in the log entry which fits in one chunk.*/
#define LOG_MSG_NARGS_SINGLE_CHUNK 3

/** @brief Number of arguments in the head of extended standard log message..*/
#define LOG_MSG_NARGS_HEAD_CHUNK (LOG_MSG_NARGS_SINGLE_CHUNK - 1)

/** @brief Maximal amount of bytes in the hexdump entry which fits in one chunk.
 */
#define LOG_MSG_HEXDUMP_BYTES_SINGLE_CHUNK \
	(LOG_MSG_NARGS_SINGLE_CHUNK * sizeof(u32_t))

/** @brief Number of bytes in the first chunk of hexdump message if message
 *         consists of more than one chunk.
 */
#define LOG_MSG_HEXDUMP_BYTES_HEAD_CHUNK \
	(LOG_MSG_HEXDUMP_BYTES_SINGLE_CHUNK - sizeof(void *))

/** @brief Number of bytes that can be stored in chunks following head chunk
 *         in hexdump log message.
 */
#define HEXDUMP_BYTES_CONT_MSG \
	(sizeof(struct log_msg) - sizeof(void *))

#define ARGS_CONT_MSG (HEXDUMP_BYTES_CONT_MSG / sizeof(u32_t))

/** @brief Flag indicating standard log message. */
#define LOG_MSG_TYPE_STD 0

/** @brief Flag indicating hexdump log message. */
#define LOG_MSG_TYPE_HEXDUMP 1

/** @brief Common part of log message header. */
#define COMMON_PARAM_HDR() \
	u16_t type : 1;	   \
	u16_t ext : 1

/** @brief Number of bits used for storing length of hexdump log message. */
#define LOG_MSG_HEXDUMP_LENGTH_BITS 14

/** @brief Maximum length of log hexdump message. */
#define LOG_MSG_HEXDUMP_MAX_LENGTH (BIT(LOG_MSG_HEXDUMP_LENGTH_BITS) - 1)

/** @brief Part of log message header identifying source and level. */
struct log_msg_ids {
	u16_t level     : 3;    /*!< Severity. */
	u16_t domain_id : 3;    /*!< Originating domain. */
	u16_t source_id : 10;   /*!< Source ID. */
};

BUILD_ASSERT_MSG((sizeof(struct log_msg_ids) == sizeof(u16_t)),
		  "Structure must fit in 2 bytes");

/** Part of log message header common to standard and hexdump log message. */
struct log_msg_generic_hdr {
	COMMON_PARAM_HDR();
	u16_t reserved : 14;
};

BUILD_ASSERT_MSG((sizeof(struct log_msg_generic_hdr) == sizeof(u16_t)),
		 "Structure must fit in 2 bytes");

/** Part of log message header specific to standard log message. */
struct log_msg_std_hdr {
	COMMON_PARAM_HDR();
	u16_t reserved : 10;
	u16_t nargs    : 4;
};

BUILD_ASSERT_MSG((sizeof(struct log_msg_std_hdr) == sizeof(u16_t)),
		 "Structure must fit in 2 bytes");

/** Part of log message header specific to hexdump log message. */
struct log_msg_hexdump_hdr {
	COMMON_PARAM_HDR();
	u16_t length     : LOG_MSG_HEXDUMP_LENGTH_BITS;
};

BUILD_ASSERT_MSG((sizeof(struct log_msg_hexdump_hdr) == sizeof(u16_t)),
		 "Structure must fit in 2 bytes");

/** Log message header structure */
struct log_msg_hdr {
	atomic_t ref_cnt; /*!< Reference counter for tracking message users. */
	union log_msg_hdr_params {
		struct log_msg_generic_hdr generic;
		struct log_msg_std_hdr std;
		struct log_msg_hexdump_hdr hexdump;
		u16_t raw;
	} params;
	struct log_msg_ids ids; /*!< Identification part of the message.*/
	u32_t timestamp;        /*!< Timestamp. */
};

/** @brief Data part of log message. */
union log_msg_head_data {
	u32_t args[LOG_MSG_NARGS_SINGLE_CHUNK];
	u8_t bytes[LOG_MSG_HEXDUMP_BYTES_SINGLE_CHUNK];
};

/** @brief Data part of extended log message. */
struct log_msg_ext_head_data {
	struct log_msg_cont *next;
	union log_msg_ext_head_data_data {
		u32_t args[LOG_MSG_NARGS_HEAD_CHUNK];
		u8_t bytes[LOG_MSG_HEXDUMP_BYTES_HEAD_CHUNK];
	} data;
};

/** @brief Log message structure. */
struct log_msg {
	struct log_msg *next;   /*!< Used by logger core list.*/
	struct log_msg_hdr hdr; /*!< Message header. */
	const char *str;
	union log_msg_data {
		union log_msg_head_data single;
		struct log_msg_ext_head_data ext;
	} payload;                 /*!< Message data. */
};

BUILD_ASSERT_MSG((sizeof(union log_msg_head_data) ==
		  sizeof(struct log_msg_ext_head_data)),
		  "Structure must be same size");

/** @brief Chunks following message head when message is extended. */
struct log_msg_cont {
	struct log_msg_cont *next; /*!< Pointer to the next chunk. */
	union log_msg_cont_data {
		u32_t args[ARGS_CONT_MSG];
		u8_t bytes[HEXDUMP_BYTES_CONT_MSG];
	} payload;
};

/** @brief Log message */
union log_msg_chunk {
	struct log_msg head;
	struct log_msg_cont cont;
};

extern struct k_mem_slab log_msg_pool;

/** @brief Function for initialization of the log message pool. */
void log_msg_pool_init(void);

/** @brief Function for indicating that message is in use.
 */
void log_msg_get(struct log_msg *msg);

/** @brief Function for indicating that message is no longer in use.
 */
void log_msg_put(struct log_msg *msg);

/** @brief Get domain ID of the message.
 */
static inline u32_t log_msg_domain_id_get(struct log_msg *msg)
{
	return msg->hdr.ids.domain_id;
}

/** @brief Get source ID (module or instance) of the message.
 */
static inline u32_t log_msg_source_id_get(struct log_msg *msg)
{
	return msg->hdr.ids.source_id;
}

/** @brief Get severity level of the message.
 */
static inline u32_t log_msg_level_get(struct log_msg *msg)
{
	return msg->hdr.ids.level;
}

/** @brief Get timestamp of the message.
 */
static inline u32_t log_msg_timestamp_get(struct log_msg *msg)
{
	return msg->hdr.timestamp;
}

/** @brief Check if message is of standard type.
 */
static inline bool log_msg_is_std(struct log_msg *msg)
{
	return  (msg->hdr.params.generic.type == LOG_MSG_TYPE_STD);
}

/** @brief Returns number of arguments in standard log message.
 */
u32_t log_msg_nargs_get(struct log_msg *msg);

/** @brief Gets argument from standard log message.
 */
u32_t log_msg_arg_get(struct log_msg *msg, u32_t arg_idx);


/** @brief Gets pointer to the unformatted string from standard log message.
 */
const char *log_msg_str_get(struct log_msg *msg);

/** @brief Allocates chunks for hexdump message and copies the data.
 */
struct log_msg *log_msg_hexdump_create(const char *str,
				       const u8_t *data,
				       u32_t length);

/** @brief Put data into hexdump log message.
 */
void log_msg_hexdump_data_put(struct log_msg *msg,
			      u8_t *data,
			      size_t *length,
			      size_t offset);

/** @brief Get data from hexdump log message.
 */
void log_msg_hexdump_data_get(struct log_msg *msg,
			      u8_t *data,
			      size_t *length,
			      size_t offset);

union log_msg_chunk *log_msg_no_space_handle(void);

static inline union log_msg_chunk *log_msg_chunk_alloc(void)
{
	union log_msg_chunk *msg = NULL;
	int err = k_mem_slab_alloc(&log_msg_pool, (void **)&msg, K_NO_WAIT);

	if (err != 0) {
		msg = log_msg_no_space_handle();
	}

	return msg;
}

/** @brief Allocate chunk for standard log message.
 */
static inline struct log_msg *z_log_msg_std_alloc(void)
{
	struct  log_msg *msg = (struct  log_msg *)log_msg_chunk_alloc();

	if (msg != NULL) {
		/* all fields reset to 0, reference counter to 1 */
		msg->hdr.ref_cnt = 1;
		msg->hdr.params.raw = 0U;
		msg->hdr.params.std.type = LOG_MSG_TYPE_STD;
	}

	return msg;
}

/** @brief Create standard log message with no arguments.
 */
static inline struct log_msg *log_msg_create_0(const char *str)
{
	struct log_msg *msg = z_log_msg_std_alloc();

	if (msg != NULL) {
		msg->str = str;
	}

	return msg;
}

/** @brief Create standard log message with one argument.
 */
static inline struct log_msg *log_msg_create_1(const char *str,
					       u32_t arg1)
{
	struct  log_msg *msg = z_log_msg_std_alloc();

	if (msg != NULL) {
		msg->str = str;
		msg->hdr.params.std.nargs = 1U;
		msg->payload.single.args[0] = arg1;
	}

	return msg;
}

/** @brief Create standard log message with two arguments.
 */
static inline struct log_msg *log_msg_create_2(const char *str,
					       u32_t arg1,
					       u32_t arg2)
{
	struct  log_msg *msg = z_log_msg_std_alloc();

	if (msg != NULL) {
		msg->str = str;
		msg->hdr.params.std.nargs = 2U;
		msg->payload.single.args[0] = arg1;
		msg->payload.single.args[1] = arg2;
	}

	return msg;
}

/** @brief Create standard log message with three arguments.
 */
static inline struct log_msg *log_msg_create_3(const char *str,
					       u32_t arg1,
					       u32_t arg2,
					       u32_t arg3)
{
	struct  log_msg *msg = z_log_msg_std_alloc();

	if (msg != NULL) {
		msg->str = str;
		msg->hdr.params.std.nargs = 3U;
		msg->payload.single.args[0] = arg1;
		msg->payload.single.args[1] = arg2;
		msg->payload.single.args[2] = arg3;
	}

	return msg;
}

/** @brief Create standard log message with variable number of arguments.
 */
struct log_msg *log_msg_create_n(const char *str,
				 u32_t *args,
				 u32_t nargs);

#ifdef __cplusplus
}
#endif

#endif /* WATCH_LOG_MSG_H_ */


#ifndef WATCH_LOG_INSTANCE_H_
#define WATCH_LOG_INSTANCE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Constant data associated with the source of log messages. */
struct log_source_const_data {
	const char *name;
	u8_t level;
#ifdef CONFIG_NIOS2
	/* Workaround alert! Dummy data to ensure that structure is >8 bytes.
	 * Nios2 uses global pointer register for structures <=8 bytes and
	 * apparently does not handle well variables placed in custom sections.
	 */
	u32_t dummy;
#endif
};

/** @brief Dynamic data associated with the source of log messages. */
struct log_source_dynamic_data {
	u32_t filters;
#ifdef CONFIG_NIOS2
	/* Workaround alert! Dummy data to ensure that structure is >8 bytes.
	 * Nios2 uses global pointer register for structures <=8 bytes and
	 * apparently does not handle well variables placed in custom sections.
	 */
	u32_t dummy[2];
#endif
};

/** @brief Creates name of variable and section for constant log data.
 */
#define LOG_ITEM_CONST_DATA(_name) UTIL_CAT(log_const_, _name)

#define Z_LOG_CONST_ITEM_REGISTER(_name, _str_name, _level)		     \
	const struct log_source_const_data LOG_ITEM_CONST_DATA(_name)	     \
	__attribute__ ((section("." STRINGIFY(LOG_ITEM_CONST_DATA(_name))))) \
	__attribute__((used)) = {					     \
		.name = _str_name,					     \
		.level  = (_level),					     \
	}

#ifdef CONFIG_LOG

#define LOG_INSTANCE_FULL_NAME(_module_name, _inst_name) \
	UTIL_CAT(_module_name, UTIL_CAT(_, _inst_name))

#if CONFIG_LOG_RUNTIME_FILTERING
#define LOG_INSTANCE_PTR_DECLARE(_name)	\
	struct log_source_dynamic_data *_name

#define LOG_INSTANCE_REGISTER(_module_name, _inst_name, _level)		   \
	Z_LOG_CONST_ITEM_REGISTER(					   \
		LOG_INSTANCE_FULL_NAME(_module_name, _inst_name),	   \
		STRINGIFY(_module_name._inst_name),			   \
		_level);						   \
	struct log_source_dynamic_data LOG_INSTANCE_DYNAMIC_DATA(	   \
						_module_name, _inst_name)  \
		__attribute__ ((section("." STRINGIFY(			   \
				LOG_INSTANCE_DYNAMIC_DATA(_module_name,	   \
						       _inst_name)	   \
				)					   \
		))) __attribute__((used))

#define LOG_INSTANCE_PTR_INIT(_name, _module_name, _inst_name)	   \
	._name = &LOG_ITEM_DYNAMIC_DATA(			   \
		LOG_INSTANCE_FULL_NAME(_module_name, _inst_name)),

#else /* CONFIG_LOG_RUNTIME_FILTERING */
#define LOG_INSTANCE_PTR_DECLARE(_name)	\
	const struct log_source_const_data *_name

#define LOG_INSTANCE_REGISTER(_module_name, _inst_name, _level)	  \
	Z_LOG_CONST_ITEM_REGISTER(				  \
		LOG_INSTANCE_FULL_NAME(_module_name, _inst_name), \
		STRINGIFY(_module_name._inst_name),		  \
		_level)


#define LOG_INSTANCE_PTR_INIT(_name, _module_name, _inst_name) \
	._name = &LOG_ITEM_CONST_DATA(			       \
		LOG_INSTANCE_FULL_NAME(_module_name, _inst_name)),

#endif /* CONFIG_LOG_RUNTIME_FILTERING */
#else /* CONFIG_LOG */
#define LOG_INSTANCE_PTR_DECLARE(_name) /* empty */
#define LOG_INSTANCE_REGISTER(_module_name, _inst_name,  _level)    /* empty */
#define LOG_INSTANCE_PTR_INIT(_name, _module_name, _inst_name)      /* empty */
#endif

#ifdef __cplusplus
}
#endif

#endif /* WATCH_LOG_INSTANCE_H_ */


#ifndef WATCH_MISC_UTIL_H_
#define WATCH_MISC_UTIL_H_

#ifndef _ASMLANGUAGE

#define POINTER_TO_UINT(x) ((u32_t) (x))
#define UINT_TO_POINTER(x) ((void *) (x))
#define POINTER_TO_INT(x)  ((s32_t) (x))
#define INT_TO_POINTER(x)  ((void *) (x))

#if !(defined (__CHAR_BIT__) && defined (__SIZEOF_LONG__))
#	error Missing required predefined macros for BITS_PER_LONG calculation
#endif

#define BITS_PER_LONG	(__CHAR_BIT__ * __SIZEOF_LONG__)
/* Create a contiguous bitmask starting at bit position @l and ending at
 * position @h.
 */
#define GENMASK(h, l) \
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

/* Evaluates to 0 if cond is true-ish; compile error otherwise */
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

/* Evaluates to 0 if array is an array; compile error if not array (e.g.
 * pointer)
 */
#define IS_ARRAY(array) \
	ZERO_OR_COMPILE_ERROR( \
		!__builtin_types_compatible_p(__typeof__(array), \
					      __typeof__(&(array)[0])))

#if defined(__cplusplus)
template < class T, size_t N >
constexpr size_t ARRAY_SIZE(T(&)[N]) { return N; }

#else
/* Evaluates to number of elements in an array; compile error if not
 * an array (e.g. pointer)
 */
#define ARRAY_SIZE(array) \
	((long) (IS_ARRAY(array) + (sizeof(array) / sizeof((array)[0]))))
#endif

/* Evaluates to 1 if ptr is part of array, 0 otherwise; compile error if
 * "array" argument is not an array (e.g. "ptr" and "array" mixed up)
 */
#define PART_OF_ARRAY(array, ptr) \
	((ptr) && ((ptr) >= &array[0] && (ptr) < &array[ARRAY_SIZE(array)]))

#define CONTAINER_OF(ptr, type, field) \
	((type *)(((char *)(ptr)) - offsetof(type, field)))

/* round "x" up/down to next multiple of "align" (which must be a power of 2) */
#define ROUND_UP(x, align)                                   \
	(((unsigned long)(x) + ((unsigned long)(align) - 1)) & \
	 ~((unsigned long)(align) - 1))
#define ROUND_DOWN(x, align)                                 \
	((unsigned long)(x) & ~((unsigned long)(align) - 1))

#define ceiling_fraction(numerator, divider) \
	(((numerator) + ((divider) - 1)) / (divider))

#ifdef INLINED
#define INLINE inline
#else
#define INLINE
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

static inline bool is_power_of_two(unsigned int x)
{
	return (x != 0U) && ((x & (x - 1)) == 0U);
}

static inline s64_t arithmetic_shift_right(s64_t value, u8_t shift)
{
	s64_t sign_ext;

	if (shift == 0U) {
		return value;
	}

	/* extract sign bit */
	sign_ext = (value >> 63) & 1;

	/* make all bits of sign_ext be the same as the value's sign bit */
	sign_ext = -sign_ext;

	/* shift value and fill opened bit positions with sign bit */
	return (value >> shift) | (sign_ext << (64 - shift));
}

#endif /* !_ASMLANGUAGE */

/* KB, MB, GB */
#define KB(x) ((x) << 10)
#define MB(x) (KB(x) << 10)
#define GB(x) (MB(x) << 10)

/* KHZ, MHZ */
#define KHZ(x) ((x) * 1000)
#define MHZ(x) (KHZ(x) * 1000)

#ifndef BIT
#if defined(_ASMLANGUAGE)
#define BIT(n)  (1 << (n))
#else
#define BIT(n)  (1UL << (n))
#endif
#endif

#define WRITE_BIT(var, bit, set) \
	((var) = (set) ? ((var) | BIT(bit)) : ((var) & ~BIT(bit)))

#define BIT_MASK(n) (BIT(n) - 1)

#define IS_ENABLED(config_macro) Z_IS_ENABLED1(config_macro)

#define Z_IS_ENABLED1(config_macro) Z_IS_ENABLED2(_XXXX##config_macro)

#define _XXXX1 _YYYY,

#define Z_IS_ENABLED2(one_or_two_args) Z_IS_ENABLED3(one_or_two_args true, false)

#define Z_IS_ENABLED3(ignore_this, val, ...) val

#define COND_CODE_1(_flag, _if_1_code, _else_code) \
	Z_COND_CODE_1(_flag, _if_1_code, _else_code)

#define Z_COND_CODE_1(_flag, _if_1_code, _else_code) \
	__COND_CODE(_XXXX##_flag, _if_1_code, _else_code)

#define COND_CODE_0(_flag, _if_0_code, _else_code) \
	Z_COND_CODE_0(_flag, _if_0_code, _else_code)

#define Z_COND_CODE_0(_flag, _if_0_code, _else_code) \
	__COND_CODE(_ZZZZ##_flag, _if_0_code, _else_code)

#define _ZZZZ0 _YYYY,

/* Macro used internally by @ref COND_CODE_1 and @ref COND_CODE_0. */
#define __COND_CODE(one_or_two_args, _if_code, _else_code) \
	__GET_ARG2_DEBRACKET(one_or_two_args _if_code, _else_code)

/* Macro used internally to remove brackets from argument. */
#define __DEBRACKET(...) __VA_ARGS__

#define __GET_ARG2_DEBRACKET(ignore_this, val, ...) __DEBRACKET val

/**
 * @brief Get first argument from variable list of arguments
 */
#define GET_ARG1(arg1, ...) arg1


#define GET_ARG2(arg1, arg2, ...) arg2


#define GET_ARGS_LESS_1(val, ...) __VA_ARGS__

#define UTIL_EMPTY(...)
#define UTIL_DEFER(...) __VA_ARGS__ UTIL_EMPTY()
#define UTIL_OBSTRUCT(...) __VA_ARGS__ UTIL_DEFER(UTIL_EMPTY)()
#define UTIL_EXPAND(...) __VA_ARGS__

#define UTIL_EVAL(...)  UTIL_EVAL1(UTIL_EVAL1(UTIL_EVAL1(__VA_ARGS__)))
#define UTIL_EVAL1(...) UTIL_EVAL2(UTIL_EVAL2(UTIL_EVAL2(__VA_ARGS__)))
#define UTIL_EVAL2(...) UTIL_EVAL3(UTIL_EVAL3(UTIL_EVAL3(__VA_ARGS__)))
#define UTIL_EVAL3(...) UTIL_EVAL4(UTIL_EVAL4(UTIL_EVAL4(__VA_ARGS__)))
#define UTIL_EVAL4(...) UTIL_EVAL5(UTIL_EVAL5(UTIL_EVAL5(__VA_ARGS__)))
#define UTIL_EVAL5(...) __VA_ARGS__

#define UTIL_CAT(a, ...) UTIL_PRIMITIVE_CAT(a, __VA_ARGS__)
#define UTIL_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__

#define UTIL_INC(x) UTIL_PRIMITIVE_CAT(UTIL_INC_, x)
#define UTIL_INC_0 1
#define UTIL_INC_1 2
#define UTIL_INC_2 3
#define UTIL_INC_3 4
#define UTIL_INC_4 5
#define UTIL_INC_5 6
#define UTIL_INC_6 7
#define UTIL_INC_7 8
#define UTIL_INC_8 9
#define UTIL_INC_9 10
#define UTIL_INC_10 11
#define UTIL_INC_11 12
#define UTIL_INC_12 13
#define UTIL_INC_13 14
#define UTIL_INC_14 15
#define UTIL_INC_15 16
#define UTIL_INC_16 17
#define UTIL_INC_17 18
#define UTIL_INC_18 19
#define UTIL_INC_19 19

#define UTIL_DEC(x) UTIL_PRIMITIVE_CAT(UTIL_DEC_, x)
#define UTIL_DEC_0 0
#define UTIL_DEC_1 0
#define UTIL_DEC_2 1
#define UTIL_DEC_3 2
#define UTIL_DEC_4 3
#define UTIL_DEC_5 4
#define UTIL_DEC_6 5
#define UTIL_DEC_7 6
#define UTIL_DEC_8 7
#define UTIL_DEC_9 8
#define UTIL_DEC_10 9
#define UTIL_DEC_11 10
#define UTIL_DEC_12 11
#define UTIL_DEC_13 12
#define UTIL_DEC_14 13
#define UTIL_DEC_15 14
#define UTIL_DEC_16 15
#define UTIL_DEC_17 16
#define UTIL_DEC_18 17
#define UTIL_DEC_19 18
#define UTIL_DEC_20 19
#define UTIL_DEC_21 20
#define UTIL_DEC_22 21
#define UTIL_DEC_23 22
#define UTIL_DEC_24 23
#define UTIL_DEC_25 24
#define UTIL_DEC_26 25
#define UTIL_DEC_27 26
#define UTIL_DEC_28 27
#define UTIL_DEC_29 28
#define UTIL_DEC_30 29
#define UTIL_DEC_31 30
#define UTIL_DEC_32 31
#define UTIL_DEC_33 32
#define UTIL_DEC_34 33
#define UTIL_DEC_35 34
#define UTIL_DEC_36 35
#define UTIL_DEC_37 36
#define UTIL_DEC_38 37
#define UTIL_DEC_39 38
#define UTIL_DEC_40 39
#define UTIL_DEC_41 40
#define UTIL_DEC_42 41
#define UTIL_DEC_43 42
#define UTIL_DEC_44 43
#define UTIL_DEC_45 44
#define UTIL_DEC_46 45
#define UTIL_DEC_47 46
#define UTIL_DEC_48 47
#define UTIL_DEC_49 48
#define UTIL_DEC_50 49
#define UTIL_DEC_51 50
#define UTIL_DEC_52 51
#define UTIL_DEC_53 52
#define UTIL_DEC_54 53
#define UTIL_DEC_55 54
#define UTIL_DEC_56 55
#define UTIL_DEC_57 56
#define UTIL_DEC_58 57
#define UTIL_DEC_59 58
#define UTIL_DEC_60 59
#define UTIL_DEC_61 60
#define UTIL_DEC_62 61
#define UTIL_DEC_63 62
#define UTIL_DEC_64 63
#define UTIL_DEC_65 64
#define UTIL_DEC_66 65
#define UTIL_DEC_67 66
#define UTIL_DEC_68 67
#define UTIL_DEC_69 68
#define UTIL_DEC_70 69
#define UTIL_DEC_71 70
#define UTIL_DEC_72 71
#define UTIL_DEC_73 72
#define UTIL_DEC_74 73
#define UTIL_DEC_75 74
#define UTIL_DEC_76 75
#define UTIL_DEC_77 76
#define UTIL_DEC_78 77
#define UTIL_DEC_79 78
#define UTIL_DEC_80 79
#define UTIL_DEC_81 80
#define UTIL_DEC_82 81
#define UTIL_DEC_83 82
#define UTIL_DEC_84 83
#define UTIL_DEC_85 84
#define UTIL_DEC_86 85
#define UTIL_DEC_87 86
#define UTIL_DEC_88 87
#define UTIL_DEC_89 88
#define UTIL_DEC_90 89
#define UTIL_DEC_91 90
#define UTIL_DEC_92 91
#define UTIL_DEC_93 92
#define UTIL_DEC_94 93
#define UTIL_DEC_95 94
#define UTIL_DEC_96 95
#define UTIL_DEC_97 96
#define UTIL_DEC_98 97
#define UTIL_DEC_99 98
#define UTIL_DEC_100 99
#define UTIL_DEC_101 100
#define UTIL_DEC_102 101
#define UTIL_DEC_103 102
#define UTIL_DEC_104 103
#define UTIL_DEC_105 104
#define UTIL_DEC_106 105
#define UTIL_DEC_107 106
#define UTIL_DEC_108 107
#define UTIL_DEC_109 108
#define UTIL_DEC_110 109
#define UTIL_DEC_111 110
#define UTIL_DEC_112 111
#define UTIL_DEC_113 112
#define UTIL_DEC_114 113
#define UTIL_DEC_115 114
#define UTIL_DEC_116 115
#define UTIL_DEC_117 116
#define UTIL_DEC_118 117
#define UTIL_DEC_119 118
#define UTIL_DEC_120 119
#define UTIL_DEC_121 120
#define UTIL_DEC_122 121
#define UTIL_DEC_123 122
#define UTIL_DEC_124 123
#define UTIL_DEC_125 124
#define UTIL_DEC_126 125
#define UTIL_DEC_127 126
#define UTIL_DEC_128 127
#define UTIL_DEC_129 128
#define UTIL_DEC_130 129
#define UTIL_DEC_131 130
#define UTIL_DEC_132 131
#define UTIL_DEC_133 132
#define UTIL_DEC_134 133
#define UTIL_DEC_135 134
#define UTIL_DEC_136 135
#define UTIL_DEC_137 136
#define UTIL_DEC_138 137
#define UTIL_DEC_139 138
#define UTIL_DEC_140 139
#define UTIL_DEC_141 140
#define UTIL_DEC_142 141
#define UTIL_DEC_143 142
#define UTIL_DEC_144 143
#define UTIL_DEC_145 144
#define UTIL_DEC_146 145
#define UTIL_DEC_147 146
#define UTIL_DEC_148 147
#define UTIL_DEC_149 148
#define UTIL_DEC_150 149
#define UTIL_DEC_151 150
#define UTIL_DEC_152 151
#define UTIL_DEC_153 152
#define UTIL_DEC_154 153
#define UTIL_DEC_155 154
#define UTIL_DEC_156 155
#define UTIL_DEC_157 156
#define UTIL_DEC_158 157
#define UTIL_DEC_159 158
#define UTIL_DEC_160 159
#define UTIL_DEC_161 160
#define UTIL_DEC_162 161
#define UTIL_DEC_163 162
#define UTIL_DEC_164 163
#define UTIL_DEC_165 164
#define UTIL_DEC_166 165
#define UTIL_DEC_167 166
#define UTIL_DEC_168 167
#define UTIL_DEC_169 168
#define UTIL_DEC_170 169
#define UTIL_DEC_171 170
#define UTIL_DEC_172 171
#define UTIL_DEC_173 172
#define UTIL_DEC_174 173
#define UTIL_DEC_175 174
#define UTIL_DEC_176 175
#define UTIL_DEC_177 176
#define UTIL_DEC_178 177
#define UTIL_DEC_179 178
#define UTIL_DEC_180 179
#define UTIL_DEC_181 180
#define UTIL_DEC_182 181
#define UTIL_DEC_183 182
#define UTIL_DEC_184 183
#define UTIL_DEC_185 184
#define UTIL_DEC_186 185
#define UTIL_DEC_187 186
#define UTIL_DEC_188 187
#define UTIL_DEC_189 188
#define UTIL_DEC_190 189
#define UTIL_DEC_191 190
#define UTIL_DEC_192 191
#define UTIL_DEC_193 192
#define UTIL_DEC_194 193
#define UTIL_DEC_195 194
#define UTIL_DEC_196 195
#define UTIL_DEC_197 196
#define UTIL_DEC_198 197
#define UTIL_DEC_199 198
#define UTIL_DEC_200 199
#define UTIL_DEC_201 200
#define UTIL_DEC_202 201
#define UTIL_DEC_203 202
#define UTIL_DEC_204 203
#define UTIL_DEC_205 204
#define UTIL_DEC_206 205
#define UTIL_DEC_207 206
#define UTIL_DEC_208 207
#define UTIL_DEC_209 208
#define UTIL_DEC_210 209
#define UTIL_DEC_211 210
#define UTIL_DEC_212 211
#define UTIL_DEC_213 212
#define UTIL_DEC_214 213
#define UTIL_DEC_215 214
#define UTIL_DEC_216 215
#define UTIL_DEC_217 216
#define UTIL_DEC_218 217
#define UTIL_DEC_219 218
#define UTIL_DEC_220 219
#define UTIL_DEC_221 220
#define UTIL_DEC_222 221
#define UTIL_DEC_223 222
#define UTIL_DEC_224 223
#define UTIL_DEC_225 224
#define UTIL_DEC_226 225
#define UTIL_DEC_227 226
#define UTIL_DEC_228 227
#define UTIL_DEC_229 228
#define UTIL_DEC_230 229
#define UTIL_DEC_231 230
#define UTIL_DEC_232 231
#define UTIL_DEC_233 232
#define UTIL_DEC_234 233
#define UTIL_DEC_235 234
#define UTIL_DEC_236 235
#define UTIL_DEC_237 236
#define UTIL_DEC_238 237
#define UTIL_DEC_239 238
#define UTIL_DEC_240 239
#define UTIL_DEC_241 240
#define UTIL_DEC_242 241
#define UTIL_DEC_243 242
#define UTIL_DEC_244 243
#define UTIL_DEC_245 244
#define UTIL_DEC_246 245
#define UTIL_DEC_247 246
#define UTIL_DEC_248 247
#define UTIL_DEC_249 248
#define UTIL_DEC_250 249
#define UTIL_DEC_251 250
#define UTIL_DEC_252 251
#define UTIL_DEC_253 252
#define UTIL_DEC_254 253
#define UTIL_DEC_255 254
#define UTIL_DEC_256 255

#define UTIL_CHECK_N(x, n, ...) n
#define UTIL_CHECK(...) UTIL_CHECK_N(__VA_ARGS__, 0,)

#define UTIL_NOT(x) UTIL_CHECK(UTIL_PRIMITIVE_CAT(UTIL_NOT_, x))
#define UTIL_NOT_0 ~, 1,

#define UTIL_COMPL(b) UTIL_PRIMITIVE_CAT(UTIL_COMPL_, b)
#define UTIL_COMPL_0 1
#define UTIL_COMPL_1 0

#define UTIL_BOOL(x) UTIL_COMPL(UTIL_NOT(x))

#define UTIL_IIF(c) UTIL_PRIMITIVE_CAT(UTIL_IIF_, c)
#define UTIL_IIF_0(t, ...) __VA_ARGS__
#define UTIL_IIF_1(t, ...) t

#define UTIL_IF(c) UTIL_IIF(UTIL_BOOL(c))

#define UTIL_EAT(...)
#define UTIL_EXPAND(...) __VA_ARGS__
#define UTIL_WHEN(c) UTIL_IF(c)(UTIL_EXPAND, UTIL_EAT)

#define UTIL_REPEAT(count, macro, ...)			    \
	UTIL_WHEN(count)				    \
	(						    \
		UTIL_OBSTRUCT(UTIL_REPEAT_INDIRECT) ()	    \
		(					    \
			UTIL_DEC(count), macro, __VA_ARGS__ \
		)					    \
		UTIL_OBSTRUCT(macro)			    \
		(					    \
			UTIL_DEC(count), __VA_ARGS__	    \
		)					    \
	)
#define UTIL_REPEAT_INDIRECT() UTIL_REPEAT


#define UTIL_LISTIFY(LEN, F, F_ARG) UTIL_EVAL(UTIL_REPEAT(LEN, F, F_ARG))

/**@brief Implementation details for NUM_VAR_ARGS */
#define NUM_VA_ARGS_LESS_1_IMPL(				\
	_ignored,						\
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,		\
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20,	\
	_21, _22, _23, _24, _25, _26, _27, _28, _29, _30,	\
	_31, _32, _33, _34, _35, _36, _37, _38, _39, _40,	\
	_41, _42, _43, _44, _45, _46, _47, _48, _49, _50,	\
	_51, _52, _53, _54, _55, _56, _57, _58, _59, _60,	\
	_61, _62, N, ...) N


#define NUM_VA_ARGS_LESS_1(...) \
	NUM_VA_ARGS_LESS_1_IMPL(__VA_ARGS__, 63, 62, 61, \
	60, 59, 58, 57, 56, 55, 54, 53, 52, 51,		 \
	50, 49, 48, 47, 46, 45, 44, 43, 42, 41,		 \
	40, 39, 38, 37, 36, 35, 34, 33, 32, 31,		 \
	30, 29, 28, 27, 26, 25, 24, 23, 22, 21,		 \
	20, 19, 18, 17, 16, 15, 14, 13, 12, 11,		 \
	10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, ~)


#define MACRO_MAP(...) MACRO_MAP_(__VA_ARGS__)
#define MACRO_MAP_(...)							\
	/* To make sure it works also for 2 arguments in total */	\
	MACRO_MAP_N(NUM_VA_ARGS_LESS_1(__VA_ARGS__), __VA_ARGS__)

#define MACRO_MAP_N(N, ...) MACRO_MAP_N_(N, __VA_ARGS__)
#define MACRO_MAP_N_(N, ...) UTIL_CAT(MACRO_MAP_, N)(__VA_ARGS__,)

#define MACRO_MAP_0(...)
#define MACRO_MAP_1(macro, a, ...)  macro(a)
#define MACRO_MAP_2(macro, a, ...)  macro(a)MACRO_MAP_1(macro, __VA_ARGS__,)
#define MACRO_MAP_3(macro, a, ...)  macro(a)MACRO_MAP_2(macro, __VA_ARGS__,)
#define MACRO_MAP_4(macro, a, ...)  macro(a)MACRO_MAP_3(macro, __VA_ARGS__,)
#define MACRO_MAP_5(macro, a, ...)  macro(a)MACRO_MAP_4(macro, __VA_ARGS__,)
#define MACRO_MAP_6(macro, a, ...)  macro(a)MACRO_MAP_5(macro, __VA_ARGS__,)
#define MACRO_MAP_7(macro, a, ...)  macro(a)MACRO_MAP_6(macro, __VA_ARGS__,)
#define MACRO_MAP_8(macro, a, ...)  macro(a)MACRO_MAP_7(macro, __VA_ARGS__,)
#define MACRO_MAP_9(macro, a, ...)  macro(a)MACRO_MAP_8(macro, __VA_ARGS__,)
#define MACRO_MAP_10(macro, a, ...) macro(a)MACRO_MAP_9(macro, __VA_ARGS__,)
#define MACRO_MAP_11(macro, a, ...) macro(a)MACRO_MAP_10(macro, __VA_ARGS__,)
#define MACRO_MAP_12(macro, a, ...) macro(a)MACRO_MAP_11(macro, __VA_ARGS__,)
#define MACRO_MAP_13(macro, a, ...) macro(a)MACRO_MAP_12(macro, __VA_ARGS__,)
#define MACRO_MAP_14(macro, a, ...) macro(a)MACRO_MAP_13(macro, __VA_ARGS__,)
#define MACRO_MAP_15(macro, a, ...) macro(a)MACRO_MAP_14(macro, __VA_ARGS__,)

#define Z_GET_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

#define _for_0(z_call, ...)
#define _for_1(z_call, x) z_call(x)
#define _for_2(z_call, x, ...) z_call(x) _for_1(z_call, ##__VA_ARGS__)
#define _for_3(z_call, x, ...) z_call(x) _for_2(z_call, ##__VA_ARGS__)
#define _for_4(z_call, x, ...) z_call(x) _for_3(z_call, ##__VA_ARGS__)
#define _for_5(z_call, x, ...) z_call(x) _for_4(z_call, ##__VA_ARGS__)
#define _for_6(z_call, x, ...) z_call(x) _for_5(z_call, ##__VA_ARGS__)
#define _for_7(z_call, x, ...) z_call(x) _for_6(z_call, ##__VA_ARGS__)
#define _for_8(z_call, x, ...) z_call(x) _for_7(z_call, ##__VA_ARGS__)
#define _for_9(z_call, x, ...) z_call(x) _for_8(z_call, ##__VA_ARGS__)
#define _for_10(z_call, x, ...) z_call(x) _for_9(z_call, ##__VA_ARGS__)

#define FOR_EACH(x, ...) \
	Z_GET_ARG(__VA_ARGS__, \
	_for_10, _for_9, _for_8, _for_7, _for_6, _for_5, \
	_for_4, _for_3, _for_2, _for_1, _for_0)(x, ##__VA_ARGS__)

#define z_rep_0(_fn, f, ...)
#define z_rep_1(_fn, f, x) {_fn(x, f); z_rep_0(_fn, f)}
#define z_rep_2(_fn, f, x, ...) {_fn(x, f); z_rep_1(_fn, f, ##__VA_ARGS__)}
#define z_rep_3(_fn, f, x, ...) {_fn(x, f); z_rep_2(_fn, f, ##__VA_ARGS__)}
#define z_rep_4(_fn, f, x, ...) {_fn(x, f); z_rep_3(_fn, f, ##__VA_ARGS__)}
#define z_rep_5(_fn, f, x, ...) {_fn(x, f); z_rep_4(_fn, f, ##__VA_ARGS__)}
#define z_rep_6(_fn, f, x, ...) {_fn(x, f); z_rep_5(_fn, f, ##__VA_ARGS__)}
#define z_rep_7(_fn, f, x, ...) {_fn(x, f); z_rep_6(_fn, f, ##__VA_ARGS__)}
#define z_rep_8(_fn, f, x, ...) {_fn(x, f); z_rep_7(_fn, f, ##__VA_ARGS__)}
#define z_rep_9(_fn, f, x, ...) {_fn(x, f); z_rep_8(_fn, f, ##__VA_ARGS__)}
#define z_rep_10(_fn, f, x, ...) {_fn(x, f); z_rep_9(_fn, f, ##__VA_ARGS__)}
#define z_rep_11(_fn, f, x, ...) {_fn(x, f); z_rep_10(_fn, f, ##__VA_ARGS__)}
#define z_rep_12(_fn, f, x, ...) {_fn(x, f); z_rep_11(_fn, f, ##__VA_ARGS__)}
#define z_rep_13(_fn, f, x, ...) {_fn(x, f); z_rep_12(_fn, f, ##__VA_ARGS__)}
#define z_rep_14(_fn, f, x, ...) {_fn(x, f); z_rep_13(_fn, f, ##__VA_ARGS__)}
#define z_rep_15(_fn, f, x, ...) {_fn(x, f); z_rep_14(_fn, f, ##__VA_ARGS__)}
#define z_rep_16(_fn, f, x, ...) {_fn(x, f); z_rep_15(_fn, f, ##__VA_ARGS__)}
#define z_rep_17(_fn, f, x, ...) {_fn(x, f); z_rep_16(_fn, f, ##__VA_ARGS__)}
#define z_rep_18(_fn, f, x, ...) {_fn(x, f); z_rep_17(_fn, f, ##__VA_ARGS__)}
#define z_rep_19(_fn, f, x, ...) {_fn(x, f); z_rep_18(_fn, f, ##__VA_ARGS__)}
#define z_rep_20(_fn, f, x, ...) {_fn(x, f); z_rep_19(_fn, f, ##__VA_ARGS__)}


#define Z_GET_ARG_2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
		   _14, _15, _16, _17, _18, _19, _20, N, ...) N

#define FOR_EACH_FIXED_ARG(fixed_arg, x, ...) \
	{Z_GET_ARG_2(__VA_ARGS__,				\
		     z_rep_20, z_rep_19, z_rep_18, z_rep_17, z_rep_16,	\
		     z_rep_15, z_rep_14, z_rep_13, z_rep_12, z_rep_11,	\
		     z_rep_10, z_rep_9, z_rep_8, z_rep_7, z_rep_6,	\
		     z_rep_5, z_rep_4, z_rep_3, z_rep_2, z_rep_1, z_rep_0) \
	 (fixed_arg, x, ##__VA_ARGS__)}

#endif /* WATCH_MISC_UTIL_H_ */


#ifdef __cplusplus
extern "C" {
#endif

#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFUL
#error "Logger does not support 64 bit architecture."
#endif

#if !CONFIG_LOG
#define CONFIG_LOG_DEFAULT_LEVEL 0
#define CONFIG_LOG_DOMAIN_ID 0
#define CONFIG_LOG_MAX_LEVEL 0
#endif

#define LOG_FUNCTION_PREFIX_MASK \
	(((u32_t)IS_ENABLED(CONFIG_LOG_FUNC_NAME_PREFIX_ERR) << \
	  LOG_LEVEL_ERR) | \
	 ((u32_t)IS_ENABLED(CONFIG_LOG_FUNC_NAME_PREFIX_WRN) << \
	  LOG_LEVEL_WRN) | \
	 ((u32_t)IS_ENABLED(CONFIG_LOG_FUNC_NAME_PREFIX_INF) << \
	  LOG_LEVEL_INF) | \
	 ((u32_t)IS_ENABLED(CONFIG_LOG_FUNC_NAME_PREFIX_DBG) << LOG_LEVEL_DBG))


#define Z_LOG_RESOLVED_LEVEL(_level, _default) \
	Z_LOG_RESOLVED_LEVEL1(_level, _default)

#define Z_LOG_RESOLVED_LEVEL1(_level, _default) \
	__COND_CODE(_LOG_XXXX##_level, (_level), (_default))

#define _LOG_XXXX0 _LOG_YYYY,
#define _LOG_XXXX1 _LOG_YYYY,
#define _LOG_XXXX2 _LOG_YYYY,
#define _LOG_XXXX3 _LOG_YYYY,
#define _LOG_XXXX4 _LOG_YYYY,

#define Z_LOG_EVAL(_eval_level, _iftrue, _iffalse) \
	Z_LOG_EVAL1(_eval_level, _iftrue, _iffalse)

#define Z_LOG_EVAL1(_eval_level, _iftrue, _iffalse) \
	__COND_CODE(_LOG_ZZZZ##_eval_level, _iftrue, _iffalse)

#define _LOG_ZZZZ1 _LOG_YYYY,
#define _LOG_ZZZZ2 _LOG_YYYY,
#define _LOG_ZZZZ3 _LOG_YYYY,
#define _LOG_ZZZZ4 _LOG_YYYY,


#define _LOG_LEVEL() Z_LOG_RESOLVED_LEVEL(LOG_LEVEL, CONFIG_LOG_DEFAULT_LEVEL)

#define LOG_CONST_ID_GET(_addr) \
	Z_LOG_EVAL(\
	  CONFIG_LOG,\
	  (__log_level ? \
	  log_const_source_id((const struct log_source_const_data *)_addr) : \
	  0),\
	  (0)\
	)

#define LOG_CURRENT_MODULE_ID() (__log_level != 0 ? \
	log_const_source_id(__log_current_const_data) : 0)

#define LOG_CURRENT_DYNAMIC_DATA_ADDR()	(__log_level ? \
	__log_current_dynamic_data : (struct log_source_dynamic_data *)0)

#define LOG_DYNAMIC_ID_GET(_addr) \
	Z_LOG_EVAL(\
	  CONFIG_LOG,\
	  (__log_level ? \
	  log_dynamic_source_id((struct log_source_dynamic_data *)_addr) : 0),\
	  (0)\
	)

#define Z_LOG_STR(...) "%s: " GET_ARG1(__VA_ARGS__), __func__\
		COND_CODE_0(NUM_VA_ARGS_LESS_1(__VA_ARGS__),\
			    (),\
			    (, GET_ARGS_LESS_1(__VA_ARGS__))\
			   )


#define _LOG_NARGS_POSTFIX_IMPL(				\
	_ignored,						\
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,		\
	_11, _12, _13, _14, N, ...) N

#define Z_LOG_NARGS_POSTFIX(...) \
	_LOG_NARGS_POSTFIX_IMPL(__VA_ARGS__, LONG, LONG, LONG, LONG, LONG, \
			LONG, LONG, LONG, LONG, LONG, LONG, LONG, 3, 2, 1, 0, ~)

#define Z_LOG_INTERNAL_X(N, ...)  UTIL_CAT(_LOG_INTERNAL_, N)(__VA_ARGS__)

#define __LOG_INTERNAL(_src_level, ...)					 \
	do {								 \
		if (IS_ENABLED(CONFIG_LOG_IMMEDIATE)) {		 \
			log_string_sync(_src_level, __VA_ARGS__);	 \
		} else {						 \
			Z_LOG_INTERNAL_X(Z_LOG_NARGS_POSTFIX(__VA_ARGS__), \
						_src_level, __VA_ARGS__);\
		}							 \
	} while (false)

#define _LOG_INTERNAL_0(_src_level, _str) \
	log_0(_str, _src_level)

#define _LOG_INTERNAL_1(_src_level, _str, _arg0) \
	log_1(_str, (u32_t)(_arg0), _src_level)

#define _LOG_INTERNAL_2(_src_level, _str, _arg0, _arg1)	\
	log_2(_str, (u32_t)(_arg0), (u32_t)(_arg1), _src_level)

#define _LOG_INTERNAL_3(_src_level, _str, _arg0, _arg1, _arg2) \
	log_3(_str, (u32_t)(_arg0), (u32_t)(_arg1), (u32_t)(_arg2), _src_level)

#define __LOG_ARG_CAST(_x) (u32_t)(_x),

#define __LOG_ARGUMENTS(...) MACRO_MAP(__LOG_ARG_CAST, __VA_ARGS__)

#define _LOG_INTERNAL_LONG(_src_level, _str, ...)		 \
	do {							 \
		u32_t args[] = {__LOG_ARGUMENTS(__VA_ARGS__)};	 \
		log_n(_str, args, ARRAY_SIZE(args), _src_level); \
	} while (false)

#define Z_LOG_LEVEL_CHECK(_level, _check_level, _default_level) \
	(_level <= Z_LOG_RESOLVED_LEVEL(_check_level, _default_level))

#define Z_LOG_CONST_LEVEL_CHECK(_level)					    \
	(IS_ENABLED(CONFIG_LOG) &&					    \
	(Z_LOG_LEVEL_CHECK(_level, CONFIG_LOG_OVERRIDE_LEVEL, LOG_LEVEL_NONE) \
	||								    \
	((IS_ENABLED(CONFIG_LOG_OVERRIDE_LEVEL) == false) &&		    \
	(_level <= __log_level) &&					    \
	(_level <= CONFIG_LOG_MAX_LEVEL)				    \
	)								    \

#define __LOG(_level, _id, _filter, ...)				    \
	do {								    \
		if (Z_LOG_CONST_LEVEL_CHECK(_level) &&			    \
		    (_level <= LOG_RUNTIME_FILTER(_filter))) {		    \
			struct log_msg_ids src_level = {		    \
				.level = _level,			    \
				.domain_id = CONFIG_LOG_DOMAIN_ID,	    \
				.source_id = _id			    \
			};						    \
									    \
			if ((BIT(_level) & LOG_FUNCTION_PREFIX_MASK) != 0U) {\
				__LOG_INTERNAL(src_level,		    \
						Z_LOG_STR(__VA_ARGS__));	    \
			} else {					    \
				__LOG_INTERNAL(src_level, __VA_ARGS__);	    \
			}						    \
		} else if (false) {					    \
			/* Arguments checker present but never evaluated.*/ \
			/* Placed here to ensure that __VA_ARGS__ are*/     \
			/* evaluated once when log is enabled.*/	    \
			log_printf_arg_checker(__VA_ARGS__);		    \
		}							    \
	} while (false)

#define Z_LOG(_level, ...)			       \
	__LOG(_level,				       \
	      (u16_t)LOG_CURRENT_MODULE_ID(),	       \
	      LOG_CURRENT_DYNAMIC_DATA_ADDR(),	       \
	      __VA_ARGS__)

#define Z_LOG_INSTANCE(_level, _inst, ...)		 \
	__LOG(_level,					 \
	      IS_ENABLED(CONFIG_LOG_RUNTIME_FILTERING) ? \
	      LOG_DYNAMIC_ID_GET(_inst) :		 \
	      LOG_CONST_ID_GET(_inst),			 \
	      _inst,					 \
	      __VA_ARGS__)


#define __LOG_HEXDUMP(_level, _id, _filter, _data, _length, _str)	      \
	do {								      \
		if (Z_LOG_CONST_LEVEL_CHECK(_level) &&			      \
		    (_level <= LOG_RUNTIME_FILTER(_filter))) {		      \
			struct log_msg_ids src_level = {		      \
				.level = _level,			      \
				.source_id = _id,			      \
				.domain_id = CONFIG_LOG_DOMAIN_ID	      \
			};						      \
									      \
			if (IS_ENABLED(CONFIG_LOG_IMMEDIATE)) {	      \
				log_hexdump_sync(src_level, _str,	      \
						 _data, _length);	      \
			} else {					      \
				log_hexdump(_str, _data, _length, src_level); \
			}						      \
		}							      \
	} while (false)

#define Z_LOG_HEXDUMP(_level, _data, _length, _str)	       \
	__LOG_HEXDUMP(_level,				       \
		      LOG_CURRENT_MODULE_ID(),		       \
		      LOG_CURRENT_DYNAMIC_DATA_ADDR(),	       \
		      _data, _length, _str)

#define Z_LOG_HEXDUMP_INSTANCE(_level, _inst, _data, _length, _str) \
	__LOG_HEXDUMP(_level,					   \
		      IS_ENABLED(CONFIG_LOG_RUNTIME_FILTERING) ?   \
		      LOG_DYNAMIC_ID_GET(_inst) :		   \
		      LOG_CONST_ID_GET(_inst),			   \
		      _inst,					   \
		      _data,					   \
		      _length,					   \
		      _str)


/** @brief Number of bits used to encode log level. */
#define LOG_LEVEL_BITS 3

/** @brief Filter slot size. */
#define LOG_FILTER_SLOT_SIZE LOG_LEVEL_BITS

/** @brief Number of slots in one word. */
#define LOG_FILTERS_NUM_OF_SLOTS (32 / LOG_FILTER_SLOT_SIZE)

/** @brief Slot mask. */
#define LOG_FILTER_SLOT_MASK (BIT(LOG_FILTER_SLOT_SIZE) - 1)

/** @brief Bit offset of a slot.
 *
 *  @param _id Slot ID.
 */
#define LOG_FILTER_SLOT_SHIFT(_id) (LOG_FILTER_SLOT_SIZE * (_id))

#define LOG_FILTER_SLOT_GET(_filters, _id) \
	((*(_filters) >> LOG_FILTER_SLOT_SHIFT(_id)) & LOG_FILTER_SLOT_MASK)

#define LOG_FILTER_SLOT_SET(_filters, _id, _filter)		     \
	do {							     \
		*(_filters) &= ~(LOG_FILTER_SLOT_MASK <<	     \
				 LOG_FILTER_SLOT_SHIFT(_id));	     \
		*(_filters) |= ((_filter) & LOG_FILTER_SLOT_MASK) << \
			       LOG_FILTER_SLOT_SHIFT(_id);	     \
	} while (false)

#define LOG_FILTER_AGGR_SLOT_IDX 0

#define LOG_FILTER_AGGR_SLOT_GET(_filters) \
	LOG_FILTER_SLOT_GET(_filters, LOG_FILTER_AGGR_SLOT_IDX)

#define LOG_FILTER_FIRST_BACKEND_SLOT_IDX 1

#if CONFIG_LOG_RUNTIME_FILTERING
#define LOG_RUNTIME_FILTER(_filter) \
	LOG_FILTER_SLOT_GET(&(_filter)->filters, LOG_FILTER_AGGR_SLOT_IDX)
#else
#define LOG_RUNTIME_FILTER(_filter) LOG_LEVEL_DBG
#endif

/** @brief Log level value used to indicate log entry that should not be
 *	   formatted (raw string).
 */
#define LOG_LEVEL_INTERNAL_RAW_STRING LOG_LEVEL_NONE

extern struct log_source_const_data __log_const_start[0];
extern struct log_source_const_data __log_const_end[0];


static inline const char *log_name_get(u32_t source_id)
{
	return __log_const_start[source_id].name;
}

static inline u8_t log_compiled_level_get(u32_t source_id)
{
	return __log_const_start[source_id].level;
}

static inline u32_t log_const_source_id(
				const struct log_source_const_data *data)
{
	return ((u8_t *)data - (u8_t *)__log_const_start)/
			sizeof(struct log_source_const_data);
}

/** @brief Get number of registered sources. */
static inline u32_t log_sources_count(void)
{
	return log_const_source_id(__log_const_end);
}

extern struct log_source_dynamic_data __log_dynamic_start[0];
extern struct log_source_dynamic_data __log_dynamic_end[0];

#define LOG_ITEM_DYNAMIC_DATA(_name) UTIL_CAT(log_dynamic_, _name)

#define LOG_INSTANCE_DYNAMIC_DATA(_module_name, _inst) \
	LOG_ITEM_DYNAMIC_DATA(LOG_INSTANCE_FULL_NAME(_module_name, _inst))


static inline u32_t *log_dynamic_filters_get(u32_t source_id)
{
	return &__log_dynamic_start[source_id].filters;
}

static inline u32_t log_dynamic_source_id(struct log_source_dynamic_data *data)
{
	return ((u8_t *)data - (u8_t *)__log_dynamic_start)/
			sizeof(struct log_source_dynamic_data);
}

/** @brief Dummy function to trigger log messages arguments type checking. */
static inline __printf_like(1, 2)
void log_printf_arg_checker(const char *fmt, ...)
{
	ARG_UNUSED(fmt);
}

void log_0(const char *str, struct log_msg_ids src_level);

void log_1(const char *str,
	   u32_t arg1,
	   struct log_msg_ids src_level);

void log_2(const char *str,
	   u32_t arg1,
	   u32_t arg2,
	   struct log_msg_ids src_level);

void log_3(const char *str,
	   u32_t arg1,
	   u32_t arg2,
	   u32_t arg3,
	   struct log_msg_ids src_level);

void log_n(const char *str,
	   u32_t *args,
	   u32_t narg,
	   struct log_msg_ids src_level);


void log_hexdump(const char *str,
		 const u8_t *data,
		 u32_t length,
		 struct log_msg_ids src_level);


int log_printk(const char *fmt, va_list ap);

void log_string_sync(struct log_msg_ids src_level, const char *fmt, ...);

void log_hexdump_sync(struct log_msg_ids src_level, const char *metadata,
		      const u8_t *data, u32_t len);

void log_generic(struct log_msg_ids src_level, const char *fmt, va_list ap);

bool log_is_strdup(void *buf);

void log_free(void *buf);

void log_dropped(void);

#ifdef __cplusplus
}
#endif

#endif /* WATCH_LOG_CORE_H_*/

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERR  1
#define LOG_LEVEL_WRN  2
#define LOG_LEVEL_INF  3
#define LOG_LEVEL_DBG  4

#define LOG_ERR(...)    Z_LOG(LOG_LEVEL_ERR, __VA_ARGS__)

#define LOG_WRN(...)   Z_LOG(LOG_LEVEL_WRN, __VA_ARGS__)

#define LOG_INF(...)   Z_LOG(LOG_LEVEL_INF, __VA_ARGS__)

#define LOG_DBG(...)    Z_LOG(LOG_LEVEL_DBG, __VA_ARGS__)


#define LOG_INST_ERR(_log_inst, ...) \
	Z_LOG_INSTANCE(LOG_LEVEL_ERR, _log_inst, __VA_ARGS__)

#define LOG_INST_WRN(_log_inst, ...) \
	Z_LOG_INSTANCE(LOG_LEVEL_WRN, _log_inst, __VA_ARGS__)

#define LOG_INST_INF(_log_inst, ...) \
	Z_LOG_INSTANCE(LOG_LEVEL_INF, _log_inst, __VA_ARGS__)

#define LOG_INST_DBG(_log_inst, ...) \
	Z_LOG_INSTANCE(LOG_LEVEL_DBG, _log_inst, __VA_ARGS__)

#define LOG_HEXDUMP_ERR(_data, _length, _str) \
	Z_LOG_HEXDUMP(LOG_LEVEL_ERR, _data, _length, _str)

#define LOG_HEXDUMP_WRN(_data, _length, _str) \
	Z_LOG_HEXDUMP(LOG_LEVEL_WRN, _data, _length, _str)

#define LOG_HEXDUMP_INF(_data, _length, _str) \
	Z_LOG_HEXDUMP(LOG_LEVEL_INF, _data, _length, _str)

#define LOG_HEXDUMP_DBG(_data, _length, _str) \
	Z_LOG_HEXDUMP(LOG_LEVEL_DBG, _data, _length, _str)

#define LOG_INST_HEXDUMP_ERR(_log_inst, _data, _length, _str) \
	Z_LOG_HEXDUMP_INSTANCE(LOG_LEVEL_ERR, _log_inst, _data, _length, _str)

#define LOG_INST_HEXDUMP_WRN(_log_inst, _data, _length, _str) \
	Z_LOG_HEXDUMP_INSTANCE(LOG_LEVEL_WRN, _log_inst, _data, _length, _str)

#define LOG_INST_HEXDUMP_INF(_log_inst, _data, _length, _str) \
	Z_LOG_HEXDUMP_INSTANCE(LOG_LEVEL_INF, _log_inst, _data, _length, _str)

#define LOG_INST_HEXDUMP_DBG(_log_inst, _data, _length, _str)	\
	Z_LOG_HEXDUMP_INSTANCE(LOG_LEVEL_DBG, _log_inst, _data, _length, _str)

int log_printk(const char *fmt, va_list ap);

char *log_strdup(const char *str);

#ifdef __cplusplus
}
#define LOG_IN_CPLUSPLUS 1
#endif

#if !defined(CONFIG_LOG)
#define _LOG_LEVEL_RESOLVE(...) LOG_LEVEL_NONE
#else
#define _LOG_LEVEL_RESOLVE(...) \
	Z_LOG_EVAL(LOG_LEVEL, \
		  (GET_ARG2(__VA_ARGS__, LOG_LEVEL)), \
		  (GET_ARG2(__VA_ARGS__, CONFIG_LOG_DEFAULT_LEVEL)))
#endif

/* Return first argument */
#define _LOG_ARG1(arg1, ...) arg1

#define _LOG_MODULE_CONST_DATA_CREATE(_name, _level)			     \
	COND_CODE_1(LOG_IN_CPLUSPLUS, (extern), ())			     \
	const struct log_source_const_data LOG_ITEM_CONST_DATA(_name)	     \
	__attribute__ ((section("." STRINGIFY(LOG_ITEM_CONST_DATA(_name))))) \
	__attribute__((used)) = {					     \
		.name = STRINGIFY(_name),				     \
		.level = _level						     \
	}

#define _LOG_MODULE_DYNAMIC_DATA_CREATE(_name)				\
	struct log_source_dynamic_data LOG_ITEM_DYNAMIC_DATA(_name)	\
	__attribute__ ((section("." STRINGIFY(				\
				     LOG_ITEM_DYNAMIC_DATA(_name))))	\
				     )					\
	__attribute__((used))

#define _LOG_MODULE_DYNAMIC_DATA_COND_CREATE(_name)		\
	COND_CODE_1(						\
		CONFIG_LOG_RUNTIME_FILTERING,			\
		(_LOG_MODULE_DYNAMIC_DATA_CREATE(_name);),	\
		()						\
		)

#define _LOG_MODULE_DATA_CREATE(_name, _level)			\
	_LOG_MODULE_CONST_DATA_CREATE(_name, _level);		\
	_LOG_MODULE_DYNAMIC_DATA_COND_CREATE(_name)

#define LOG_MODULE_REGISTER(...)					\
	Z_LOG_EVAL(							\
		_LOG_LEVEL_RESOLVE(__VA_ARGS__),			\
		(_LOG_MODULE_DATA_CREATE(GET_ARG1(__VA_ARGS__),		\
				      _LOG_LEVEL_RESOLVE(__VA_ARGS__))),\
		()/*Empty*/						\
	)								\
	LOG_MODULE_DECLARE(__VA_ARGS__)

#define LOG_MODULE_DECLARE(...)						      \
	extern const struct log_source_const_data			      \
			LOG_ITEM_CONST_DATA(GET_ARG1(__VA_ARGS__));	      \
	extern struct log_source_dynamic_data				      \
			LOG_ITEM_DYNAMIC_DATA(GET_ARG1(__VA_ARGS__));	      \
									      \
	static const struct log_source_const_data *			      \
		__log_current_const_data __attribute__((unused)) =	      \
			_LOG_LEVEL_RESOLVE(__VA_ARGS__) ?		      \
			&LOG_ITEM_CONST_DATA(GET_ARG1(__VA_ARGS__)) : NULL;   \
									      \
	static struct log_source_dynamic_data *				      \
		__log_current_dynamic_data __attribute__((unused)) =	      \
			(_LOG_LEVEL_RESOLVE(__VA_ARGS__) &&		      \
			IS_ENABLED(CONFIG_LOG_RUNTIME_FILTERING)) ?	      \
			&LOG_ITEM_DYNAMIC_DATA(GET_ARG1(__VA_ARGS__)) : NULL; \
									      \
	static const u32_t __log_level __attribute__((unused)) =	      \
					_LOG_LEVEL_RESOLVE(__VA_ARGS__)

#define LOG_LEVEL_SET(level) \
	static const u32_t __log_level __attribute__((unused)) = \
			_LOG_LEVEL_RESOLVE(level)

#endif /* WATCH_LOG_H_ */