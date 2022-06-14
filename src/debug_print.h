/*
 * debug.h
 *
 *  Created on: 9 juin 2022
 *      Author: pa267340
 */

#ifndef DEBUG_PRINT_H_
#define DEBUG_PRINT_H_

#if 0
  #define DBG(a) printf a
#else
  #define DBG(a) (void)0
#endif

#if 0
  #define DBG_print_pkt(a) printf a
#else
  #define DBG_print_pkt(a) (void)0
#endif

#endif /* DEBUG_PRINT_H_ */
