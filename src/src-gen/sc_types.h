
#ifndef SC_TYPES_H_
#define SC_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdint.h>
#include <stdbool.h>
 
typedef bool			sc_boolean;
typedef int_fast16_t	sc_short;
typedef uint_fast16_t	sc_ushort;
typedef int32_t			sc_integer; 
typedef uint32_t		sc_uinteger; 
typedef double			sc_real;
typedef char*			sc_string;

typedef void*			sc_eventid;

#ifdef __cplusplus
}
#endif

#ifndef null
	#ifdef __cplusplus
		#define null 0
	#else
		#define null ((void *)0)
	#endif
#endif

#define bool_true true
#define bool_false false

#endif /* SC_TYPES_H_ */
