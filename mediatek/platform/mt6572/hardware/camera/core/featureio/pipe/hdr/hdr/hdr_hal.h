
#ifndef _HDR_HAL_H_
#define _HDR_HAL_H_

#include <utils/threads.h>	// For Mutex.
#include "hdr_hal_base.h"
#include "MTKHdr.h"	// For MTKHdr class, WEIGHT_TBL_INFO struct.
#include "MTKMav.h"	// For MTKMav class.


using namespace android;


#define EIS_WIDTH2	160
#define EIS_HEIGHT2	120




class HdrHal : public HdrHalBase
{
public:
    static HdrHalBase* getInstance();
    virtual MVOID destroyInstance();

private:
    HdrHal();
    virtual ~HdrHal();
    virtual MBOOL init(void *pInitInData);
    virtual MBOOL uninit();

public:
    virtual MBOOL Do_Normalization(void);
    virtual MBOOL Do_SE(HDR_PIPE_SE_INPUT_INFO& rHdrPipeSEInputInfo);
    virtual MBOOL Do_FeatureExtraction(HDR_PIPE_FEATURE_EXTRACT_INPUT_INFO& rHdrPipeFeatureExtractInputInfo);
    virtual MBOOL Do_Alignment(void);
    virtual MBOOL Do_Fusion(HDR_PIPE_WEIGHT_TBL_INFO** pprBlurredWeightMapInfo);
    virtual MBOOL WeightingMapInfoGet(HDR_PIPE_WEIGHT_TBL_INFO** pprWeightMapInfo);
    virtual MBOOL WeightingMapInfoSet(HDR_PIPE_SET_BMAP_INFO* pBmapInfo);
    virtual MBOOL ResultBufferSet(MUINT32 bufferAddr, MUINT32 bufferSize);
    virtual MBOOL HdrCroppedResultGet(HDR_PIPE_HDR_RESULT_STRUCT& rHdrCroppedResult);
	virtual MBOOL HdrSmallImgBufSet(HDR_PIPE_CONFIG_PARAM& rHdrPipeConfigParam);
	virtual MBOOL ConfigMavParam();
    virtual MBOOL HdrSettingClear(void);
    virtual MBOOL HdrWorkingBufSet(MUINT32 u4BufAddr, MUINT32 u4BufSize);
    virtual MUINT32 HdrWorkingBuffSizeGet(void);
    virtual MBOOL MavWorkingBuffSizeGet(MUINT32 ru4SmallImgWidth, MUINT32 ru4SmallImgHeight, MUINT32 *pMavWorkingBuffSize);
	virtual void QuerySmallImgResolution(MUINT32& ru4Width, MUINT32& ru4Height);
    virtual void QuerySEImgResolution(MUINT32& ru4Width, MUINT32& ru4Height);
    virtual MUINT32 SEImgBuffSizeGet(void);
    virtual void SaveHdrLog(MUINT32 u4RunningNumber);

private:

    volatile MINT32 mUsers;
    mutable Mutex mLock;


	MTKHdr* m_pHdrDrv;
	MTKMav* m_pMavDrv;


//    MTKHdr* m_pMTKHdrObj;

};

#endif	// _HDR_HAL_H_

