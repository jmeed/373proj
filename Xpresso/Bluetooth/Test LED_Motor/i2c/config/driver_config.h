/*
 * driver_config.h
 *
 *  Created on: Aug 31, 2010
 *      Author: nxp28548
 */

#ifndef DRIVER_CONFIG_H_
#define DRIVER_CONFIG_H

#include <LPC11xx.h>

#define CONFIG_ENABLE_DRIVER_CRP						1
#define CONFIG_CRP_SETTING_NO_CRP						1

#define CONFIG_ENABLE_DRIVER_I2C						1
#define CONFIG_I2C_DEFAULT_I2C_IRQHANDLER				1

<<<<<<< HEAD
#define CONFIG_ENABLE_DRIVER_GPIO						1
#define CONFIG_GPIO_DEFAULT_PIOINT0_IRQHANDLER						1
#define CONFIG_GPIO_DEFAULT_PIOINT1_IRQHANDLER						1
#define CONFIG_GPIO_DEFAULT_PIOINT3_IRQHANDLER						1

=======
#define CONFIG_ENABLE_DRIVER_UART						1
#define CONFIG_UART_DEFAULT_UART_IRQHANDLER				1
#define CONFIG_UART_ENABLE_INTERRUPT					1
>>>>>>> 89493145aa9b93ddb9360c3b9fbe7b19b4c27ee9

 /* DRIVER_CONFIG_H_ */
#endif
