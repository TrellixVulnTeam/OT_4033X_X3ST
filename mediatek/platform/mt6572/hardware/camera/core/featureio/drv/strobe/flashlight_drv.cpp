








#define LOG_TAG "FlashlightDrv"
#include <utils/Errors.h>
#include <cutils/log.h>
#include <fcntl.h>

//
#include "ae_feature.h"
#include "../inc/strobe_drv.h"
#include "flashlight_drv.h"
#include "kd_flashlight.h"

#include <CamLog.h>
#define STROBE_DEV_NAME    "/dev/kd_camera_flashlight"

#define DEBUG_STROBE_DRV
#ifdef  DEBUG_STROBE_DRV
#define DRV_DBG(fmt, arg...)  CAM_LOGD( fmt, ##arg)
#define DRV_ERR(fmt, arg...)  CAM_LOGD("Err: %5d:, "fmt, __LINE__, ##arg)
#else
#define DRV_DBG(a,...)
#define DRV_ERR(a,...)
#endif


#include <time.h>

static int getMs()
{
	//	max:
	//	2147483648 digit
	//	2147483.648 second
	//	35791.39413 min
	//	596.5232356 hour
	//	24.85513481 day
	int t;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (tv.tv_sec*1000 + (tv.tv_usec+500)/1000);
	return t;
}



//#include <CamLog.h>
//#define MY_LOGV(fmt, arg...)    CAM_LOGV("[%s] "fmt, __FUNCTION__, ##arg)
//#define MY_LOGD(fmt, arg...)    CAM_LOGD("[%s] "fmt, __FUNCTION__, ##arg)


//#define STROB_DRV_DBG(fmt, arg...) CAM_LOGD(LOG_TAG fmt, ##arg)
//#define STROB_DRV_ERR(fmt, arg...) CAM_LOGD(LOG_TAG "Err: %5d:, "fmt, __LINE__, ##arg)

StrobeDrv* FlashlightDrv::getInstance()
{
	DRV_DBG("getInstance line=%d",__LINE__);
    static FlashlightDrv singleton;
    return &singleton;
}


void FlashlightDrv::destroyInstance()
{
	DRV_DBG("destroyInstance line=%d",__LINE__);
}

int FlashlightDrv::getVBat(int* vbat)
{
	int err=0;
	int v;
	FILE* fp;
	fp = fopen("/sys/devuces/platform/mt6329-battery/FG_Battery_CurrentConsumption", "rb");
	if(fp!=0)
	{
		int ret;
		ret = fscanf(fp,"%d",&v);
		if(ret == 1) //read 1
		{
			*vbat = v;
		}
		else
			err = STROBE_FILE_ERR2;
		fclose(fp);
	}
	else
	{
		err = STROBE_FILE_ERR;
	}
	return err;
}
int FlashlightDrv::setOnOff(int a_isOn)
{
	DRV_DBG("setOnOff() isOn = %d\n",a_isOn);
	int err = 0;
	if (m_fdSTROBE < 0)
	{
	    DRV_DBG("setOnOff() m_fdSTROBE < 0\n");
	    return StrobeDrv::STROBE_UNKNOWN_ERROR;
	}


	if(a_isOn==1)
		err = ioctl(m_fdSTROBE,FLASH_IOC_SET_ONOFF,1);
	else if(a_isOn==0)
		err = ioctl(m_fdSTROBE,FLASH_IOC_SET_ONOFF,0);
	else
		err = STROBE_ERR_PARA_INVALID;

	if (err < 0)
	{
	    DRV_ERR("setOnOff() err=%d\n", err);
	}
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::setStep(int step)
{
	DRV_DBG("setStep() step = %d\n",step);
	int err = 0;
	if (m_fdSTROBE < 0)
	{
	    DRV_DBG("setStep() m_fdSTROBE < 0\n");
	    return StrobeDrv::STROBE_UNKNOWN_ERROR;
	}
	err = ioctl(m_fdSTROBE,FLASH_IOC_SET_STEP,step);
	if (err < 0)
	{
	    DRV_ERR("setOnOff() err=%d\n", err);
	}
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::setDuty(int duty)
{
	DRV_DBG("setDuty() duty = %d\n",duty);
	int err = 0;
	if (m_fdSTROBE < 0)
	{
	    DRV_DBG("setDuty() m_fdSTROBE < 0\n");
	    return StrobeDrv::STROBE_UNKNOWN_ERROR;
	}
	err = ioctl(m_fdSTROBE,FLASH_IOC_SET_DUTY,duty);
	if (err < 0)
	{
	    DRV_ERR("setDuty() err=%d\n", err);
	}
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::getFlashReg(int shift, unsigned short* a_reg)
{
	DRV_DBG("getFlashReg() shift = %d",shift);
	int err = 0;
	if (m_fdSTROBE < 0)
	{
	    DRV_DBG(" [getFlashReg] m_fdSTROBE < 0\n");
	    return StrobeDrv::STROBE_UNKNOWN_ERROR;
	}
	/*
	if(shift==0)
		err = ioctl(m_fdSTROBE,FLASHLIGHT_GET_REG0,a_reg);
	else if(shift==1)
		err = ioctl(m_fdSTROBE,FLASHLIGHT_GET_REG1,a_reg);
	else
		err = ioctl(m_fdSTROBE,FLASHLIGHT_GET_REG2,a_reg);
		*/
	if (err < 0)
	{
	    DRV_ERR("getFlashReg() err=%d\n", err);
	}
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::setFlashReg(int shift, unsigned short reg, unsigned short mask)
{
	DRV_DBG("setFlashSpReg() shift=%d, reg=%d, mak=0x%x",shift, reg, mask);
	int err = 0;
	if (m_fdSTROBE < 0)
	{
	    DRV_DBG("setFlashSpReg() m_fdSTROBE < 0\n");
	    return StrobeDrv::STROBE_UNKNOWN_ERROR;
	}
	DRV_DBG("FlashErr setFlashSpReg() code not ready, line=%d",__LINE__);
	if (err < 0)
	{
	    DRV_ERR("setFlashSpReg() err=%d\n", err);
	}
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::setTimeOutTime(int ms)
{
	DRV_DBG("setTimeOutTime(ms=%d)", ms);
	int err = 0;
	if (m_fdSTROBE < 0)
	{
	    DRV_DBG("setTimeOutTime() m_fdSTROBE < 0\n");
	    return StrobeDrv::STROBE_UNKNOWN_ERROR;
	}
	err = ioctl(m_fdSTROBE,FLASH_IOC_SET_TIME_OUT_TIME_MS,ms);
	if (err < 0)
	{
	    DRV_ERR("setTimeOutTime() err=%d\n", err);
	}
	return err;
}
int FlashlightDrv::getCoolDownTime(int* ms)
{
	DRV_DBG("getCoolDownTime()");
	int err=0;
	DRV_DBG("FlashErr setFlashSpReg() code not ready, line=%d",__LINE__);
	*ms = 0;
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::lockSensedV()
{
	DRV_DBG("lockSensedV()");
	DRV_DBG("FlashErr lockSensedV() code not ready, line=%d",__LINE__);
	int err=0;
	return err;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int FlashlightDrv::unlockSensedV()
{
	DRV_DBG("unlockSensedV()");
	DRV_DBG("FlashErr unlockSensedV() code not ready, line=%d",__LINE__);
	int err=0;
	return err;
}
int FlashlightDrv::mapDutyStep(int peakI, int aveI, int* duty, int* step)
{
	DRV_DBG("mapDutyStep()");
	DRV_DBG("FlashErr mapDutyStep() code not ready, line=%d",__LINE__);
	int err=0;
	return err;
}
StrobeDrv::FLASHLIGHT_TYPE_ENUM FlashlightDrv::getFlashlightType() const
{
	DRV_DBG("getFlashlightType line=%d",__LINE__);
    int err = 0;

    if (m_fdSTROBE < 0)
    {
        DRV_DBG(" [getFlashlightType] m_fdSTROBE < 0\n");
        return StrobeDrv::FLASHLIGHT_NONE;
    }

    DRV_DBG("[getFlashlightType] m_flashType=%d\n",m_flashType);
    err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_G_FLASHTYPE,&m_flashType);
    if (err < 0)
    {
        DRV_ERR("FLASHLIGHTIOC_G_FLASHTYPE error:%d\n",m_flashType);
        return StrobeDrv::FLASHLIGHT_NONE;;
    }
    return (StrobeDrv::FLASHLIGHT_TYPE_ENUM)m_flashType;
}


FlashlightDrv::FlashlightDrv()
	  : StrobeDrv()
    , m_fdSTROBE(-1)
    , m_flashType((int)StrobeDrv::FLASHLIGHT_NONE)
    , m_strobeMode(LIB3A_AE_STROBE_MODE_UNSUPPORTED)
    , mUsers(0)
{
    DRV_DBG("FlashlightDrv()\n");
}


FlashlightDrv::~FlashlightDrv()
{
	DRV_DBG("FlashlightDrv line=%d",__LINE__);

}


int FlashlightDrv::init(unsigned long sensorDev)
{
	DRV_DBG("init line=%d",__LINE__);
    int err = 0;
    unsigned long flashlightIdx = 0; //default dummy driver

    //MHAL_LOG("[halSTROBEInit]: %s \n\n", __TIME__);
    DRV_DBG("[init] mUsers = %d\n", mUsers);
    Mutex::Autolock lock(mLock);
    if (mUsers == 0)
    {
        if (m_fdSTROBE == -1)
        {
        	 int ta;
        	 int tb;
        		ta = getMs();

            m_fdSTROBE = open(STROBE_DEV_NAME, O_RDWR);

            tb = getMs();



            DRV_DBG("[init] m_fdSTROBE = %d t=%d\n", m_fdSTROBE,tb-ta);

            if (m_fdSTROBE < 0)
            {
                DRV_ERR("error opening %s: %s", STROBE_DEV_NAME, strerror(errno));
                 return StrobeDrv::STROBE_UNKNOWN_ERROR;
            }

            //set flashlight driver
            DRV_DBG("[init] sensorDev = %ld\n", sensorDev);
            //TODO: support sub flashlight
            if (1 == sensorDev) //main sensor
            {
                flashlightIdx = 1; //custom driver
            }
            err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_X_SET_DRIVER,flashlightIdx);
            if (err < 0)
            {
                DRV_ERR("FLASHLIGHTIOC_X_SET_DRIVER error\n");
                return err ;
            }
        }
    }
    android_atomic_inc(&mUsers);
    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::uninit()
{
	DRV_DBG("uninit line=%d",__LINE__);
    //MHAL_LOG("[halSTROBEUninit] \n");
    DRV_DBG("[uninit] mUsers = %d\n", mUsers);

    Mutex::Autolock lock(mLock);

    if (mUsers == 0)
    {
        DRV_DBG("[uninit] mUsers = %d\n", mUsers);
    }

     if (mUsers == 1)
     {
        if (m_fdSTROBE > 0)
        {
            close(m_fdSTROBE);
        }
        m_fdSTROBE = -1;
    }

    android_atomic_dec(&mUsers);

    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::setFire(unsigned long a_fire)
{
	DRV_DBG("setFire line=%d",__LINE__);
    int err = 0;
    unsigned long  fire = 0;

    if (m_fdSTROBE < 0)
    {
        DRV_DBG(" [setFire] m_fdSTROBE < 0\n");
        return StrobeDrv::STROBE_UNKNOWN_ERROR;
    }

    DRV_DBG("[FlashlightDrv] setFire = %lu line=%d",a_fire,__LINE__);
    fire = (0 == a_fire) ? 0 : 1;
    err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_T_ENABLE,fire);

    if (err < 0)
    {
        //DRV_ERR("FLASHLIGHTIOC_T_ENABLE error\n");
        DRV_DBG("[FlashlightDrv] setFire ERR line=%d",__LINE__);
        return err ;
    }

    DRV_DBG("[FlashlightDrv] setFire OK line=%d",__LINE__);

    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::setLevel(unsigned long  a_level)
{
	DRV_DBG("setLevel line=%d",__LINE__);
    int err = 0;

    if (m_fdSTROBE < 0)
    {
        DRV_DBG(" [setLevel] m_fdSTROBE < 0\n");
        return StrobeDrv::STROBE_UNKNOWN_ERROR;
    }

    DRV_DBG("[FlashlightDrv] setLevel = %lu\n",a_level);
    err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_T_LEVEL,a_level);

    DRV_DBG("[FlashlightDrv] setLevel err = %d, line=%d\n",err, __LINE__);


    if (err < 0)
    {
        DRV_ERR("FLASHLIGHTIOC_T_ENABLE error;%ld\n",a_level);
        return err;
    }

    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::setTimeus(unsigned long a_timeus)
{
	DRV_DBG("setTimeus line=%d",__LINE__);
    int err = 0;
    if (m_fdSTROBE < 0)
    {
        DRV_DBG(" [setTimeus] m_fdSTROBE < 0\n");
        return StrobeDrv::STROBE_UNKNOWN_ERROR;
    }

    err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_T_FLASHTIME,a_timeus);
    if (err < 0)
    {
        DRV_ERR("FLASHLIGHTIOC_T_ENABLE error\n");
        return err;
    }

    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::setStartTimeus(unsigned long a_timeus)
{
	DRV_DBG("setStartTimeus line=%d",__LINE__);
    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::setState(unsigned long  a_state)
{
	DRV_DBG("setState line=%d",__LINE__);
    int err = 0;
    unsigned long strobeState = 0;

    if (m_fdSTROBE < 0)
    {
        DRV_DBG(" [setState] m_fdSTROBE < 0\n");
        return 0;
    }

    strobeState = (0 == a_state) ? (unsigned long)FLASHLIGHTDRV_STATE_PREVIEW : (unsigned long)FLASHLIGHTDRV_STATE_STILL;
    err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_T_STATE,strobeState);
    if (err < 0)
    {
        DRV_ERR("FLASHLIGHTIOC_T_STATE error\n");
        return err;
    }

    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::setFlashlightModeConf(unsigned long a_strobeMode)
{
	DRV_DBG("setFlashlightModeConf line=%d",__LINE__);
    int err = StrobeDrv::STROBE_NO_ERROR;
    static bool torchOn = false;    // cotta : added for 555 (torch) mode

    //cotta : modified for 555 (torch) mode
    if (StrobeDrv::FLASHLIGHT_NONE == m_flashType)
    {
        if(a_strobeMode != 555)
        {
            if(torchOn == true)
            {
                DRV_DBG("[setFlashlightModeConf] Torch mode off\n");
                torchOn = false;
                err = FlashlightDrv::setFire(0);
                return err;
            }
            else
            {
                DRV_DBG("[setFlashlightModeConf] FLASHLIGHT_NONE\n");
                return StrobeDrv::STROBE_NO_ERROR;
            }
        }
    }

    m_strobeMode = a_strobeMode;

    DRV_DBG("[setFlashlightModeConf] a_strobeMode: %d \n",m_strobeMode);

    switch(m_strobeMode)
    {
        case LIB3A_AE_STROBE_MODE_AUTO:
        //case LIB3A_AE_STROBE_MODE_SLOWSYNC:
            break;
        case LIB3A_AE_STROBE_MODE_FORCE_ON:
                //err = FlashlightDrv::setState(1); //still capture state
                //err = FlashlightDrv::setLevel(31); //max level
            if (StrobeDrv::FLASHLIGHT_LED_TORCH == m_flashType)
            {
                err = FlashlightDrv::setFire(1);
            }
            break;

        //FIXME: create an official control path for torch light
        case 555://LIB3A_AE_STROBE_MODE_FORCE_TORCH:
            err = FlashlightDrv::setLevel(1);
            err = FlashlightDrv::setFire(1);
            torchOn = true; //cotta : added for 555 (torch) mode
            break;

        case LIB3A_AE_STROBE_MODE_REDEYE:
            break;
        case LIB3A_AE_STROBE_MODE_FORCE_OFF:
        default:
            err = FlashlightDrv::setFire(0);

            if(torchOn == true) //cotta : added for 555 (torch) mode
            {
                torchOn = false;
            }

            break;
    }

    return err;
}


int FlashlightDrv::setCaptureFlashlightConf(unsigned long a_strobeWidth)
{
	DRV_DBG("setCaptureFlashlightConf line=%d",__LINE__);
    int err = StrobeDrv::STROBE_NO_ERROR;

    DRV_DBG("[setCaptureFlashlightConf] m_flashType = %d, width=%lu\n",m_flashType,a_strobeWidth);

    if (StrobeDrv::FLASHLIGHT_LED_PEAK == m_flashType)
    {
        DRV_DBG("[setCaptureFlashlightConf] FLASHLIGHT_LED_PEAK\n");
        err = FlashlightDrv::setState(1); //still capture state
        if (err < 0)
        {
            DRV_DBG("[setCaptureFlashlightConf] setState fail\n");
        }
        err = FlashlightDrv::setLevel(a_strobeWidth);
        if (err < 0)
        {
            DRV_DBG("[setCaptureFlashlightConf] setLevel fail\n");
        }
    }
    else if (StrobeDrv::FLASHLIGHT_LED_CONSTANT == m_flashType && 0 != a_strobeWidth )
             /*LIB3A_AE_STROBE_MODE_FORCE_ON == m_strobeMode) */
    {
        err = FlashlightDrv::setState(1); //still capture state
        if (err < 0)
        {
            DRV_DBG("[setCaptureFlashlightConf] setState fail\n");
        }

        err = FlashlightDrv::setLevel(a_strobeWidth); //set level
        if (err < 0)
        {
            DRV_DBG("[setCaptureFlashlightConf] setLevel fail\n");
        }

        err = FlashlightDrv::setFire(1);
        if (err < 0)
        {
            DRV_DBG("[setCaptureFlashlightConf] setFire fail\n");
        }
    }
    else
    {
        DRV_DBG("[setCaptureFlashlightConf] No config\n");
    }

    return err;
}


int FlashlightDrv::setCaptureDelay(unsigned int value)
{
	DRV_DBG("setCaptureDelay line=%d",__LINE__);
    int err = 0;

    if (m_fdSTROBE < 0)
    {
        DRV_DBG(" [setCaptureDelay] m_fdSTROBE < 0\n");
        return 0;
    }

    DRV_DBG(" [setCaptureDelay] set capture delay : %u\n",value);

    err = ioctl(m_fdSTROBE,FLASHLIGHTIOC_T_DELAY,value);
    if (err < 0)
    {
        DRV_ERR("FLASHLIGHTIOC_T_DELAY error : %u\n",value);
        return err;
    }

    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::getStrobeWDTValue(unsigned int *pValue)
{
	DRV_DBG("getStrobeWDTValue line=%d",__LINE__);
    *pValue = FLASH_LIGHT_WDT_TIMEOUT_MS;

    DRV_DBG("[getStrobeWDTValue] WDT value = %u\n",*pValue);
    return StrobeDrv::STROBE_NO_ERROR;
}


int FlashlightDrv::sendCommand(unsigned int cmd, unsigned int pArg1, unsigned int *pArg2, unsigned int *pArg3)
{
	DRV_DBG("sendCommand line=%d",__LINE__);
    int err = 0;

    switch(cmd)
    {
        case CMD_STROBE_SET_CAP_DELAY :
            err = setCaptureDelay(pArg1);   // set capture delay to strobe kernel driver
            break;
        case CMD_STROBE_GET_WDT_VALUE :
            err = getStrobeWDTValue(pArg2); // get WDT value
            break;
        default :
            DRV_ERR("[strobe sendCommand] no commadn support\n");
            return StrobeDrv::STROBE_UNKNOWN_ERROR;
    }

    if (err < 0)
    {
        DRV_ERR("[strobe sendCommand] Err-ctrlCode(%x)\n",err);
        return err;
    }

    return err;
}



