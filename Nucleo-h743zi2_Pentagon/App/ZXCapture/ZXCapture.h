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

#ifndef _ZX_CAPTURE_H_
#define _ZX_CAPTURE_H_

#include <inttypes.h>


typedef struct ZXCapture_HandleStruct 
{
    void* drv_handle;
    uint8_t is_dual_ring;

}ZXCapture_HandleTypeDef;

#endif