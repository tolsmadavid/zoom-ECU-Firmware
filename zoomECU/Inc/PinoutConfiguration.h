/******************************************************************************
* File:                    PinoutConfiguration.h
* Author:                  David Tolsma
* Date Modified:           05/25/2020
* Breif Description:       Pinout Definitions for zoomECU
******************************************************************************/
#ifndef pinoutConfiguration_h
#define pinoutConfiguration_h

/******************************************************************************
* Defines
******************************************************************************/
// GPIO Pinout

// Low Side Drivers
#define LSD_1_PIN (0x1UL << 9) 
#define LSD_2_PIN (0x1UL << 8) 
#define LSD_3_PIN (0x1UL << 7) 
#define LSD_4_PIN (0x1UL << 6)
#define LSD_5_PIN (0x1UL << 15) 
#define LSD_6_PIN (0x1UL << 14) 
#define LSD_7_PIN (0x1UL << 13) 
#define LSD_8_PIN (0x1UL << 12)  

#define LSD_1_PORT GPIOC
#define LSD_2_PORT GPIOC
#define LSD_3_PORT GPIOC
#define LSD_4_PORT GPIOC
#define LSD_5_PORT GPIOD
#define LSD_6_PORT GPIOD
#define LSD_7_PORT GPIOD
#define LSD_8_PORT GPIOD

// Push Pull Drivers
#define PP_1_PIN (0x1UL << 4)
#define PP_2_PIN (0x1UL << 5)
#define PP_3_PIN (0x1UL << 6)
#define PP_4_PIN (0x1UL << 7)
#define PP_5_PIN (0x1UL << 2)
#define PP_6_PIN (0x1UL << 3)
#define PP_7_PIN (0x1UL << 4)
#define PP_8_PIN (0x1UL << 5)

#define PP_1_PORT GPIOB
#define PP_2_PORT GPIOB
#define PP_3_PORT GPIOB
#define PP_4_PORT GPIOB
#define PP_5_PORT GPIOE
#define PP_6_PORT GPIOE
#define PP_7_PORT GPIOE
#define PP_8_PORT GPIOE

// Variable Reluctance Decoder
#define VR_1_PIN (0x1UL << 1)
#define VR_2_PIN (0x1UL << 0)

#define VR_1_PORT GPIOC
#define VR_2_PORT GPIOB

// Crank and Cam Sensor Decoder
#define CRANK_PIN (0x1UL << 3)
#define CAM_PIN (0x1UL << 1)

#define CRANK_PORT GPIOA
#define CAM_PORT GPIOB

// Stepper Motor Driver
#define STEP_EN_PIN (0x1UL << 2)
#define STEP_DIR_PIN (0x1UL << 3)
#define STEP_STEP_PIN (0x1UL << 2)

#define STEP_EN_PORT GPIOC
#define STEP_DIR_PORT GPIOC
#define STEP_STEP_PORT GPIOF

// USB VSENSE 
#define USB_VSENSE_PIN (0x1UL << 9)

#define USB_VSENSE_PORT GPIOA

// Neutral and Idle Switch 
#define NEUTRAL_SW_PIN (0x1UL << 0)
#define IDLE_SW_PIN (0x1UL << 0)

#define NEUTRAL_SW_PORT GPIOE
#define IDLE_SW_PORT GPIOE

// SD Card
#define SD_NSS_PIN (0x1UL << 4)
#define SD_SCLK_PIN (0x1UL << 5)
#define SD_MISO_PIN (0x1UL << 6)
#define SD_MOSI_PIN (0x1UL << 7)
#define SD_DETECT_PIN (0x1UL << 4)

#define SD_NSS_PORT GPIOA
#define SD_SCLK_PORT GPIOA
#define SD_MISO_PORT GPIOA
#define SD_MOSI_PORT GPIOA
#define SD_DETECT_PORT GPIOC

// Sensor Inputs
#define MAP_PIN (0x1UL << 0)
#define AFM_PIN (0x1UL << 1)
#define TPS_PIN (0x1UL << 5)
#define O2_PIN (0x1UL << 9)
#define IAT_PIN (0x1UL << 14)
#define CTS_PIN (0x1UL << 15)
#define BAT_PIN (0x1UL << 12)
#define IN_A_PIN (0x1UL << 9)
#define IN_B_PIN (0x1UL << 20)
#define IN_C_PIN (0x1UL << 8)

#define MAP_PORT GPIOA
#define AFM_PORT GPIOA
#define TPS__PORT GPIOC
#define O2_PORT GPIOE
#define IAT_PORT GPIOE
#define CTS_PORT GPIOE
#define BAT_PORT GPIOB
#define IN_A_PORT GPIOA
#define IN_B_PORT GPIOD
#define IN_C_PORT GPIOE

// CAN Bus
#define CAN_TX_PIN (0x1UL << 1)
#define CAN_RX_PIN (0x1UL << 0)

#define CAN_TX_PORT GPIOD
#define CAN_RX_PORT GPIOD

// USB Bus
#define USB_DP_PIN (0x1UL << 12)
#define USB_DM_PIN (0x1UL << 11)

#define USB_DP_PORT GPIOA
#define USB_DM_PORT GPIOA

// Development Board LED
#define DEV_LED_PORT GPIOA
#define DEV_LED_PIN (0x1UL << 5)

/*****************************************************************************/


#endif //pinoutConfiguraton_h
