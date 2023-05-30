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


void ZXCAPTURE_process(&_zx_handle)
{
        if(DCMI_flag) 
        {
            copy_pixels();
            UVC_flag = 1;
            DCMI_flag = 0;
        }
        printf_flush();
}