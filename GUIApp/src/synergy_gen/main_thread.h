/* generated thread header file - do not edit */
#ifndef MAIN_THREAD_H_
#define MAIN_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void main_thread_entry(void);
                #else
extern void main_thread_entry(void);
#endif
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "sf_external_irq.h"
#include "sf_i2c.h"
#include "sf_i2c_api.h"
#include "sf_touch_panel_chip_sx8654.h"
#include "sf_touch_panel_v2.h"
#include "sf_touch_panel_v2_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/* External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq0;
#ifndef NULL
void NULL(external_irq_callback_args_t *p_args);
#endif
/** SF External IRQ on SF External IRQ Instance. */
extern const sf_external_irq_instance_t g_sf_external_irq0;
/* SF I2C on SF I2C Instance. */
extern const sf_i2c_instance_t g_sf_i2c_device0;
extern const sf_touch_panel_chip_instance_t g_touch_panel_chip_sx8654;
extern const sf_touch_panel_chip_api_t g_sf_touch_panel_chip_sx8654;
/** SF Touch Panel V2 on SF Touch Panel V2 Instance. */
extern const sf_touch_panel_v2_instance_t g_sf_touch_panel;
void g_sf_touch_panel_err_callback(void *p_instance, void *p_data);
#ifndef touch_panel_v2_callback
void touch_panel_v2_callback(sf_touchpanel_v2_callback_args_t *p_args);
#endif			
void sf_touch_panel_v2_init(void);
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
extern const spi_cfg_t g_spi_lcdc_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi_lcdc;
extern sci_spi_instance_ctrl_t g_spi_lcdc_ctrl;
extern const sci_spi_extended_cfg g_spi_lcdc_cfg_extend;

#ifndef g_lcd_spi_callback
void g_lcd_spi_callback(spi_callback_args_t *p_args);
#endif

#define SYNERGY_NOT_DEFINED (1)            
#if (SYNERGY_NOT_DEFINED == g_transfer0)
    #define g_spi_lcdc_P_TRANSFER_TX (NULL)
#else
#define g_spi_lcdc_P_TRANSFER_TX (&g_transfer0)
#endif
#if (SYNERGY_NOT_DEFINED == g_transfer1)
    #define g_spi_lcdc_P_TRANSFER_RX (NULL)
#else
#define g_spi_lcdc_P_TRANSFER_RX (&g_transfer1)
#endif
#undef SYNERGY_NOT_DEFINED

#define g_spi_lcdc_P_EXTEND (&g_spi_lcdc_cfg_extend)
extern TX_SEMAPHORE g_main_semaphore_lcdc;
extern TX_EVENT_FLAGS_GROUP g_touch_event_flags;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* MAIN_THREAD_H_ */
