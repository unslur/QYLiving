/*
*	Author       hzzhuling15
*	Date         2015/09/11
*	Copyright    Hangzhou, Netease Inc.
*	Brief        
*   Notice       
*/
#ifndef _NLSS_CHILDVIDEO_API_H_
#define _NLSS_CHILDVIDEO_API_H_

#include "nlss_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file */

/**
*  ��ȡ����ͷ�豸֧�ֵĲɼ�����
*
*  @param  pstCamera: ����ͷ�������
*  @param  pstCaptureParams: ����ͷ֧�ֵĲɼ��������飬����
*  @param  piNum: ����ͷ֧�ֵĲɼ������������������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API   NLSS_RET  Nlss_DeviceGetCamereCaptureInf(ST_NLSS_INDEVICE_INF *pstCamera, NLSS_OUT ST_NLSS_CAMERA_CAPTURE_PARAM **pstCaptureParams, NLSS_OUT int *piNum);

/**
*  ������Ƶʵ������������ֱ��
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pVideoInParam: ����Ƶ�ɼ����������
*
*  @return _HNLSSCHILDSERVICE ��NULL�ɹ��� NULLʧ��
*/
EXPORTS_API  _HNLSSCHILDSERVICE      Nlss_ChildVideoOpen(_HNLSSERVICE hNLSService, ST_NLSS_VIDEOIN_PARAM *pVideoInParam);

/**
*  ��������Ƶ��Ϊ�����㣬ͬʱ������Ƶ��ʾȫ���̿�
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return ��
*/
EXPORTS_API       void               Nlss_ChildVideoSetBackLayer(_HNLSSCHILDSERVICE hNLSSChild);

/**
*  ��������Ƶ�㼶�������ߵ���һ������ʾ
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*  @param  bAdustUp: true����һ�㣬false��һ�㣬���
*
*  @return ��
*/
EXPORTS_API       void               Nlss_ChildVideoAdjustLayer(_HNLSSCHILDSERVICE hNLSSChild, bool bAdustUp);

/**
*  ���ø�����Ƶ�������������е���ʾλ�úʹ�С
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*  @param  pstRect: ��ʾλ�ò���
*
*  @return ��
*/
EXPORTS_API       void               Nlss_ChildVideoSetDisplayRect(_HNLSSCHILDSERVICE hNLSSChild, ST_NLSS_RECTSCREEN_PARAM *pstRect);

/**
*  �ر�����Ƶʵ��
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return ��
*/
EXPORTS_API       void               Nlss_ChildVideoClose(_HNLSSCHILDSERVICE hNLSSChild);

/**
*  ����Ƿ�Ϊ�����ɼ��豸������Ƶ�ɼ�����
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return bool true�У�falseû��
*/
EXPORTS_API  bool  Nlss_ChildVideoIsOtherDevice(_HNLSSCHILDSERVICE hNLSSChild);

/**
*  �򿪲ɼ��豸�ĸ߼����ã�
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*  ע�� �ýӿڿ���ϵͳ�ᵯ���ɼ����ô��ڣ����Խ�����Nlss_IsOtherCaptureDevice��⵽֮�����û��ֶ�ѡ���Ƿ���
*/
EXPORTS_API  NLSS_RET  Nlss_ChildVideoOpenOtherDeviceConf(_HNLSSCHILDSERVICE hNLSSChild);

/**
*  ��ʱ��|�ظ�����Ƶ�����Ƿ���ʾ
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*  @param  bHide: ����Ƶ���ڵ����Ƿ����أ����
*
*  @return ��
*/
EXPORTS_API       void     Nlss_ChildVideoSwitchDisplay(_HNLSSCHILDSERVICE hNLSSChild, bool bHide);

/**
*  ������Ƶ�ɼ�����Ҫ����ƵԤ��ǰ���ã�����Ҫ�ı�ɼ��豸ʱ��ʱ����Ҫ��ֹͣ�����´�
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API  NLSS_RET  Nlss_ChildVideoStartCapture( _HNLSSCHILDSERVICE hNLSSChild);

/**
*  �ر���Ƶ�ɼ�
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return ��
*/
EXPORTS_API   void     Nlss_ChildVideoStopCapture(_HNLSSCHILDSERVICE hNLSSChild);


/*******************************����ͼ��ص�API*************************************************************/
/**
*  ���õ���Ԥ������Ƶ��buffer�ص�������������Ԥ��������ʾ
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*  @param  pFunVideoSamplerCB: ����������buffer�ص����������
*
*  @return ��
*/
EXPORTS_API   void     Nlss_ChildVideoSetSoloPreviewCB(_HNLSSCHILDSERVICE hNLSSChild, PFN_NLSS_CHILD_VIDEO_SAMPLER_CB pFunVideoSamplerCB);

/**
*  ��|������Ƶ����Ԥ��
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*  @param  bOn: ����Ƶ���ڵ���Ԥ�����أ����
*
*  @return ��
*/
EXPORTS_API  void      Nlss_ChildVideoSwitchSoloPreview(_HNLSSCHILDSERVICE hNLSSChild, bool bOn);


/*******************************ֱ������API*************************************************************/
/**
*  ��ͣ��Ƶֱ������
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_ChildVideoPauseLiveStream(_HNLSSCHILDSERVICE hNLSSChild);
/**
*  �ָ���Ƶֱ������
*
*  @param  hNLSSChild: ��Ӧ����Ƶʵ�������
*
*  @return ��
*/
EXPORTS_API  void       Nlss_ChildVideoResumeLiveStream(_HNLSSCHILDSERVICE hNLSSChild);

/**
*  �û�ָ����Ƶ���������ͽӿ�
*
*  @param  hNLSService: ֱ������ʵ�������
*  @param  pcVideoData: �û�ָ����Ƶ�����ݣ����
*  @param  iLen:        ��Ƶ�����ݳ��ȣ����
*
*  @return NLSS_RET NLSS_OK�ɹ���NLSS_ERRʧ��
*/
EXPORTS_API NLSS_RET    Nlss_VideoChildSendCustomData(_HNLSSCHILDSERVICE hNLSSChild, char *pcVideoData, int iLen);



#ifdef __cplusplus
}
#endif

#endif// _LS_MEDIACAPTURE_API_H_



