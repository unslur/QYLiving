#include <Windows.h>
#include "sample.h"
#include <conio.h>
#include <stdio.h>  
#include <stdlib.h>  
#include "stdafx.h"
#include "include\nlss_api.h"
#include "Wininet.h"  
#pragma comment(lib,"Wininet.lib")  
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
//获取建议的输出目标码率供开发者使用，请关注
typedef int(*PFN_THREADPROC)(LPVOID pParam);
BOOL  GetRealIpByDomainName(char *szHost, char szIp[50][100], int *nCount);

#define  NLSS_720P_BITRATE 800000

//打开设备

int  iAppNum = 0;
int CurrentLivingStatus = 0;
int closeLive = 0;
_HNLSSERVICE hNLSService = NULL;
ST_NLSS_INDEVICE_INF *m_pVideoDevices;

_HNLSSCHILDSERVICE hChildVideoService1 = NULL;
ST_NLSS_VIDEOIN_PARAM stChildVInParam;
string cameraPath = "";
bool bThreadRun = false;

HANDLE CreateThreadH(PFN_THREADPROC lpThreadFunc, _HNLSSERVICE hNLSService);
void CloseThreadH(HANDLE  hOSThread);

int GetOutBitrate(int iWidth, int iHeight, int iFps)
{
	if (iWidth == 0 || iHeight == 0 || iFps == 0)
	{
		return  NLSS_720P_BITRATE; //1280*720 20fps,设置的码率
	}

	int iOutBitrate = NLSS_720P_BITRATE * iWidth / 1280;
	iOutBitrate = iOutBitrate * iHeight / 720;
	if (iFps >= 15)
	{
		iOutBitrate = iOutBitrate * iFps / 20;
	}
	else if (iFps < 15)
	{
		iOutBitrate = iOutBitrate * 15 / 20;
	}

	return iOutBitrate;
}

bool SetVideoOutParam(ST_NLSS_VIDEOOUT_PARAM *pstVideoParam, EN_NLSS_VIDEOQUALITY_LVL enVideoQ, bool bWideScreen)
{
	pstVideoParam->enOutCodec = EN_NLSS_VIDEOOUT_CODEC_X264;
	pstVideoParam->bHardEncode = false;
	pstVideoParam->iOutFps = 20;

	switch (enVideoQ)
	{
	case EN_NLSS_VIDEOQUALITY_SUPER:
		pstVideoParam->iOutWidth = 1920;
		pstVideoParam->iOutHeight = 1080;
		break;
	case EN_NLSS_VIDEOQUALITY_HIGH:
		pstVideoParam->iOutWidth = 1280;
		pstVideoParam->iOutHeight = 720;
		break;
	case EN_NLSS_VIDEOQUALITY_MIDDLE:
		pstVideoParam->iOutWidth = 640;
		pstVideoParam->iOutHeight = 480;
		break;
	case EN_NLSS_VIDEOQUALITY_LOW:
		pstVideoParam->iOutWidth = 320;
		pstVideoParam->iOutHeight = 240;
		break;
	default:
		return false;
		break;
	}

	if (bWideScreen)
	{
		pstVideoParam->iOutWidth = pstVideoParam->iOutWidth / 4 * 4;
		pstVideoParam->iOutHeight = pstVideoParam->iOutWidth * 9 / 16;
		pstVideoParam->iOutHeight = pstVideoParam->iOutHeight / 4 * 4;
	}
	pstVideoParam->iOutBitrate = GetOutBitrate(pstVideoParam->iOutWidth, pstVideoParam->iOutHeight, pstVideoParam->iOutFps);
	return true;
}

bool SetVideoInParam(ST_NLSS_VIDEOIN_PARAM *pstVideoParam, EN_NLSS_VIDEOIN_TYPE mVideoSourceType, const char *pVideoPath, EN_NLSS_VIDEOQUALITY_LVL  enLvl)
{
	pstVideoParam->enInType = mVideoSourceType;
	switch (mVideoSourceType)
	{
	case EN_NLSS_VIDEOIN_FULLSCREEN:
	{
		pstVideoParam->iCaptureFps = 10;
	}
	break;
	case EN_NLSS_VIDEOIN_CAMERA:
		//获取视频参数
		pstVideoParam->iCaptureFps = 20;
		if (m_pVideoDevices != NULL)
		{
			pstVideoParam->u.stInCamera.paDevicePath = (char *)pVideoPath;
		}
		else
		{
			return false;
		}
		pstVideoParam->u.stInCamera.enLvl = enLvl;
		break;
	case EN_NLSS_VIDEOIN_RECTSCREEN:
		pstVideoParam->u.stInRectScreen.iRectLeft = 100;
		pstVideoParam->u.stInRectScreen.iRectRight = 500;
		pstVideoParam->u.stInRectScreen.iRectTop = 100;
		pstVideoParam->u.stInRectScreen.iRectBottom = 500;
		pstVideoParam->iCaptureFps = 20;

		break;
	case EN_NLSS_VIDEOIN_APP:
	{
		pstVideoParam->iCaptureFps = 20;
		pstVideoParam->u.stInApp.paAppPath = (char *)pVideoPath;

	}

	break;
	case EN_NLSS_VIDEOIN_PNG:
		//strcpy(pstVideoParam->u.stInPng.paPngPath, "logo.png");
		//  = "logo.png";
		pstVideoParam->iCaptureFps = 20;
		break;
	case EN_NLSS_VIDEOIN_NONE:
		return false;
		break;
	default:
		return false;
		break;
	}

	return true;
}

void SetAudioParam(ST_NLSS_AUDIO_PARAM *pstAudioParam, char *pAudioPath, EN_NLSS_AUDIOIN_TYPE enAudioType)
{
	
	pstAudioParam->stIn.iInSamplerate = 44100;
	pstAudioParam->stIn.enInType = enAudioType;
	pstAudioParam->stIn.paaudioDeviceName = pAudioPath;
}

bool initLiveStream(_HNLSSERVICE hNLSService, ST_NLSS_PARAM *pstParam, char *paOutUrl)
{
	bool have_video_source = true;
	bool have_audio_source = false;


	if (have_audio_source && have_video_source)
		pstParam->enOutContent = EN_NLSS_OUTCONTENT_AV;//默认音视频设备都存在则推流音视频，当然，也可以设置成音频/视频，
	else if (have_audio_source && !have_video_source)
		pstParam->enOutContent = EN_NLSS_OUTCONTENT_AUDIO;
	else if (have_video_source && !have_audio_source)
		pstParam->enOutContent = EN_NLSS_OUTCONTENT_VIDEO;
	else if (!have_audio_source && !have_video_source)
	{
		return false;
	}

	pstParam->paOutUrl = new char[1024];
	memset(pstParam->paOutUrl, 0, 1024);
	strcpy(pstParam->paOutUrl, paOutUrl);

	if (NLSS_OK != Nlss_InitParam(hNLSService, pstParam))
	{
		delete[]pstParam->paOutUrl;
		return false;
	}
	delete[]pstParam->paOutUrl;
	return true;
}

Tool *s;
int flag = 1;

void   previewCB(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_SAMPLER *pstSampler)
{
	return;
	int width = pstSampler->iWidth;
	int height = pstSampler->iHeight;
	s->bmp32to24_write(pstSampler->puaData, width, height);
}

void statusCB(_HNLSSERVICE hNLSService, EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
{
	if (enStatus == EN_NLSS_STATUS_START&&enErrCode== EN_NLSS_ERR_NO) {
		printf("Living Start\n");
	}
	else
	{
		fprintf(stdout, "Living error:Status=%d,Error=%d\n", enStatus, enErrCode);
	}
	
	return;
}

void   deleteAllDevices(ST_NLSS_INDEVICE_INF *pstDevices, int iCount)
{
	for (int i = 0; i < iCount; i++)
	{
		delete[]pstDevices[i].paPath;
		delete[]pstDevices[i].paFriendlyName;
	}
	delete pstDevices;
}
void SendImageTimer(void *lpParameter) {
	extern int interval;
	
	while (true)
	{
		Sleep(interval*1000);
		s->flag = 1;
	}
	
}

void Sever(void *lpParameter)
{
	HANDLE hCheck = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShitLiving, NULL, 0, NULL);
	WSADATA wsaData;
	int port = 15554;

	char buf[] = "OK";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return;
	}

	//创建用于监听的套接字  
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024以上的端口号  
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR) {
		printf("Failed bind:%d\n", WSAGetLastError());
		return;
	}

	if (listen(sockSrv, 10) == SOCKET_ERROR) {
		printf("Listen failed:%d", WSAGetLastError());
		return;
	}
	extern int quality;
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		//等待客户请求到来    
		SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
		if (sockConn == SOCKET_ERROR) {
			printf("Accept failed:%d", WSAGetLastError());
			break;
		}

		printf("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

		

		char recvBuf[100];
		memset(recvBuf, 0, sizeof(recvBuf));
		//      //接收数据  
		recv(sockConn, recvBuf, sizeof(recvBuf), 0);
		if (strcmp(recvBuf, "chat") == 0) {
			printf(" 关闭直播:\n");
			Nlss_ChildVideoStopCapture(hChildVideoService1);
			Nlss_ChildVideoClose(hChildVideoService1);
			closeLive = 1;
			//Nlss_StopLiveStream(hNLSService);
			Sleep(1000);
		}
		else if (strcmp(recvBuf, "endchat") == 0)
		{
			printf(" 开启直播:\n");
			closeLive = 1;

			SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_CAMERA, cameraPath.c_str(), EN_NLSS_VIDEOQUALITY_LVL(quality - 1));
			hChildVideoService1 = Nlss_ChildVideoOpen(hNLSService, &stChildVInParam);
			if (hChildVideoService1 == NULL)\
			{printf(" 重启直播失败:\n");
			continue; }
			ST_NLSS_RECTSCREEN_PARAM stRect = { 0, 1000, 0, 600 };
			switch (EN_NLSS_VIDEOQUALITY_LVL(quality - 1))
			{
			case EN_NLSS_VIDEOQUALITY_SUPER:

				stRect = { 0, 1920, 0,1080 };
				break;
			case EN_NLSS_VIDEOQUALITY_HIGH:

				stRect = { 0, 1280, 0, 720 };
				break;
			case EN_NLSS_VIDEOQUALITY_MIDDLE:

				stRect = { 0, 640, 0, 480 };
				break;
			case EN_NLSS_VIDEOQUALITY_LOW:

				stRect = { 0, 320, 0, 240 };
				break;
			default:

				break;
			}
			////////////720p摄像头只能画质使用到high， xps只能使用到middle
			Nlss_ChildVideoSetDisplayRect(hChildVideoService1, &stRect);
			NLSS_RET rtn= Nlss_ChildVideoStartCapture(hChildVideoService1);

			if (rtn == NLSS_ERR) {
				printf(" 重启失败:\n");
			
			}
			 //Nlss_StartVideoPreview(hNLSService);
			 //Nlss_StartLiveStream(hNLSService);
			//HANDLE hCheck = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShitLiving, NULL, 0, NULL);
			//Nlss_StartLiveStream(hNLSService);
		}

		printf("接收到命令 ：%s\n", recvBuf);
		//发送数据  
		int iSend = send(sockConn, buf, sizeof(buf), 0);
		if (iSend == SOCKET_ERROR) {
			printf("send failed");
			break;
		}
		closesocket(sockConn);
	}

	closesocket(sockSrv);
	WSACleanup();
	system("pause");
}


BOOL  GetRealIpByDomainName(char *szHost, char szIp[50][100], int *nCount)
{
	WSADATA wsaData;
	HOSTENT *pHostEnt;
	int nAdapter = 0;
	struct sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		printf(" gethostbyname error for host:\n");
		return FALSE;
	}

	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		while (pHostEnt->h_addr_list[nAdapter])
		{
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);

			sprintf_s(szIp[nAdapter], "%s", inet_ntoa(sAddr.sin_addr));

			nAdapter++;
		}

		*nCount = nAdapter;
	}
	else
	{
		DWORD  dwError = GetLastError();
		*nCount = 0;
	}
	WSACleanup();
	return TRUE;
}
void CheckInternetPingTimer(void *lpParameter) {
	// 返回的域名对应实际IP的个数
	int nIpCount = 0;
	// 返回的域名对应实际I列表
	char szIpList[50][100];
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "www.baidu.com");
	GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
	
	BOOL con;
	while (true)
	{

		con = GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
		if ( nIpCount>0) {
			
			//std::cout << "Connected!" << std::endl;
			if (CurrentLivingStatus == 0) {
				Nlss_StartLiveStream(hNLSService);
				CurrentLivingStatus = 1;
			}
			else
			{
				CurrentLivingStatus = 1;
			}
			Sleep(2000);
		}
		else {
			std::cout << "Not connected!" << std::endl;
			if (CurrentLivingStatus == 1) {
				Nlss_StopLiveStream(hNLSService);
				CurrentLivingStatus = 0;
			}
			else
			{
				CurrentLivingStatus = 0;
			}
			Sleep(2000);
		}
	}
}

void getcamera() {

	int  m_iVideoDeviceNum = 0;
	int  m_iAudioDeviceNum = 0;

	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);

	if (m_iVideoDeviceNum <= 0)
	{
		return;
	}
	else
	{
		m_pVideoDevices = new ST_NLSS_INDEVICE_INF[m_iVideoDeviceNum];
		for (int i = 0; i < m_iVideoDeviceNum; i++)
		{
			m_pVideoDevices[i].paPath = new char[1024];
			m_pVideoDevices[i].paFriendlyName = new char[1024];
		}
	}

	Nlss_GetFreeDeviceInf(m_pVideoDevices, m_iVideoDeviceNum, nullptr, 0);
	cameraPath = m_pVideoDevices[0].paPath;
	return;
}
void ShitLiving(void * lpParameter)
{
	return;
	extern int quality;
	getcamera();
	BOOL con;
	// 返回的域名对应实际IP的个数
	int nIpCount = 0;
	// 返回的域名对应实际I列表
	char szIpList[50][100];
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "www.baidu.com");
	GetRealIpByDomainName(szDomain, szIpList, &nIpCount);

	while (true)
	{
		con = GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
		if (nIpCount>0) {
			//Sleep(2000);
			break;
		}
		else
			std::cout << "Not connected!" << std::endl;
		Sleep(1000);
	}
	
	
	//s = new Tool();
#ifdef _DEBUG
	/*extern char livingurl[0x100];
	char paOutUrl[0x100];
	memcpy(paOutUrl, livingurl, strlen(livingurl));*/
	 char paOutUrl[] = "rtmp://p0cc229d3.live.126.net/live/74e2817e136e4b61b79bfe70548c0aa7?wsSecret=181bc2ac8b4b2de62d161973fa102b7f&wsTime=1520843926";
#else
	 extern char livingurl[0x100];
	 char paOutUrl[0x100] ;
	 memcpy(paOutUrl, livingurl, strlen(livingurl));
	 //char paOutUrl[] = "rtmp://pb9599a7a.live.126.net/live/449a765b771d46ee962c04d835bbb70f?wsSecret=f4abbffb2ba8a521f703d7c42abcc9c6&wsTime=1515057754";
#endif // DEBUG
	 printf("推流地址:%s\n", paOutUrl);
	
	 bool rtn;
	
	int count;
	
	Nlss_Create(NULL, NULL, &hNLSService);

	Nlss_SetVideoSamplerCB(hNLSService, previewCB);
	Nlss_SetStatusCB(hNLSService, statusCB);

	//solo camera
	
	NLSS_RET nrtn = 0;
	ST_NLSS_PARAM stParam;
	Nlss_GetDefaultParam(hNLSService, &stParam);
	rtn=SetVideoOutParam(&stParam.stVideoParam, EN_NLSS_VIDEOQUALITY_LVL(quality-1), true);
	
	rtn = initLiveStream(hNLSService, &stParam, paOutUrl);

	
	//printf("推流地址:%s\n", (char *)m_pVideoDevices[1].paPath);
	rtn=SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_CAMERA, cameraPath.c_str(), EN_NLSS_VIDEOQUALITY_LVL(quality - 1));
	hChildVideoService1 = Nlss_ChildVideoOpen(hNLSService, &stChildVInParam);
	Nlss_Start(hNLSService);
	ST_NLSS_RECTSCREEN_PARAM stRect = { 0, 1000, 0, 600 };
	switch (EN_NLSS_VIDEOQUALITY_LVL(quality - 1))
	{
	case EN_NLSS_VIDEOQUALITY_SUPER:
			
		stRect = { 0, 1920, 0,1080 };
		break;
	case EN_NLSS_VIDEOQUALITY_HIGH:
			
		stRect = { 0, 1280, 0, 720 };
		break;
	case EN_NLSS_VIDEOQUALITY_MIDDLE:
			
		stRect = { 0, 640, 0, 480 };
		break;
	case EN_NLSS_VIDEOQUALITY_LOW:
			
		stRect = { 0, 320, 0, 240 };
		break;
	default:
			
		break;
	}
	////////////720p摄像头只能画质使用到high， xps只能使用到middle
	Nlss_ChildVideoSetDisplayRect(hChildVideoService1, &stRect);
	nrtn = Nlss_ChildVideoStartCapture(hChildVideoService1);
	Nlss_StartVideoPreview(hNLSService);
	Nlss_StartLiveStream(hNLSService);
	CurrentLivingStatus = 1;
	//HANDLE hCheck = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckInternetStatusTimer, NULL, 0, NULL);
	//HANDLE hCheck = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckInternetPingTimer, NULL, 0, NULL);
	
	
		
	
	while (true)
	{
		// 返回的域名对应实际IP的个数
		int nIpCount = 0;
		// 返回的域名对应实际I列表
		char szIpList[50][100];
		char szDomain[256] = { 0 };
		char szIp[1024] = { 0 };
		strcpy_s(szDomain, "www.baidu.com");
		GetRealIpByDomainName(szDomain, szIpList, &nIpCount);

		BOOL con;
		while (true)
		{
			/*if (closeLive>0)
			{
				goto end;
			}*/
			con = GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
			if (nIpCount>0) {

				//std::cout << "Connected!" << std::endl;
				if (CurrentLivingStatus == 0) {
					Nlss_StartLiveStream(hNLSService);
					CurrentLivingStatus = 1;
				}
				else
				{
					CurrentLivingStatus = 1;
				}
				Sleep(2000);
			}
			else {
				std::cout << "Not connected!" << std::endl;
				if (CurrentLivingStatus == 1) {
					Nlss_StopLiveStream(hNLSService);
					CurrentLivingStatus = 0;
				}
				else
				{
					CurrentLivingStatus = 0;
				}
				Sleep(2000);
			}
		}
	}
	
	
	

	

end:
	Nlss_ChildVideoClose(hChildVideoService1);
	Nlss_StopLiveStream(hNLSService);
	Nlss_Stop(hNLSService);
	Nlss_UninitParam(hNLSService);
	Nlss_Destroy(hNLSService);
	
	
}

//int SwitchVideo(LPVOID lpParam)
//{
//	int testNum = 0;
//	_HNLSSERVICE hNLSService = (_HNLSSERVICE)lpParam;
//	if (!hNLSService)
//	{
//		return -1;
//	}
//
//	while (bThreadRun)
//	{
//		if (testNum == 0)
//		{
//			testNum = 1;
//			Nlss_ChildVideoStopCapture(hChildVideoService1);
//			Nlss_ChildVideoClose(hChildVideoService1);
//			hChildVideoService1 = NULL;
//
//			ST_NLSS_VIDEOIN_PARAM stChildVInParam;
//			if (rand() % 2 == 0)
//			{
//				SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_FULLSCREEN, (char *)m_pVideoDevices[0].paPath, EN_NLSS_VIDEOQUALITY_MIDDLE);
//			}
//			else {
//				SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_RECTSCREEN, (char *)m_pVideoDevices[0].paPath, EN_NLSS_VIDEOQUALITY_MIDDLE);
//			}
//
//			hChildVideoService2 = Nlss_ChildVideoOpen(hNLSService, &stChildVInParam);
//			Nlss_ChildVideoSetBackLayer(hChildVideoService2);
//			Nlss_ChildVideoStartCapture(hChildVideoService2);
//		}
//		else if (testNum == 1)
//		{
//			testNum = 0;
//			Nlss_ChildVideoStopCapture(hChildVideoService2);
//			Nlss_ChildVideoClose(hChildVideoService2);
//			hChildVideoService2 = NULL;
//
//			ST_NLSS_VIDEOIN_PARAM stChildVInParam;
//			SetVideoInParam(&stChildVInParam, EN_NLSS_VIDEOIN_CAMERA, (char *)m_pVideoDevices[0].paPath, EN_NLSS_VIDEOQUALITY_MIDDLE);
//			hChildVideoService1 = Nlss_ChildVideoOpen(hNLSService, &stChildVInParam);
//			Nlss_ChildVideoSetBackLayer(hChildVideoService1);
//			Nlss_ChildVideoStartCapture(hChildVideoService1);
//
//		}
//		Sleep(1500);
//	}
//
//	return 0;
//
//}



HANDLE CreateThreadH(PFN_THREADPROC lpThreadFunc, _HNLSSERVICE hNLSService)
{
	HANDLE Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lpThreadFunc, (LPVOID)hNLSService, 0, NULL);
	return Thread;
}

void CloseThreadH(HANDLE  hOSThread)
{
	CloseHandle(hOSThread);
	hOSThread = NULL;
}