/*******************************************************************************
* File Name        : uart_handle.c
*
* Description      : This file contains and UART CLI command handling for user
*                    interaction
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


#include "vglite_demos.h"
#include "retarget_io_init.h"

/*******************************************************************************
* Macros
*******************************************************************************/

/* Macros for UART input handling */
#define UART_KEY_CANCEL        (0x03)   /* Ctrl+C */
#define UART_KEY_ENTER_CR      (0x0D)   /* Carriage Return */
#define UART_KEY_ENTER_LF      (0x0A)   /* Line Feed */
#define UART_KEY_MIN           ('1')    /* Lowest valid app number */
#define UART_KEY_MAX           ('5')    /* Highest valid app number */

/*******************************************************************************
* Extern Variables 
*******************************************************************************/
extern QueueHandle_t event_queque;
extern bool cancel_requested;

/*******************************************************************************
 *  Function Name: benchmarking_case
 * *****************************************************************************
 * Summary:
 *  -This function handles the benchmarking cases based on user input.
 *  -It takes a case number as input and executes the corresponding demo.
 * 
 * Parameters:
 *  case_number: The number of the case to execute.
 * 
 * Return:
 *  None
 * ****************************************************************************/
void benchmarking_case(int case_number)
{
    event_t event;
    event.b_event = EVENT_DEFAULT;

    switch (case_number)
    {
    case 0:
        printf("Fill rules: two vector path shapes using different fill rules\n\r\r\n");
        printf("LEFT SIDE : VG_LITE_FILL_EVEN_ODD   &  RIGHT SIDE : VG_LITE_FILL_NON_ZERO\n\r\r\n");
        event.b_event = EVENT_FILL_RULES;
        break;
    case 1:
        printf("Alpha behavior: Alpha blending between two vector path shapes using different blend modes\n\r\r\n");
        printf("LEFT SIDE : VG_LITE_BLEND_SRC_OVER   &  RIGHT SIDE : VG_LITE_BLEND_MULTIPLY\n\r\r\n");
        event.b_event = EVENT_ALPHA_BEHAVIOR;
        break;
    case 2:
        printf("Blit color: Blit in rectangle Infineon logo\n\r\r\n");
        event.b_event = EVENT_BLIT_COLOR;
        break;
    case 3:
        printf("Pattern fill: Pattern fill demonstration\n\r\r\n");
        event.b_event = EVENT_PATTERN_FILL;
        break;
    case 4:
        printf("UI/filter: Demonstrate VG_LITE_FILTER_LINEAR\n\r\r\n");
        event.b_event = EVENT_UI_FILTER;
        break;
    default:
        printf("Invalid case\n\r");
        break;
    }

    if (event.b_event != EVENT_DEFAULT)
    {
        xQueueSend(event_queque, &event, 0);
    }
    return;
}


/*******************************************************************************
* Function Name: uart_cli_handler
********************************************************************************
* Summary:
*  -Handles UART CLI commands for the OOB application.
*  -This function displays a menu for the user to select a demo topic,       
*   processes the user's choice, and invokes the corresponding demo function.
*  -It also handles cancellation requests (Ctrl+C) to return to the menu.
*  -This function runs in an infinite loop, waiting for user input.
*
* Parameters:   
*  pvParameters: Pointer to parameters passed to the task (unused).
*
* Returns:
*  None 
*******************************************************************************/
void uart_cli_handler(void *pv_parameters)
{
    (void)pv_parameters; /* Unused parameter */
    uint8_t uart_read_choice = 0;

    while (1) 
    {   
        /* ANSI ESC sequence for clear screen */
        printf("\x1b[2J\x1b[;H");
        printf("****************** "
                    "PSOC Edge MCU: Graphics using VGLite API"
                    " ****************** \r\n\n");
        printf("List of operations to choose from: \r\n");
        printf("1. Fill Rules (Vector Paths) \n\r");
        printf("2. Alpha Blending (Vector Paths) \n\r");
        printf("3. Blit Color Rendering \n\r");
        printf("4. Pattern Fill \n\r");
        printf("5. UI/Filter Demo \n\r\r\n");

        bool waiting_for_enter = false;

        while (1) 
        {

            if ( Cy_SCB_UART_GetNumInRxFifo(CYBSP_DEBUG_UART_HW) ) 
            {
                uart_read_choice = (uint8_t) Cy_SCB_UART_Get(CYBSP_DEBUG_UART_HW);

                if (uart_read_choice == UART_KEY_CANCEL) 
                {
                    printf("Operation cancelled. Returning to menu.\r\n");
                    cancel_requested = true;
                    waiting_for_enter = false; /* reset state */
                    break;
                }
                else if (uart_read_choice == UART_KEY_ENTER_CR || uart_read_choice == UART_KEY_ENTER_LF) 
                {
                    cancel_requested = true;
                    waiting_for_enter = false;  /* allow new number input after Enter */
                    break;
                }
                else if (!waiting_for_enter && uart_read_choice >= UART_KEY_MIN && uart_read_choice <= UART_KEY_MAX) 
                {
                    uint8_t app_number = uart_read_choice - UART_KEY_MIN;
                    cancel_requested = false;
                    waiting_for_enter = true;  /* block further number input until Enter is pressed */

                    /* ANSI ESC sequence for clear screen */
                    printf("\x1b[2J\x1b[;H");

                    benchmarking_case(app_number);
                }
                else if (!waiting_for_enter) 
                {
                    printf("Invalid choice. Please try again.\r\n");
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

/* [] END OF FILE */