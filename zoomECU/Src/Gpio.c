/******************************************************************************
* File:                    Gpio.c
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Provides initialization and control of GPIO pins
*******************************************************************************
* Includes
******************************************************************************/
#include "Gpio.h"
#include "PinoutConfiguration.h"
#include "stm32g4xx.h"

/******************************************************************************
* Defines
******************************************************************************/
#define GPIO_MODE_INPUT                 (0x00000000U)       /*!< Select input mode */
#define GPIO_MODE_OUTPUT                GPIO_MODER_MODE0_0  /*!< Select output mode */
#define GPIO_MODE_ALTERNATE             GPIO_MODER_MODE0_1  /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG                GPIO_MODER_MODE0    /*!< Select analog mode */

#define GPIO_OUTPUT_PUSHPULL            (0x00000000U)       /*!< Select push-pull as output type */
#define GPIO_OUTPUT_OPENDRAIN           GPIO_OTYPER_OT0     /*!< Select open-drain as output type */

#define GPIO_SPEED_LOW                  (0x00000000U)           /*!< Select I/O low output speed    */
#define GPIO_SPEED_MEDIUM               GPIO_OSPEEDR_OSPEED0_0  /*!< Select I/O medium output speed */
#define GPIO_SPEED_HIGH                 GPIO_OSPEEDR_OSPEED0_1  /*!< Select I/O fast output speed   */
#define GPIO_SPEED_VERY_HIGH            GPIO_OSPEEDR_OSPEED0    /*!< Select I/O high output speed   */

#define GPIO_PULL_NO                    (0x00000000U)       /*!< Select I/O no pull */
#define GPIO_PULL_UP                    GPIO_PUPDR_PUPD0_0  /*!< Select I/O pull up */
#define GPIO_PULL_DOWN                  GPIO_PUPDR_PUPD0_1  /*!< Select I/O pull down */

#define GPIO_AF_0                       (0x0000000U) /*!< Select alternate function 0 */
#define GPIO_AF_1                       (0x0000001U) /*!< Select alternate function 1 */
#define GPIO_AF_2                       (0x0000002U) /*!< Select alternate function 2 */
#define GPIO_AF_3                       (0x0000003U) /*!< Select alternate function 3 */
#define GPIO_AF_4                       (0x0000004U) /*!< Select alternate function 4 */
#define GPIO_AF_5                       (0x0000005U) /*!< Select alternate function 5 */
#define GPIO_AF_6                       (0x0000006U) /*!< Select alternate function 6 */
#define GPIO_AF_7                       (0x0000007U) /*!< Select alternate function 7 */
#define GPIO_AF_8                       (0x0000008U) /*!< Select alternate function 8 */
#define GPIO_AF_9                       (0x0000009U) /*!< Select alternate function 9 */
#define GPIO_AF_10                      (0x000000AU) /*!< Select alternate function 10 */
#define GPIO_AF_11                      (0x000000BU) /*!< Select alternate function 11 */
#define GPIO_AF_12                      (0x000000CU) /*!< Select alternate function 12 */
#define GPIO_AF_13                      (0x000000DU) /*!< Select alternate function 13 */
#define GPIO_AF_14                      (0x000000EU) /*!< Select alternate function 14 */
#define GPIO_AF_15                      (0x000000FU) /*!< Select alternate function 15 */

/******************************************************************************
* Public Variables
******************************************************************************/


/******************************************************************************
* Private Function Prototypes (static)
******************************************************************************/
static void gpio_initPin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode, uint32_t Speed, uint32_t OutputType, uint32_t Pull, uint32_t Alternate);

/******************************************************************************
* Private Variables (static)
******************************************************************************/


/******************************************************************************
* Function Code
******************************************************************************/

/******************************************************************************
* void Gpio_Init(void)
* Initializes all GPIO for the zoomECU
* David Tolsma, 05/25/2020
******************************************************************************/
void Gpio_Init(void)
{
    // Enable GPIO Port Clocks
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOFEN);

    // Initialize Pins
    gpio_initPin(LSD_1_PORT, LSD_1_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_2_PORT, LSD_2_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_3_PORT, LSD_3_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_4_PORT, LSD_4_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_5_PORT, LSD_5_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_6_PORT, LSD_6_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_7_PORT, LSD_7_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(LSD_8_PORT, LSD_8_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);

    gpio_initPin(PP_1_PORT, PP_1_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_2_PORT, PP_2_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_3_PORT, PP_3_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_4_PORT, PP_4_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_5_PORT, PP_5_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_6_PORT, PP_6_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_7_PORT, PP_7_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(PP_8_PORT, PP_8_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    
    gpio_initPin(NEUTRAL_SW_PORT, NEUTRAL_SW_PIN, GPIO_MODE_INPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(IDLE_SW_PORT, IDLE_SW_PIN, GPIO_MODE_INPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);

    gpio_initPin(VR_1_PORT, VR_1_PIN, GPIO_MODE_INPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(VR_2_PORT, VR_2_PIN, GPIO_MODE_INPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);

    gpio_initPin(CRANK_PORT, CRANK_PIN, GPIO_MODE_INPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(CAM_PORT, CAM_PIN, GPIO_MODE_INPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);

    gpio_initPin(STEP_EN_PORT, STEP_EN_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(STEP_DIR_PORT, STEP_DIR_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
    gpio_initPin(STEP_STEP_PORT, STEP_STEP_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);

    gpio_initPin(DEV_LED_PORT, DEV_LED_PIN, GPIO_MODE_OUTPUT, GPIO_SPEED_VERY_HIGH, GPIO_OUTPUT_PUSHPULL, GPIO_PULL_NO, GPIO_AF_0);
}
/*****************************************************************************/


/******************************************************************************
* uint32_t Gpio_ReadInputPin(Port, Pin)
* Returns a boolean value for an STM32G4 GPIO Pin
* David Tolsma, 05/25/2020
******************************************************************************/
uint32_t Gpio_ReadInputPin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
	return ((READ_REG(GPIOx->IDR) & Pin) >> POSITION_VAL(Pin));
}
/*****************************************************************************/


/******************************************************************************
* void Gpio_SetPin(Port, Pin)
* Sets an STM32G4 GPIO output pin
* David Tolsma, 05/25/2020
******************************************************************************/
void Gpio_SetPin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    WRITE_REG(GPIOx->BSRR, Pin);
}
/*****************************************************************************/


/******************************************************************************
* void GPIO_ResetPin(Port, Pin)
* Resets an STM32G4 GPIO output pin
* David Tolsma, 05/25/2020
******************************************************************************/
void Gpio_ResetPin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    WRITE_REG(GPIOx->BRR, Pin);
}
/*****************************************************************************/


/******************************************************************************
* void Gpio_TogglePin(void)
* Toggles an STM32G4 GPIO output pin
* David Tolsma, 05/25/2020
******************************************************************************/
void Gpio_TogglePin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
    WRITE_REG(GPIOx->ODR, READ_REG(GPIOx->ODR) ^ Pin);
}
/*****************************************************************************/


/******************************************************************************
* void gpio_initPin(Port, Pin, Mode, Speed, OutputType, Pull, AlternateMode))
* Library function for initializing STM32G4 GPIO pin
* David Tolsma, 05/25/2020
******************************************************************************/
void gpio_initPin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode, uint32_t Speed, uint32_t OutputType, uint32_t Pull, uint32_t AlternateMode)
{
    // Reset Pin
    WRITE_REG(GPIOx->BRR, Pin);
    // Set Mode
    MODIFY_REG(GPIOx->MODER, (GPIO_MODER_MODE0 << (POSITION_VAL(Pin) * 2U)), (Mode << (POSITION_VAL(Pin) * 2U)));
    // Set Output Type
    MODIFY_REG(GPIOx->OTYPER, Pin, (Pin * OutputType));
    // Set Speed 
    MODIFY_REG(GPIOx->OSPEEDR, (GPIO_OSPEEDR_OSPEED0 << (POSITION_VAL(Pin) * 2U)), (Speed << (POSITION_VAL(Pin) * 2U)));
    // Set Pull
    MODIFY_REG(GPIOx->PUPDR, (GPIO_PUPDR_PUPD0 << (POSITION_VAL(Pin) * 2U)), (Pull << (POSITION_VAL(Pin) * 2U)));
    // Set Alternate Mode
    MODIFY_REG(GPIOx->AFR[0], (GPIO_AFRL_AFSEL0 << (POSITION_VAL(Pin) * 4U)), (AlternateMode << (POSITION_VAL(Pin) * 4U)));
}
/*****************************************************************************/

void gpioOn(void){
	//Gpio_SetPin(PP_1_PORT, PP_1_PIN);
	//Gpio_SetPin(PP_2_PORT, PP_2_PIN);
	//Gpio_SetPin(PP_3_PORT, PP_3_PIN);
	//Gpio_SetPin(PP_4_PORT, PP_4_PIN);
	//Gpio_SetPin(PP_5_PORT, PP_5_PIN);
	//Gpio_SetPin(PP_6_PORT, PP_6_PIN);
	Gpio_SetPin(PP_7_PORT, PP_7_PIN);
	Gpio_SetPin(PP_8_PORT, PP_8_PIN);

	//Gpio_SetPin(LSD_1_PORT, LSD_1_PIN);
	//Gpio_SetPin(LSD_2_PORT, LSD_2_PIN);
	//Gpio_SetPin(LSD_3_PORT, LSD_3_PIN);
	//Gpio_SetPin(LSD_4_PORT, LSD_4_PIN);
	//Gpio_SetPin(LSD_5_PORT, LSD_5_PIN);
	//Gpio_SetPin(LSD_6_PORT, LSD_6_PIN);
	//Gpio_SetPin(LSD_7_PORT, LSD_7_PIN);
	//Gpio_SetPin(LSD_8_PORT, LSD_8_PIN);

	//Gpio_SetPin(DEV_LED_PORT, DEV_LED_PIN);

	//puts("Turning all outputs on. \n");

}


void gpioOff(void){
	//Gpio_ResetPin(PP_1_PORT, PP_1_PIN);
	//Gpio_ResetPin(PP_2_PORT, PP_2_PIN);
	//Gpio_ResetPin(PP_3_PORT, PP_3_PIN);
	//Gpio_ResetPin(PP_4_PORT, PP_4_PIN);
	//Gpio_ResetPin(PP_5_PORT, PP_5_PIN);
	//Gpio_ResetPin(PP_6_PORT, PP_6_PIN);
	Gpio_ResetPin(PP_7_PORT, PP_7_PIN);
	Gpio_ResetPin(PP_8_PORT, PP_8_PIN);

	//Gpio_ResetPin(LSD_1_PORT, LSD_1_PIN);
	//Gpio_ResetPin(LSD_2_PORT, LSD_2_PIN);
	//Gpio_ResetPin(LSD_3_PORT, LSD_3_PIN);
	//Gpio_ResetPin(LSD_4_PORT, LSD_4_PIN);
	//Gpio_ResetPin(LSD_5_PORT, LSD_5_PIN);
	//Gpio_ResetPin(LSD_6_PORT, LSD_6_PIN);
	//Gpio_ResetPin(LSD_7_PORT, LSD_7_PIN);
	//Gpio_ResetPin(LSD_8_PORT, LSD_8_PIN);

	//Gpio_ResetPin(DEV_LED_PORT, DEV_LED_PIN);

	//printf("Turning all outputs off. \n");

}
