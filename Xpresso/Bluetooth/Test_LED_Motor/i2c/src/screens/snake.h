/*
 * snake.h
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdint.h>

void main_snake();
void add_snake_square(int8_t x, int8_t y);

enum snake_dir {S_U, S_R, S_D, S_L};

enum snake_dir s_dir;

uint8_t s_tick;

#endif /* SNAKE_H_ */
