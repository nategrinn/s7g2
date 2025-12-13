/* generated HAL source file - do not edit */
#include "hal_data.h"
static qspi_instance_ctrl_t g_qspi0_ctrl;
const qspi_cfg_t g_qspi0_cfg =
{ .p_extend = NULL, .addr_mode = QSPI_3BYTE_ADDR_MODE, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const qspi_instance_t g_qspi0 =
{ .p_ctrl = &g_qspi0_ctrl, .p_cfg = &g_qspi0_cfg, .p_api = &g_qspi_on_qspi, };
void g_hal_init(void)
{
    g_common_init ();
}
