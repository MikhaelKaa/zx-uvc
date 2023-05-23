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

#define ZXCAPTURE_DRV_WRITE_PIN(HANDLE, PIN, VAL)   ((HANDLE)->interface.IC_IF_GPIO_WritePin((PIN), (VAL)))
#define ZXCAPTURE_DRV_READ_PIN(HANDLE, PIN)         ((HANDLE)->interface.IC_IF_GPIO_ReadPin((PIN)))
#define ZXCAPTURE_DRV_GETTICK(HANDLE)               ((HANDLE)->interface.IC_IF_GetTickMs())
#define ZXCAPTURE_DRV_DELAY(HANDLE, DELAY)          ((HANDLE)->interface.IC_IF_DelayMs((DELAY)))

typedef struct ZXCAPTURE_DRV_HandleStruct 
{
    ZXCAPTURE_If interface;

}ZXCAPTURE_DRV_HandleTypeDef;

 ZXCAPTURE_StatusTypeDef ZXCAPTURE_DRV_init(ZXCAPTURE_DRV_HandleTypeDef *handle, ZXCAPTURE_IfTypeDef interface );
 ZXCAPTURE_StatusTypeDef ZXCAPTURE_DRV_init(ZXCAPTURE_DRV_HandleTypeDef *handle, ZXCAPTURE_IfTypeDef interface );