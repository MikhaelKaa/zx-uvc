/*
 * 
 * 
 * 
 * 
 * 
 * Authors
 * - VasilievS [imp.cepera@gmail.com]
 * - Kaa [@gmail.com]
 */

#ifndef _ZX_CAPTURE_INTERFACE_H_
#define _ZX_CAPTURE_INTERFACE_H_

struct ZXCAPTURE_IfStruct 
{
    ZXCAPTURE_IF_GPIO_ReadPin_Ptr      IC_IF_GPIO_ReadPin;
    ZXCAPTURE_IF_GPIO_WritePin_Ptr     IC_IF_GPIO_WritePin;
    ZXCAPTURE_IF_SPI_Receive_Ptr       IC_IF_SPI_Receive;
    ZXCAPTURE_IF_SPI_Transmit_Ptr      IC_IF_SPI_Transmit;
    ZXCAPTURE_IF_GetTickMs_Ptr         IC_IF_GetTickMs;
    ZXCAPTURE_IF_DelayMs_Ptr           IC_IF_DelayMs;
};

#endif