/*******************************************************************************
* File Name        : vglite_demos.h
*
* Description      : This file contains the declarations for the VGLite demo 
*                    event handling system. It defines the event types, event
*                    handler function pointers, and the event queue used for 
*                    processing events in the VGLite demos.
*
* Related Document : See README.md
*
********************************************************************************
* Copyright 2025, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/


#ifndef VGLITE_DEMOS_H
#define VGLITE_DEMOS_H  

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Header Files
*******************************************************************************/
#include <inttypes.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "vg_lite.h"
#include "cy_graphics.h"

/*******************************************************************************
* Macros
*******************************************************************************/

#define WHITE_COLOR                         (0x00FFFFFFU)
#define TEAL_COLOR                          (0xFF808000U)
#define RESET_VAL                           (0U)

/*******************************************************************************
* Data Structures and Types
*******************************************************************************/

typedef enum {
    EVENT_DEFAULT,
    EVENT_FILL_RULES,
    EVENT_ALPHA_BEHAVIOR,
    EVENT_BLIT_COLOR,
    EVENT_PATTERN_FILL,
    EVENT_UI_FILTER,
    /* Add more event types as needed */
    EVENT_MAX
} event_type_t;

typedef struct {
    event_type_t b_event;
    uint8_t data;
} event_t;

typedef enum {
    EXIT_0,
    EXIT_1,
    EXIT_2,
    EXIT_3,
    EXIT_4,
    /* Add more exit state as needed */
    EXIT_MAX
} exit_state;

typedef void (*event_handler_t)(void);

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void fill_rules_draw(void);
void alpha_behavior_draw(void);
void blit_color_draw(void);
void filter_draw(void);
void calculate_fps(void);
bool setup_vglite_image_buffer( vg_lite_buffer_t *buffer, uint8_t *imm_array, 
                                int32_t width, int32_t height, int32_t stride,
                                vg_lite_buffer_format_t format );
bool load_images(void);
void pattern_fill_draw(void);
void cleanup(event_type_t demo_id, uint8_t e_id);
uint32_t get_time_ms(void);
void default_draw(void);
void swap_frame_buffer( void );

/*******************************************************************************
* Extern Variables 
*******************************************************************************/
extern vg_lite_matrix_t matrix;
extern vg_lite_buffer_t *render_target;
extern vg_lite_buffer_t intermediate_buffer;
extern bool zoom_out;
extern int scale_count;

#if defined(__cplusplus)
}
#endif

#endif /* VGLITE_DEMOS_H */

/* [] END OF FILE */
