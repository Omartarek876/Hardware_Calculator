/*
 * StdTypes.h
 *
 *  Created on: Sep 16, 2023
 *      Author: omar
 */
#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char           u8;
typedef signed char             s8;
typedef unsigned short int      u16;
typedef signed short int        s16;
typedef unsigned long int       u32;
typedef signed long int         s32;
typedef unsigned long long int  u64;
typedef signed long long int    s64;
typedef float                   f32;
typedef double                  f64;
typedef long double             f128;

typedef enum
{
    E_OK,
    E_NOT_OK
} StdReturnType;

#endif