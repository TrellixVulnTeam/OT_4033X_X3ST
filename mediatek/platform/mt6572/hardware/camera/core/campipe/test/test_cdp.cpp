
///////////////////////////////////////////////////////////////////////////////
// No Warranty
// Except as may be otherwise agreed to in writing, no warranties of any
// kind, whether express or implied, are given by MTK with respect to any MTK
// Deliverables or any use thereof, and MTK Deliverables are provided on an
// "AS IS" basis.  MTK hereby expressly disclaims all such warranties,
// including any implied warranties of merchantability, non-infringement and
// fitness for a particular purpose and any warranties arising out of course
// of performance, course of dealing or usage of trade.  Parties further
// acknowledge that Company may, either presently and/or in the future,
// instruct MTK to assist it in the development and the implementation, in
// accordance with Company's designs, of certain softwares relating to
// Company's product(s) (the "Services").  Except as may be otherwise agreed
// to in writing, no warranties of any kind, whether express or implied, are
// given by MTK with respect to the Services provided, and the Services are
// provided on an "AS IS" basis.  Company further acknowledges that the
// Services may contain errors, that testing is important and Company is
// solely responsible for fully testing the Services and/or derivatives
// thereof before they are used, sublicensed or distributed.  Should there be
// any third party action brought against MTK, arising out of or relating to
// the Services, Company agree to fully indemnify and hold MTK harmless.
// If the parties mutually agree to enter into or continue a business
// relationship or other arrangement, the terms and conditions set forth
// hereunder shall remain effective and, unless explicitly stated otherwise,
// shall prevail in the event of a conflict in the terms in any agreements
// entered into between the parties.
////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2008, MediaTek Inc.
// All rights reserved.
//
// Unauthorized use, practice, perform, copy, distribution, reproduction,
// or disclosure of this information in whole or in part is prohibited.
////////////////////////////////////////////////////////////////////////////////
// AcdkCLITest.cpp  $Revision$
////////////////////////////////////////////////////////////////////////////////

//! \file  AcdkCLITest.cpp
//! \brief
 
#define LOG_TAG "CamPipeTest"

#include <vector>

using namespace std;

#include <linux/cache.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
//
#include <errno.h>
#include <fcntl.h>

#include <common/CamTypes.h>
#include <common/hw/hwstddef.h>
//#include <campipe/_buffer.h>

#include <campipe/IPipe.h>
#include <campipe/ICdpPipe.h>

//
#include <drv/sensor_hal.h>
#include <drv/imem_drv.h>

using namespace NSCamPipe; 


#include <inc/common/CamLog.h>
#define MY_LOGV(fmt, arg...)    CAM_LOGV("[%s] "fmt, __FUNCTION__, ##arg)
#define MY_LOGD(fmt, arg...)    CAM_LOGD("[%s] "fmt, __FUNCTION__, ##arg)
#define MY_LOGI(fmt, arg...)    CAM_LOGI("[%s] "fmt, __FUNCTION__, ##arg)
#define MY_LOGW(fmt, arg...)    CAM_LOGW("[%s] "fmt, __FUNCTION__, ##arg)
#define MY_LOGE(fmt, arg...)    CAM_LOGE("[%s] "fmt, __FUNCTION__, ##arg)

#define BUF_NUM          3
#define PREVIEW_WIDTH    640
#define PREVIEW_HEIGHT   480 

static  IMemDrv *g_pIMemDrv;

static void allocMem(IMEM_BUF_INFO &memBuf) 
{
    if (g_pIMemDrv->allocVirtBuf(&memBuf)) {
        MY_LOGE("g_pIMemDrv->allocVirtBuf() error");
    }
    memset((void*)memBuf.virtAddr, 0 , memBuf.size);
    if (g_pIMemDrv->mapPhyAddr(&memBuf)) {
        MY_LOGE("mpIMemDrv->mapPhyAddr() error");
    }
}

static void deallocMem(IMEM_BUF_INFO &memBuf)
{
    if (g_pIMemDrv->unmapPhyAddr(&memBuf)) {
        MY_LOGE("m_pIMemDrv->unmapPhyAddr() error");
    }

    if (g_pIMemDrv->freeVirtBuf(&memBuf)) {
        MY_LOGE("m_pIMemDrv->freeVirtBuf() error");
    }        
}


static bool
saveBufToFile(char const*const fname, MUINT8 *const buf, MUINT32 const size)
{
    int nw, cnt = 0;
    uint32_t written = 0;

    MY_LOGD("(name, buf, size) = (%s, %x, %d)", fname, buf, size); 
    MY_LOGD("opening file [%s]\n", fname);
    int fd = ::open(fname, O_RDWR | O_CREAT, S_IRWXU);
    if (fd < 0) {
        MY_LOGE("failed to create file [%s]: %s", fname, ::strerror(errno));
        return false;
    }

    MY_LOGD("writing %d bytes to file [%s]\n", size, fname);
    while (written < size) {
        nw = ::write(fd,
                     buf + written,
                     size - written);
        if (nw < 0) {
            MY_LOGE("failed to write to file [%s]: %s", fname, ::strerror(errno));
            break;
        }
        written += nw;
        cnt++;
    }
    MY_LOGD("done writing %d bytes to file [%s] in %d passes\n", size, fname, cnt);
    ::close(fd);
    return true; 
}


static uint32_t
loadFileToBuf(char const*const fname, uint8_t*const buf, uint32_t size)
{
    int nr, cnt = 0;
    uint32_t readCnt = 0;

    MY_LOGD("opening file [%s]\n", fname);
    int fd = ::open(fname, O_RDONLY);
    if (fd < 0) {
        MY_LOGE("failed to create file [%s]: %s", fname, strerror(errno));
        return readCnt;
    }
    //
    if (size == 0) {
        size = ::lseek(fd, 0, SEEK_END);
        ::lseek(fd, 0, SEEK_SET);
    }
    //
    MY_LOGD("read %d bytes from file [%s]\n", size, fname);
    while (readCnt < size) {
        nr = ::read(fd,
                    buf + readCnt,
                    size - readCnt);
        if (nr < 0) {
            MY_LOGE("failed to read from file [%s]: %s",
                        fname, strerror(errno));
            break;
        }
        readCnt += nr;
        cnt++;
    }
    MY_LOGD("done reading %d bytes to file [%s] in %d passes\n", size, fname, cnt);
    ::close(fd);

    return readCnt;
}

int main_testCDP(IMEM_BUF_INFO rInMem, int width, int height, int format) 
{
    MY_LOGD("+"); 
    // (1). Create Instance 
    ICdpPipe    *pCdpPipe = ICdpPipe::createInstance(eSWScenarioID_CAPTURE_NORMAL, eScenarioFmt_RAW); 
    if (pCdpPipe != NULL) 
    {
        MY_LOGD("Pipe (Name, ID, IDName) = (%s, %d)", pCdpPipe->getPipeName(), pCdpPipe->getPipeId()); 
    }
    
    // (2). Query port property
    vector<PortProperty> rInPortProperty; 
    vector<PortProperty> rOutPortProperty;     
    if (pCdpPipe->queryPipeProperty(rInPortProperty,rOutPortProperty))
    {
        MY_LOGD("Port Property (IN, OUT): (%d, %d)", rInPortProperty.size(), rOutPortProperty.size()); 
        for (MUINT32 i = 0; i < rInPortProperty.size(); i++)
        {
            MY_LOGD("IN: (type,index,inout) = (%d, %d, %d)", rInPortProperty.at(i).type, rInPortProperty.at(i).index, rInPortProperty.at(i).inout); 
            MY_LOGD("IN: (fmt, rot, flip) = (%d, %d, %d)", rInPortProperty.at(i).u4SupportFmt,
                                         rInPortProperty.at(i).fgIsSupportRotate, rInPortProperty.at(i).fgIsSupportFlip); 
        }       
        for (MUINT32 i = 0; i < rOutPortProperty.size(); i++)
        {
            MY_LOGD("OUT: (type,index,inout) = (%d, %d, %d)", rOutPortProperty.at(i).type, rOutPortProperty.at(i).index, rOutPortProperty.at(i).inout); 
            MY_LOGD("OUT: (fmt, rot, flip) = (%d, %d, %d)", rOutPortProperty.at(i).u4SupportFmt,
                                         rOutPortProperty.at(i).fgIsSupportRotate, rOutPortProperty.at(i).fgIsSupportFlip); 
        } 
    }    

    // (3). init 
    pCdpPipe->init(); 

    // (4). setCallback
    pCdpPipe->setCallbacks(NULL, NULL, NULL); 

    // (5). Config pipe 
    // 
    MUINT32 u4YuvStride[3] = {width, 0, 0};
    MemoryInPortInfo rMemInPort(ImgInfo(eImgFmt_YUY2, width, height), 
                                0, u4YuvStride, Rect(0, 0, width, height)); 
    //
    MemoryOutPortInfo rDispPort(ImgInfo(eImgFmt_I420, width, height), 
                               u4YuvStride, 0, 0); 
    MUINT32 u4VdoStride[3] = {320, 0, 0}; 
    MemoryOutPortInfo rVdoPort(ImgInfo(eImgFmt_I420, 320, 240), 
                               u4VdoStride, 0, 0);   
    rVdoPort.index = 1;   
    //
    vector<PortInfo const*> vCdpInPorts;  
    vector<PortInfo const*> vCdpOutPorts; 
    //
    vCdpInPorts.push_back(&rMemInPort); 
    vCdpOutPorts.push_back(&rDispPort); 
    vCdpOutPorts.push_back(&rVdoPort); 
    //
    pCdpPipe->configPipe(vCdpInPorts, vCdpOutPorts); 

    // (6). Enqueue, In buf
    // 
    QBufInfo rInBuf; 
    rInBuf.vBufInfo.clear(); 
    BufInfo rBufInfo(rInMem.size, rInMem.virtAddr, rInMem.phyAddr, rInMem.memID);  
    rInBuf.vBufInfo.push_back(rBufInfo); 
    pCdpPipe->enqueBuf(PortID(EPortType_MemoryIn, 0, 0), rInBuf); 

    // (6.1) Enqueue, Out Buf
    // 
    IMEM_BUF_INFO rDispMem; 
    rDispMem.size = ((width * height * 3/2) + L1_CACHE_BYTES-1) & ~(L1_CACHE_BYTES-1);     
    allocMem(rDispMem);     
    // 
    QBufInfo rOutBuf; 
    rOutBuf.vBufInfo.clear(); 
    rBufInfo.u4BufSize = rDispMem.size; 
    rBufInfo.u4BufVA = rDispMem.virtAddr; 
    rBufInfo.u4BufPA = rDispMem.phyAddr; 
    rBufInfo.i4MemID = rDispMem.memID; 
    rOutBuf.vBufInfo.push_back(rBufInfo); 
    pCdpPipe->enqueBuf(PortID(EPortType_MemoryOut, 0, 1), rOutBuf); 

    //
    IMEM_BUF_INFO rVdoMem; 
    rVdoMem.size = ((320 * 240 * 3/2)+ L1_CACHE_BYTES - 1) & ~(L1_CACHE_BYTES - 1); 
    allocMem(rVdoMem);    
    //
    rOutBuf.vBufInfo.at(0).u4BufSize = rVdoMem.size; 
    rOutBuf.vBufInfo.at(0).u4BufVA = rVdoMem.virtAddr; 
    rOutBuf.vBufInfo.at(0).u4BufPA = rVdoMem.phyAddr; 
    rOutBuf.vBufInfo.at(0).i4MemID = rVdoMem.memID; 
    pCdpPipe->enqueBuf(PortID(EPortType_MemoryOut, 1, 1), rOutBuf); 


    // (7). start
    pCdpPipe->start(); 

    // (8). Dequeue
    QTimeStampBufInfo rQDispOutBuf;         
    QTimeStampBufInfo rQVdoOutBuf; 
    QTimeStampBufInfo rQInBuf; 
    pCdpPipe->dequeBuf(PortID(EPortType_MemoryOut, 0, 1), rQDispOutBuf); 
    pCdpPipe->dequeBuf(PortID(EPortType_MemoryOut, 1, 1), rQVdoOutBuf); 
    pCdpPipe->dequeBuf(PortID(EPortType_MemoryIn, 0, 0), rQInBuf);  

 
    // (9). Stop 
    pCdpPipe->stop();
    // (10). uninit 
    pCdpPipe->uninit(); 
    // (11). destory instance 
    pCdpPipe->destroyInstance(); 

    char filename[256]; 
    sprintf(filename, "/data/disp%dx%d.yuv", width, height);     
    saveBufToFile(filename, reinterpret_cast<MUINT8*>(rDispMem.virtAddr), width * height * 2); 
    deallocMem(rDispMem); 
    sprintf(filename, "/data/vdo%dx%d.yuv", 320, 240);     
    saveBufToFile(filename, reinterpret_cast<MUINT8*>(rVdoMem.virtAddr), 320 * 240 * 2); 
    deallocMem(rVdoMem); 

    //
    return 0; 
}


int main_cdp(int argc, char** argv)
{
    int ret = 0; 
    char *filename; 
    int width = 0, height = 0; 
    int format = 0; 
    IMEM_BUF_INFO rInMem; 
    //
    if (argc != 5) 
    {
        printf("Usage: campipetest <filename> <width> <height> <format>\n"); 
        goto EXIT; 
    }
 
    filename = argv[1]; 
    width = atoi(argv[2]); 
    height = atoi(argv[3]); 
    format = atoi(argv[4]); 

    
    //
    g_pIMemDrv =  IMemDrv::createInstance(); 
    if (NULL == g_pIMemDrv)
    {
        MY_LOGE("g_pIMemDrv is NULL"); 
        return 0; 
    }
    g_pIMemDrv->init();

    // 
    rInMem.size = (width * height * 2 + L1_CACHE_BYTES-1) & ~(L1_CACHE_BYTES-1);     
    allocMem(rInMem); 

    if (width > 5000 || width < 0)
    {
        width = 0; 
    }
    if (height > 5000 || height < 0) 
    {
        height = 0; 
    }
    loadFileToBuf(filename, reinterpret_cast<MUINT8*>(rInMem.virtAddr), width * height * 2), 
    main_testCDP(rInMem, width, height, format); 
 
    deallocMem(rInMem); 

    g_pIMemDrv->uninit();
    //
    g_pIMemDrv->destroyInstance(); 

EXIT:
    printf("press any key to exit \n"); 
    getchar(); 

    return ret; 
}