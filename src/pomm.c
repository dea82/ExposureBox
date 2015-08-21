/******************************************************************************
 * Copyright (C) 2014 Andreas, Sweden
 * <andreas.laghamn@gmail.com>
 * http://laghamn.dyndns.org
 *****************************************************************************
 *
 * File    : pomm.c
 * Date    : 29 maj 2014
 * 
 * Summary : POwer Manager/Monitor
 *
 * Handles:
 * - power loss (shutdown)
 *      - monitors supply voltage and triggers interrupt if below bandgap
 *          voltage.
 *      - when supply voltage is lost duty cycle data is stored in EEPROM.
 * - startup
 *      - verifies EEPROM memory
 *      - monitors primary and secondary eeprom image
 *
 *****************************************************************************/

/******************************************************************************
 * Header files
 *****************************************************************************/
#include <avr/interrupt.h>

#include "conf.h"
#include "gpio.h"
#include "lcdc.h"
#include "types.h"
#include "pomm.h"
#include "eepr.h"

static tPOMM_VERIFICATION_STATUS_E verificationStatus_E = POMM_EEPROM_STATUS_PENDING_E;
static tB powerDown_B = FALSE;
/******************************************************************************
 * Function prototypes
 *****************************************************************************/
static void setupPowerMonitor(void);
static void startPowerMonitor(void);
static void verifyEeprom(void);

/******************************************************************************
 * Functions
 *****************************************************************************/
void Pomm_init(void)
{
    setupPowerMonitor();
    startPowerMonitor();
    //TODO: Verify eeprom data, move to during handling in state instead.
    verifyEeprom();
}

static void setupPowerMonitor(void)
{

    /* Setup power loss detector pin. */
    gpio_conf(PWR_LOSS_DETECTOR_CFG, INPUT, NOPULLUP);

    /* Disable multiplexer, AIN1 used. */
    SFIOR &= ~(1 << ACME);

    /* Analog Comparator Bandgap Select 1.23 V. */
    ACSR |= (1 << ACBG);

    /* Configure interrupt on rising output edge */
    ACSR |= (1 << ACIS1) | (1 << ACIS0);

}

static void startPowerMonitor(void)
{
    /* Clear interrupt flag */
    ACSR &= ~(1 << ACI);

    /* Enable interrupt */
    ACSR |= (1 << ACIE);

    /* Prepare EEPROM for write */
    Eepr_writeEnable_B();
}

ISR(ANA_COMP_vect)
{
    uint8_t i = 0;
    Lcdc_setBacklight(FALSE);
    // Turn off everything
    for (i = 0; i < 100; i++)
    {
        //Eepr_writeByte_B(i, i+'a');
    }
    powerDown_B = TRUE;
}

tB Pomm_powerDown_B(void)
{
    return powerDown_B;
}

tPOMM_VERIFICATION_STATUS_E Pomm_getVerificationState_E(void)
{
    return verificationStatus_E;
}

static void verifyEeprom(void)
{
    verificationStatus_E = POMM_EEPROM_STATUS_OK_E;
}
