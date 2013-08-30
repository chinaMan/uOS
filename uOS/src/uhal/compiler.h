/*============================================================================*/
/*
 * @brief:   compiler sepcified config
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef COMPILER_H
#define COMPILER_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 
/*==============================[marco]=======================================*/
#define EXTERN extern
#define STATIC static
#define CONST  const
#define INLINE __inline
#define VOLIATE 

/*==============================[typedef]=====================================*/
typedef unsigned char  uint8;
typedef unsigned char  boolean;
typedef unsigned char  byte;
typedef          char  sint8;
typedef unsigned short uint16;
typedef          short sint16;
typedef unsigned int   uint32;
typedef          int   sint32;

#ifdef  FALSE
    #undef  FALSE
#endif
#define FALSE 0

#ifdef  TRUE 
    #undef  TRUE
#endif
#define TRUE  1

#ifdef NULL
    #undef NULL
#endif
#define NULL  ((void *)0)

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* COMPILER_H */
