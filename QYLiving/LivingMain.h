

#include <iostream>
#include <process.h>
#include <Windows.h>

#include <stdio.h>  

#include "Wininet.h"  
#include <iostream>
#include <Windows.h>
#pragma comment(lib,"Wininet.lib")  
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"LSMediaCapture.lib")
#pragma warning(disable:4996)
#define  NLSS_720P_BITRATE 800000
#include <string.h>

#include "AudioClip.h"

#include <string>
#include "include\nlss_api.h"
#include "include\nlss_type.h"
#include "include\nlss_define.h"
#include "include\nlss_childvideo_api.h"
using namespace std;
#pragma once
class LivingMain
{
public:
	LivingMain(int quality, string livingurl);
	~LivingMain();
	void getcamera();
	void Start();
	void SetRect();
	void Sever(void *lpParameter);
	//BOOL GetRealIpByDomainName(char *szHost, char szIp[50][100], int *nCount);
	int GetOutBitrate(int iWidth, int iHeight, int iFps);
	bool SetVideoOutParam(ST_NLSS_VIDEOOUT_PARAM *pstVideoParam, EN_NLSS_VIDEOQUALITY_LVL enVideoQ, bool bWideScreen);
	friend void NetThread(LPVOID param);
	void  previewCB(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_SAMPLER *pstSampler);
	
	//void statusCB(_HNLSSERVICE hNLSService, EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode);
	_HNLSSERVICE hNLSService = NULL;
	ST_NLSS_INDEVICE_INF *m_pVideoDevices;
	NLSS_RET rtn;
	int quality = 0;
	string livingurl = "";
	ST_NLSS_PARAM stParam;
	
	_HNLSSCHILDSERVICE hChildVideoService1 = NULL;
	ST_NLSS_VIDEOIN_PARAM stChildVInParam;
	ST_NLSS_RECTSCREEN_PARAM stRect = { 0, 1000, 0, 600 };
	string cameraPath = "";
	string StartLivingPath = "1.mp3";
	string PauseLivingPath = "";
	string RecoverLivingPath = "";
	AudioClip ac;
};

