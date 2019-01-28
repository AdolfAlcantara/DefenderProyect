/*
 * system.h
 *
 *  Created on: Jan 28, 2019
 *      Author: ideras
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <cstdint>

#define COL_COUNT 80
#define ROW_COUNT 30
#define VGAFB_SIZE (COL_COUNT * ROW_COUNT)

typedef uint16_t *vga_ptr_t;

extern uint16_t vgafb[];

#define VGA_START_ADDR      (&vgafb[0])
#define VGA_END_ADDR        (&vgafb[VGAFB_SIZE])
#define BTN_STATE_REG_ADDR  ((volatile unsigned char *)0xffff0004)
#define MS_COUNTER_REG_ADDR ((volatile unsigned int  *)0xffff0008)

#endif /* SYSTEM_H_ */
 
