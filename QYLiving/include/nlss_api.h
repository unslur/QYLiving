/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _NLSS_API_H_
#define _NLSS_API_H_

#include "nlss_childvideo_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

/*******************************�豸����API����ȡ�ɹ��ɼ���Դ�б�*************************************/
/**
*  ��ȡ�ɲɼ�Ӧ��ͼ���app����
*
*  @param  piAppWindNum: �ɲɼ�ͼ���app���� ����
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET Nlss_GetAvailableAppWindNum(int *piAppWindNum);

/**
*  ��ȡ�ɲɼ�ͼ���app�б���Ϣ
*
*  @param  pLSAppWindTitles: �ɲɼ�ͼ���app��Ϣ ����
*  @param  iMaxNum��pLSAppWindTitles������� ���
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET Nlss_GetAvailableAppWind(NLSS_OUT ST_NLSS_INDEVICE_INF *pLSAppWindTitles, int iMaxNum);

/**
*  ��ȡ���ö�ý���豸�б����
*
*  @param  iVideoDeviceNum: ��Ƶ�豸���� ����
*  @param  iAudioDeviceNum����Ƶ�豸���� ����
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET  Nlss_GetFreeDevicesNum(NLSS_OUT int *iVideoDeviceNum, NLSS_OUT int *iAudioDeviceNum);
/**
*  ��ȡ���ö�ý���豸�б����ƣ���ʱֻ֧��DShow�ɼ�����Ƶ
*
*  @param  pLSVideoDevices: ��Ƶ�豸��Ϣ ����
*  @param  iMaxVideoDevicesNum��pstVideoDevices������� ���
*  @param  pLSAudioDevices����Ƶ�豸��Ϣ ����
*  @param  iMaxAudioDevicesNum��pstAudioDevices������� ���
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET  Nlss_GetFreeDeviceInf(NLSS_OUT ST_NLSS_INDEVICE_INF *pstVideoDevices, int iMaxVideoDevicesNum, NLSS_OUT ST_NLSS_INDEVICE_INF* pstAudioDevices, int iMaxAudioDevicesNum);


/*******************************��ʼ���Ͳ�������API*************************************************************/
/**
*  ����ֱ������ʵ������������ֻ�������һ�������ֱ������ֻ����һ��
*
*  @param  paWorkPath:    NLSS workĿ¼(Ҫ��UTF-8����)�����workĿ¼���涯̬��������Ҫ��dll��
*                         �������Ϊ�գ����ڵ�ǰ����Ŀ¼���� �������������顣
*  @param  paCachePath:   NLSS cacheĿ¼�����ڸ�Ŀ¼����������־,��Ҫ���ļ�������д��Ȩ��
*                         �������Ϊ�գ����ڵ�ǰ����Ŀ¼���� �������������顣
*  @param  phNLSService: ֱ������ʵ�� ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_Create(const char *paWorkPath, const char *paCachePath, NLSS_OUT _HNLSSERVICE *phNLSService);

/**
*  ��ȡsdk�汾��
*
*  @param  ppaVersion: �汾�ţ���Ҫ����free�����ͷţ�����
*
*  @return ��
*/
EXPORTS_API   void     Nlss_GetSDKVersion(NLSS_OUT char **ppaVersion);

/**
*  ����ֱ������ʵ�������Գ����˳�������
*
*  @param  hNLSService: ֱ������ʵ�������
*/
EXPORTS_API  void       Nlss_Destroy(_HNLSSERVICE hNLSService);

/**
*  ��ȡֱ��Ĭ�ϲ���
*  @param  hNLSService: ֱ������ʵ�������
*  @param  enVideoInType�� ������Ƶ�����ͣ����
*  @param  enAudioInType�� ������Ƶ�����ͣ����
*  @param  pstParam: ֱ������������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_GetDefaultParam(_HNLSSERVICE hNLSService, NLSS_OUT ST_NLSS_PARAM *pstParam);
/**
*  ��ʼ��ֱ������
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pstParam: ֱ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_InitParam(_HNLSSERVICE hNLSService, ST_NLSS_PARAM *pstParam);

/**
*  ������Ƶˮӡ��Ĭ������ˮӡ
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pstWaterParam: ˮӡ�����ṹ��.
*/
EXPORTS_API   void     Nlss_SetVideoWaterMark(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_WATER_PARAM *pstWaterParam);

/**
*  ������Ƶ��ͼ�ĵĻص�
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pFunVideoSamplerCB ��Ƶ��ͼͼ��ص�
*/
EXPORTS_API void Nlss_SetVideoSamplerCB(_HNLSSERVICE hNLSService, PFN_NLSS_MERGED_VIDEO_SAMPLER_CB pFunVideoSamplerCB);

/**
*  ����ֱ��������״̬�ص�
*
*  @param  hNLSService: ֱ������ʵ��
*  @param  pFunStatusNty ֱ��״̬֪ͨ����
*/
EXPORTS_API   void     Nlss_SetStatusCB(_HNLSSERVICE hNLSService, PFN_NLSS_STATUS_NTY pFunStatusNty);

/**
*  ���Nlss_InitParam���õ�ֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pstParam: ֱ������
*
*  @return ��
*/
EXPORTS_API   void    Nlss_UninitParam(_HNLSSERVICE hNLSService);
/*******************************����|ֹͣ����API*************************************************************/
/**
*  ���������ڸ����ʼ��֮����Ԥ����ֱ������֮ǰ
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_Start(_HNLSSERVICE hNLSService);

/**
*  ֹͣ������Ԥ����ֱ��ֹ֮ͣ����Nlss_UninitParam֮ǰ
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API   void     Nlss_Stop(_HNLSSERVICE hNLSService);


/*******************************��ƵԤ��API*************************************************************/
/**
*  ����ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_StartVideoPreview(_HNLSSERVICE hNLSService);

/**
*  ��ͣ��ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void      Nlss_PauseVideoPreview(_HNLSSERVICE hNLSService);

/**
*  �ָ���ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void      Nlss_ResumeVideoPreview(_HNLSSERVICE hNLSService);

/**
*  ֹͣ��ƵԤ��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_StopVideoPreview(_HNLSSERVICE hNLSService);

/*******************************ֱ������API*************************************************************/

/**
*  ����ֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_StartLiveStream(_HNLSSERVICE hNLSService);

/**
*  ֱֹͣ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_StopLiveStream(_HNLSSERVICE hNLSService);

/**
*  ��ͣ��Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_PauseVideoLiveStream(_HNLSSERVICE hNLSService);
/**
*  �ָ���Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_ResumeVideoLiveStream(_HNLSSERVICE hNLSService);

/**
*  ������Ƶ����
*
*  @param  hNLSService: ֱ������ʵ������Σ� iRatio �ݶ�0-100��0��ʾ������100��ʾԭʼ����
*
*  @return ��
*/
EXPORTS_API   void     Nlss_SetAudioVolume(_HNLSSERVICE hNLSService, int iRatio);

/**
*  ��ͣ��Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_PauseAudioLiveStream(_HNLSSERVICE hNLSService);

/**
*  �ָ���Ƶֱ������
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_ResumeAudioLiveStream(_HNLSSERVICE hNLSService);


/**
*  ��ʼֱ��¼��
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pcRecordPath: ¼���ļ����λ�ü��ļ��������
*
*  @return ��
*/
EXPORTS_API  NLSS_RET   Nlss_StartRecord(_HNLSSERVICE hNLSService, char *pcRecordPath);

/**
*  ֱֹͣ��¼��
*
*  @param  hNLSService: ֱ������ʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_StopRecord(_HNLSSERVICE hNLSService);

/*******************************ֱ��������ʽ2 API: ����ֱ���ӿ�*************************************************************/
/**
*  �û�ָ����Ƶ���������ͽӿڣ�֧��ֱ����������Ƶ�����ز���
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pcAudioData: �û�ָ����Ƶ�����ݣ����
*  @param  iLen:        ��Ƶ�����ݳ��ȣ����
*  @param  iSampleRate: ��Ƶ�������ʣ�����֧���м�仯���
*
*  @return NLSS_RET     NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_SendCustomAudioData(_HNLSSERVICE hNLSService, char *pcAudioData, int iLen, int iSampleRate);

/**
*  ��ȡֱ������״̬��Ϣ
*
*  @param  hNLSService:   ֱ������ʵ�������
*  @param  pstatistics:  ֱ������ͳ����Ϣ������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET   Nlss_GetStaticInfo(_HNLSSERVICE hNLSService, NLSS_OUT ST_NLSS_STATS *pstStats);



#ifdef __cplusplus
}
#endif

#endif// _LS_MEDIACAPTURE_API_H_



