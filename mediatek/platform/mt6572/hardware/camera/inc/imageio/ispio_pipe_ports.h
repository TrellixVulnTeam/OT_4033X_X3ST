#ifndef _ISPIO_PIPE_PORTS_H_
#define _ISPIO_PIPE_PORTS_H_
//
#include "ispio_stddef.h"


namespace NSImageio {
namespace NSIspio   {
////////////////////////////////////////////////////////////////////////////////


enum EPortType
{
    EPortType_Memory,
    EPortType_Sensor,
    EPortType_GDMA,
    EPortType_VRZ_RDMA,
    EPortType_VID_RDMA,
    EPortType_DISP_RDMA,
};
enum EPortIndex
{
    EPortIndex_TG1I,
    EPortIndex_TG2I,
    EPortIndex_IMGI,
    EPortIndex_IMGCI,
    EPortIndex_NR3I,
    EPortIndex_FLKI,
    EPortIndex_LSCI,
    EPortIndex_LCEI,
    EPortIndex_VIPI,
    EPortIndex_VIP2I,
    EPortIndex_CQI,
    EPortIndex_TDRI,
    EPortIndex_IMGO,
    EPortIndex_IMG2O,
    EPortIndex_LCSO,
    EPortIndex_AAO,
    EPortIndex_NR3O,
    EPortIndex_ESFKO,
    EPortIndex_AFO,
    EPortIndex_EISO,
    EPortIndex_DISPO,
    EPortIndex_VIDO,
    EPortIndex_FDO
};
enum ESensorIndex
{
    ESensorIndex_NONE   = 0x00,
    ESensorIndex_MAIN   = 0x01,
    ESensorIndex_SUB    = 0x02,
    ESensorIndex_ATV    = 0x04,
    ESensorIndex_MAIN_2 = 0x08,
    ESensorIndex_MAIN_3D= 0x09,
};

enum EPipePass
{
    EPipePass_PASS2 = 0,
    EPipePass_PASS2B,
    EPipePass_PASS2C,
    EPipePass_PASS1_TG1,
    EPipePass_PASS1_TG2,

};
enum EPortDirection
{
    EPortDirection_In,
    EPortDirection_Out,
};

struct PortID
{
public:     //// fields.
    MUINT32     type    :   8;      //  EPortType
    MUINT32     index   :   8;      //  port index
    MUINT32     inout   :   1;      //  0:in/1:out
    MUINT32     pipePass;
    //
public:     //// constructors.
    PortID(
        EPortType const _eType     = EPortType_Memory,
        MUINT32 const _index       = 0,
        MUINT32 const _inout       = 0,
        MUINT32 const _pipePass= EPipePass_PASS2
    )
    {
        type        = _eType;
        index       = _index;
        inout       = _inout;
        pipePass    = _pipePass;
    }
    //
public:     //// operations.
    MUINT32 operator()() const
    {
        return  *reinterpret_cast<MUINT32 const*>(this);
    }
};


struct PortInfo : public ImgInfo, public PortID, public BufInfo, public RingInfo, public SegmentInfo
{
public:     //// constructors.
    PortInfo()
        : ImgInfo()
        , PortID()
    {
    }
    //
    PortInfo(ImgInfo const& _ImgInfo)
        : ImgInfo(_ImgInfo)
        , PortID()
    {
    }
    //
    PortInfo(PortID const& _PortID)
        : ImgInfo()
        , PortID(_PortID)
    {
    }
    //
    PortInfo(ImgInfo const& _ImgInfo, PortID const& _PortID)
        : ImgInfo(_ImgInfo)
        , PortID(_PortID)
    {
    }
    //
    PortInfo(ImgInfo const& _ImgInfo, PortID const& _PortID, BufInfo const& _BufInfo)
        : ImgInfo(_ImgInfo)
        , PortID(_PortID)
        ,BufInfo(_BufInfo)
    {
    }
    //
    PortInfo(ImgInfo const& _ImgInfo, PortID const& _PortID, BufInfo const& _BufInfo, RingInfo const& _RingInfo )
        : ImgInfo(_ImgInfo)
        , PortID(_PortID)
        , BufInfo(_BufInfo)
        , RingInfo(_RingInfo)
    {
    }
    //
    PortInfo(ImgInfo const& _ImgInfo, PortID const& _PortID, BufInfo const& _BufInfo, RingInfo const& _RingInfo, SegmentInfo const& _SegmentInfo )
        : ImgInfo(_ImgInfo)
        , PortID(_PortID)
        , BufInfo(_BufInfo)
        , RingInfo(_RingInfo)
        , SegmentInfo(_SegmentInfo)
    {
    }
    //

};


////////////////////////////////////////////////////////////////////////////////
};  //namespace NSIspio
};  //namespace NSImageio
#endif  //  _ISPIO_PIPE_PORTS_H_

