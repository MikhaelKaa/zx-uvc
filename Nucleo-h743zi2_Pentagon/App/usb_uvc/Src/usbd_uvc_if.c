#include "usbd_uvc_if.h"

void Camera_On(void);
void Camera_Off(void);
uint8_t *Camera_GetFrame(uint32_t *pFrameLength);
void Camera_FreeFrame(uint8_t *frame);


USBD_UVC_CameraTypeDef USBD_UVC_Camera =
{
	Camera_On,
	Camera_Off,
	Camera_GetFrame,
	Camera_FreeFrame
};

void Camera_On(void){}

void Camera_Off(void){}

void Camera_FreeFrame(uint8_t *frame){}
int wait_cnt = 0;
int uvc_wait_cnt = 14000;
uint8_t *Camera_GetFrame(uint32_t *pFrameLength)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	HAL_GPIO_TogglePin(test_pin1_GPIO_Port, test_pin1_Pin);
	pFrameLength[0] = UVC_VIDEO_MAX_FRAME_BUF_SIZE;
	#ifdef UVC_USE_RGB565
	// Пробный вариант
	/*while(uvc_wait_flag) {
		__asm("nop");
		if(wait_cnt++ > uvc_wait_cnt) {
			wait_cnt = 0;
			uvc_wait_flag  = 0;
		}
	}*/
	#endif /* UVC_USE_RGB565 */
	HAL_GPIO_TogglePin(test_pin2_GPIO_Port, test_pin2_Pin);
	//uvc_wait_flag = 0;
	return (uint8_t *)&ucv_buf;
}
