/*
 * shared.h
 *
 *  Created on: Mar 31, 2013
 *      Author: ftheo
 */

#ifndef SHARED_H_
#define SHARED_H_


#define DISABLE_IRQ (IER_THRE | IER_RLS)
#define ENABLE_IRQ (IER_THRE | IER_RLS | IER_RBR)

// Bluetooth opcodes
#define AUTHENTICATE 0


#endif /* SHARED_H_ */
