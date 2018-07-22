#ifndef _NLSS_TYPE_NEW_H_
#define _NLSS_TYPE_NEW_H_

#include "nlss_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

typedef  NLSS_MASK_HANDLE_TYPE(_HNLSSERVICE)  _HNLSSERVICE;

typedef  NLSS_MASK_HANDLE_TYPE(_HNLSSCHILDSERVICE)  _HNLSSCHILDSERVICE;

/**
*  ֱ������״̬
*/
typedef enum enum_NLSS_STATUS{
    EN_NLSS_STATUS_INIT,                      //!< ��ʼ״̬
    EN_NLSS_STATUS_START,                     //!< ֱ����ʼ״̬
    EN_NLSS_STATUS_ERR,                       //!< ֱ������
    EN_NLSS_STATUS_STOP                       //!< ֱ��ֹͣ
}EN_NLSS_STATUS;

/**
*  ֱ��������ƵԴģʽ������Ƶ��������
*/
typedef enum enum_NLSS_VIDEOIN_TYPE{
    EN_NLSS_VIDEOIN_NONE = 0,                  //!< ���ɼ���Ƶ
    EN_NLSS_VIDEOIN_CAMERA,                    //!< ����ͷģʽ
    EN_NLSS_VIDEOIN_FULLSCREEN,			       //!< ȫ��ģʽ
    EN_NLSS_VIDEOIN_RECTSCREEN,			       //!< �����������
    EN_NLSS_VIDEOIN_APP,					   //!< Ӧ�ó��򴰿ڽ���
    EN_NLSS_VIDEOIN_PNG,					   //!< PNGͼƬԴ��������Ϊ����
    EN_NLSS_VIDEOIN_RAWDATA,                   //!< ��Ƶ������ģʽ
}EN_NLSS_VIDEOIN_TYPE;

/**
*  ֱ��������ƵԴģʽ������Ƶ�����ɼ�Դ
*/
typedef enum enum_NLSS_AUDIOIN_TYPE{
    EN_NLSS_AUDIOIN_NONE = 0,                  //!< ���ɼ�����
    EN_NLSS_AUDIOIN_MIC,                       //!< ��˷�ģʽ
    EN_NLSS_AUDIOIN_SYS,                       //!< windowsϵͳ����
    EN_NLSS_AUDIOIN_RAWDATA                   //!< ��Ƶ��������ģʽ
}EN_NLSS_AUDIOIN_TYPE;

/**
*  ֱ����������ʽ��FLV��RTMP
*/
typedef enum enum_NLSS_OUTFORMAT{
    EN_NLSS_OUTFORMAT_FLV,                    //!< FLV���������
    EN_NLSS_OUTFORMAT_RTMP					//!< RTMP���������
}EN_NLSS_OUTFORMAT;

/**
*  ֱ�����������ݣ�����Ƶ��
*/
typedef enum enum_NLSS_OUTCONTENT{
    EN_NLSS_OUTCONTENT_AUDIO = 0x01,            //!< ������Ƶ��
    EN_NLSS_OUTCONTENT_VIDEO = 0x02,			//!< ������Ƶ��
    EN_NLSS_OUTCONTENT_AV = 0x03			    //!< ��������Ƶ��
}EN_NLSS_OUTCONTENT;

/**
*  ֱ����Ƶ�����ʽ
*/
typedef enum enum_NLSS_VIDEOOUT_CODEC{
    EN_NLSS_VIDEOOUT_CODEC_OPENH264,
    EN_NLSS_VIDEOOUT_CODEC_X264,
    EN_NLSS_VIDEOOUT_CODEC_VP9,
    EN_NLSS_VIDEOOUT_CODEC_HEVC
}EN_NLSS_VIDEOOUT_CODEC;

/**
*  ֱ����Ƶ�����ʽ
*/
typedef enum enum_NLSS_AUDIOOUT_CODEC{
    EN_NLSS_AUDIOOUT_CODEC_AAC,
    EN_NLSS_AUDIOOUT_CODEC_GIPS
}EN_NLSS_AUDIOOUT_CODEC;

/**
*  ֱ����Ƶ������
*/
typedef enum enum_NLSS_VIDEOQUALITY_LVL{
    EN_NLSS_VIDEOQUALITY_LOW,                    //!< ��Ƶ�ֱ��ʣ�����.
    EN_NLSS_VIDEOQUALITY_MIDDLE,                 //!< ��Ƶ�ֱ��ʣ�����.
    EN_NLSS_VIDEOQUALITY_HIGH,                   //!< ��Ƶ�ֱ��ʣ�����.
    EN_NLSS_VIDEOQUALITY_SUPER,                  //!< ��Ƶ�ֱ��ʣ�����.
    EN_NLSS_VIDEOQUALITY_INVALID                 //!< ��Ƶ�ֱ��ʣ���Чֵ
}EN_NLSS_VIDEOQUALITY_LVL;

/**
*  ����Ƶ��Ϊ�û��ɼ�ʱ����EN_NLSS_AUDIOIN_RAWDATAʱ���������Ƶ����ʽ
*/
typedef enum enum_NLSS_VIDEOIN_FMT{
    EN_NLSS_VIDEOIN_FMT_NV12 = 0,
    EN_NLSS_VIDEOIN_FMT_NV21,
    EN_NLSS_VIDEOIN_FMT_I420,
    EN_NLSS_VIDEOIN_FMT_BGRA32,
    EN_NLSS_VIDEOIN_FMT_ARGB32,
    EN_NLSS_VIDEOIN_FMT_YUY2,
    EN_NLSS_VIDEOIN_FMT_BGR24,
    EN_NLSS_VIDEOIN_FMT_BGR24FLIP,
    EN_NLSS_VIDEOIN_FMT_INVALID
}EN_NLSS_VIDEOIN_FMT;//custom videoin rawdata input source format

/**
*  ����Ƶ��Ϊ�û��ɼ�ʱ����EN_NLSS_AUDIOIN_RAWDATAʱ���������Ƶ����ʽ
*/
typedef enum enum_NLSS_AUDIOIN_FMT{
    EN_NLSS_AUDIOIN_FMT_NONE = -1,
    EN_NLSS_AUDIOIN_FMT_U8,          ///< unsigned 8 bits
    EN_NLSS_AUDIOIN_FMT_S16,         ///< signed 16 bits
    EN_NLSS_AUDIOIN_FMT_S32,         ///< signed 32 bits
    EN_NLSS_AUDIOIN_FMT_FLT,         ///< float
    EN_NLSS_AUDIOIN_FMT_DBL,         ///< double

    EN_NLSS_AUDIOIN_FMT_U8P,         ///< unsigned 8 bits, planar
    EN_NLSS_AUDIOIN_FMT_S16P,        ///< signed 16 bits, planar
    EN_NLSS_AUDIOIN_FMT_S32P,        ///< signed 32 bits, planar
    EN_NLSS_AUDIOIN_FMT_FLTP,        ///< float, planar
    EN_NLSS_AUDIOIN_FMT_DBLP,        ///< double, planar

    EN_NLSS_AUDIOIN_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
}EN_NLSS_AUDIOIN_FMT; //custom audioin rawdata input source format

/**
*  ֱ����ƵԴΪ����ͷģʽʱ������EN_NLSS_VIDEOIN_CAMERA���������
*/
typedef struct struct_NLSS_CAMERA_PARAM
{
    char               *paDevicePath;    //!< camera����.
    EN_NLSS_VIDEOQUALITY_LVL  enLvl;           //!< ��Ƶ�ֱ���
} ST_NLSS_CAMERA_PARAM;

typedef struct struct_NLSS_CAMERA_CAPTURE_PARAM
{
	int                 iWidth;       //!<�ɼ��Ŀ�
	int                 iHeight;      //!<�ɼ��ĸ�
	int                 iFps;         //!<�ɼ���֡��
	EN_NLSS_VIDEOIN_FMT enFmt;        //!<������ԴΪyuv����ʱ����Ƶ��ʽ
} ST_NLSS_CAMERA_CAPTURE_PARAM;

/**
*  ֱ����ƵԴΪ������Ļ����ģʽʱ������EN_NLSS_VIDEOIN_RECTSCREEN���������
*/
typedef struct struct_NLSS_RECTSCREEN_PARAM
{
    int iRectLeft;                            //!<��������������ģʽ�����ý�������ĵ�λ��
    int iRectRight;
    int iRectTop;
    int iRectBottom;
} ST_NLSS_RECTSCREEN_PARAM;


typedef struct struct_NLSS_APPVIDEO_PARAM
{
    char    *paAppPath;    //!< App����.
} ST_NLSS_APPVIDEO_PARAM;

typedef struct struct_NLSS_PNG_PARAM
{
    char    *paPngPath;    //!< png pathĿ¼.
} ST_NLSS_PNG_PARAM;

/**
*  ֱ����ƵԴΪ��Ƶ������ģʽʱ������EN_NLSS_VIDEOIN_RAWDATA���������
*/
typedef struct struct_NLSS_CUSTOMVIDEO_PARAM
{
    int                 iInWidth;            //!<����Դ�Ŀ�
    int                 iInHeight;           //!<����Դ�ĸ�
    EN_NLSS_VIDEOIN_FMT enVideoInFmt;        //!<������ԴΪyuv����ʱ����Ƶ��ʽ
} ST_NLSS_CUSTOMVIDEO_PARAM;

/**
*  ˮӡ����
*/
typedef struct stru_NLSS_VIDEO_WATER_PARAM{
    char                *pucFilePath;  //!<ˮӡ���ļ�·��
    unsigned int         uiStartx;     //!<ˮӡ����ʼX����
    unsigned int         uiStarty;     //!<ˮӡ����ʼY����
} ST_NLSS_VIDEO_WATER_PARAM;

/**
*  ֱ��������Ƶ�������
*/
typedef struct struct_NLSS_VIDEOOUT_PARAM
{
    int                           iOutWidth;           //!< ��Ƶ�������.Ҫ����4�ı���
    int                           iOutHeight;          //!< ��Ƶ�������.Ҫ����4�ı���
    int                           iOutFps;             //!< ��Ƶ��֡��.
    int                           iOutBitrate;         //!< ����.
    EN_NLSS_VIDEOOUT_CODEC        enOutCodec;          //!< ��Ƶ������.����ѡ��X264��Openh264
    bool                          bHardEncode;         //!< �Ƿ�ʹ����ƵӲ������
	bool                          bQosAutoChangeRatio; //!< �Ƿ��Զ������ֱ���                          
} ST_NLSS_VIDEOOUT_PARAM;

/**
*  ֱ��������Ƶ�������
*/
typedef struct struct_NLSS_VIDEOIN_PARAM
{
    EN_NLSS_VIDEOIN_TYPE          enInType;          //!< ��ƵԴ����
    int                           iCaptureFps;       //!< �ɼ�fps
    union
    {
        ST_NLSS_CAMERA_PARAM      stInCamera;
        ST_NLSS_RECTSCREEN_PARAM  stInRectScreen;
        ST_NLSS_APPVIDEO_PARAM    stInApp;
        ST_NLSS_CUSTOMVIDEO_PARAM stInCustomVideo;
        ST_NLSS_PNG_PARAM         stInPng;
    }u;
} ST_NLSS_VIDEOIN_PARAM;

/**
*  ֱ��������Ƶ�������
*/
typedef struct struct_NLSS_AUDIOOUT_PARAM
{
    EN_NLSS_AUDIOOUT_CODEC enOutcodec;            //!< ��Ƶ������.
    int                    iOutBitrate;           //!< ��Ƶ��������. �ο�ֵ��64000
    bool                   bHardEncode;           //!< �Ƿ�ʹ����ƵӲ������ 
} ST_NLSS_AUDIOOUT_PARAM;

/**
*  ֱ��������Ƶ�������
*/
typedef struct struct_NLSS_AUDIOIN_PARAM
{
    EN_NLSS_AUDIOIN_TYPE   enInType;              //!< ��Ƶ�����ɼ�Դ
    char                   *paaudioDeviceName;    //!< ����Ƶ�����ɼ�Դ��˷�ʱ��EN_NLSS_AUDIOIN_MIC�������ã���Ϊ��˷��豸���ƣ�ͨ��
    int                    iInSamplerate;         //!< ��Ƶ�������ɼ���. �ο�ֵ��44100
    int                    iInNumOfChannels;      //!< ��Ƶ�ɼ���ͨ��������������˫����. �ο�ֵ��1
    int                    iInFrameSize;          //!< ��Ƶ�ɼ���ÿ֡��С. �ο�ֵ��2048
    int                    iInBitsPerSample;      //!< ��Ƶ������λ��  
    EN_NLSS_AUDIOIN_FMT    enInFmt;               //!< ��Ƶ�����ʽ���ο�ֵ��EN_NLSS_AUDIOIN_FMT_S16
} ST_NLSS_AUDIOIN_PARAM;

/**
*  ֱ��������Ƶ����
*/
typedef struct struct_NLSS_AUDIO_PARAM
{
    ST_NLSS_AUDIOIN_PARAM  stIn;
    ST_NLSS_AUDIOOUT_PARAM stOut;
} ST_NLSS_AUDIO_PARAM;

/**
*  ֱ����������
*/
typedef struct struct_NLSS_PARAM
{
    EN_NLSS_OUTCONTENT        enOutContent;       //!< ���������ݣ�����Ƶ����Ƶ����Ƶ.
    EN_NLSS_OUTFORMAT         enOutFormat;        //!< �������ĸ�ʽ��FLV��RTMP.
    char                      *paOutUrl;          //!< ֱ����ַ����
    ST_NLSS_VIDEOOUT_PARAM    stVideoParam;       //!< ������Ƶ�����ز���.
    ST_NLSS_AUDIO_PARAM       stAudioParam;       //!< ������Ƶ��ز���.
} ST_NLSS_PARAM;

/**
* ��Ƶ��ͼ�Ľṹ�����
*/
typedef struct struct_NLSS_VIDEO_SAMPLER
{
    int           iWidth;                        //!< ��Ƶ��ͼͼ��Ŀ��.
    int           iHeight;                       //!< ��Ƶ��ͼͼ��ĸ߶�.
    int           iFormat;                       //!< ��Ƶ��ͼͼ��ĸ�ʽ.
    int           iDataSize;                     //!< ��Ƶ��ͼͼ������ݴ�С.
    unsigned char *puaData;                      //!< ��Ƶ��ͼͼ�������ָ��.
    int           *piRef;                        //!< ���˾�.
}ST_NLSS_VIDEO_SAMPLER;

/**
* �豸��Ϣ
*/
typedef struct struct_NLSS_INDEVICE_INF
{
    const char  *paPath;
    const char  *paFriendlyName;
}ST_NLSS_INDEVICE_INF;

typedef struct struct_NLSS_STATS{
	unsigned int uiVSendFrameRate;     //!< ��Ƶ����֡����Ϣ
	unsigned int uiVSendBitRate;	   //!< ��Ƶ����������Ϣ
	unsigned int uiVSendWidth;	       //!< ��Ƶ�����Ϣ
	unsigned int uiVSendHeight;	       //!< ��Ƶ�߶���Ϣ
	unsigned int uiVSetFrameRate;	   //!< ��Ƶ���õ�֡����Ϣ
	unsigned int uiVSetBitRate;	       //!< ��Ƶ���õ�������Ϣ
	unsigned int uiVSetWidth;		   //!< ��Ƶ���õĿ����Ϣ
	unsigned int uiVSetHeight;	       //!< ��Ƶ���õĸ߶���Ϣ
	unsigned int uiASendBitRate;       //!< ��Ƶ����������Ϣ
}ST_NLSS_STATS;

typedef enum enum_NLSS_ERRCODE{
    EN_NLSS_ERR_NO          = 0,           //!< �����룺��ȷ
    EN_NLSS_ERR_AUDIOINIT   = 2001,		   //!< �����룺��Ƶ��ʼ��
    EN_NLSS_ERR_AUDIOSTART  = 2002,		   //!< �����룺��Ƶ��ʼ����ʧ��

    EN_NLSS_ERR_VIDEOINIT   = 3001,		   //!< �����룺��Ƶ��ʼ��
    EN_NLSS_ERR_VIDEOSTART  = 3002,		   //!< �����룺��Ƶ��ʼ����ʧ��

    EN_NLSS_ERR_NETTIMEOUT  = 4001,		   //!< �����룺����
    EN_NLSS_ERR_URLINVALID  = 4002         //!< �����룺url��ַ��Ч
}EN_NLSS_ERRCODE;	

/**
*  @brief ֱ����������ص�����ֱ�������з�������֪ͨӦ�ò㣬Ӧ�ò��������Ӧ�Ĵ���
*
*  @param  hNLSService: ֱ������ʵ��
*  @param enStatus:  ֱ��״̬
*  @param enErrCode: ������
*/
typedef void(*PFN_NLSS_STATUS_NTY)(_HNLSSERVICE hNLSService, EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);

/**
*  @brief ��ȡ����һ֡�ϲ�����Ƶ��������Ƶ��ͼ��Ļص�
*
*  @param  hNLSService: ֱ������ʵ��
*  @param pstSampler ����һ֡�ϲ�����Ƶ��������Ƶ��ͼ�Ľṹ�����ָ��
*/
typedef void(*PFN_NLSS_MERGED_VIDEO_SAMPLER_CB)(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_SAMPLER *pstSampler);

/**
*  @brief ��ȡ��������Ƶ��ͼ��Ļص�
*
*  @param  hNLSSChild: ����Ƶ����
*  @param pstSampler ����һ֡��������Ƶ��ͼ�Ľṹ�����ָ��
*/
typedef void(*PFN_NLSS_CHILD_VIDEO_SAMPLER_CB)(_HNLSSCHILDSERVICE hNLSSChild, ST_NLSS_VIDEO_SAMPLER *pstSampler);


_HNLSSCHILDSERVICE Nlss_GetChildVideo(_HNLSSERVICE hNlssService, void *pNlssChildID);

#ifdef __cplusplus
}
#endif

#endif//_LS_MEDIALIVESTREAMING_H_
