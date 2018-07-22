#include "LivingMain.h"
int CurrentLivingStatus = 1;
void  LivingMain::previewCB(_HNLSSERVICE hNLSService, ST_NLSS_VIDEO_SAMPLER *pstSampler)
{
	return;

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
void NetThread(LPVOID param) {

	_HNLSSERVICE hNLSService = (_HNLSSERVICE)param;
	
	// 返回的域名对应实际IP的个数
	int nIpCount = 0;
	// 返回的域名对应实际I列表
	char szIpList[50][100];
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "www.baidu.com");
	//GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
	AudioClip ac;
	BOOL con;
	while (true)
	{
		/*if (closeLive>0)
		{
		goto end;
		}*/
		con = GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
			
		if (nIpCount>0) {
			string IP1 = szIpList[0];
			if (IP1.find("192.")==string::npos)
			{

				if (CurrentLivingStatus == 0) {
					ac.load("连接到无线网络.mp3");
					ac.play();
					Nlss_StartLiveStream(hNLSService);
					CurrentLivingStatus = 1;
				}
				else
				{
					CurrentLivingStatus = 1;
				}
					
			}
			else {
				std::cout << "Not net!" << std::endl;
				ac.load("已连接无线网络无法上网.mp3");
				ac.play();
			}
				
				
				
		}else {
			printf("Not connected!\n");
			ac.load("无法连接到无线网络.mp3");
			ac.play();
			if (CurrentLivingStatus == 1) {
					
				Nlss_StopLiveStream(hNLSService);
				CurrentLivingStatus = 0;
			}
			else
			{
				CurrentLivingStatus = 0;
			}
				
		}
		Sleep(5000);
	}
	

}
void statusCBS(_HNLSSERVICE hNLSService, EN_NLSS_STATUS enStatus, EN_NLSS_ERRCODE enErrCode)
{
	if (enStatus == EN_NLSS_STATUS_START && enErrCode == EN_NLSS_ERR_NO) {
		printf("Living Start\n");
	}
	else
	{
		AudioClip ac;
		ac.load("直播错误请注意.mp3");
		ac.play();
		fprintf(stdout, "Living error:Status=%d,Error=%d\n", enStatus, enErrCode);
		exit(-1);
	}

	return;
}
LivingMain::LivingMain(int quality, string livingurl)
{
	this->quality = quality;
	this->livingurl = livingurl;
}


LivingMain::~LivingMain()
{
}
void LivingMain::SetRect() {

	switch (EN_NLSS_VIDEOQUALITY_LVL(this->quality - 1))
	{
	case EN_NLSS_VIDEOQUALITY_SUPER:

		this->stRect = { 0, 1920, 0,1080 };
		break;
	case EN_NLSS_VIDEOQUALITY_HIGH:

		this->stRect = { 0, 1280, 0, 720 };
		break;
	case EN_NLSS_VIDEOQUALITY_MIDDLE:

		this->stRect = { 0, 640, 0, 480 };
		break;
	case EN_NLSS_VIDEOQUALITY_LOW:

		this->stRect = { 0, 320, 0, 240 };
		break;
	default:
		this->stRect = { 0, 320, 0, 240 };
		break;
	}
}
void LivingMain::Start() {

	DWORD flag;
	BOOL con;
	// 返回的域名对应实际IP的个数
	int nIpCount = 0;
	// 返回的域名对应实际I列表
	char szIpList[50][100];
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "www.baidu.com");
	//GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
	
	while (true)
	{
		con = GetRealIpByDomainName(szDomain, szIpList, &nIpCount);
		
		if (nIpCount>0) {
			
			string IP1 = szIpList[0];
			
			if (IP1.find("192.")== string::npos)
			{
				
				ac.load("连接到无线网络.mp3");
				ac.play();
				//printf("%s\n", szIpList);
				Sleep(2000);
				break;
			}
			else
			{
				std::cout << "Not net!" << std::endl;
				ac.load("已连接无线网络无法上网.mp3");
				ac.play();
			}
			
		}
		else
		{
			std::cout << "Not connected!" << std::endl;
			ac.load("无法连接到无线网络.mp3");
			ac.play();
		}
		
		Sleep(5000);
	}
	std::cout << "1" << std::endl;
	getcamera();
	rtn=Nlss_Create(nullptr,nullptr,&hNLSService);
	if (rtn==NLSS_ERR)
	{
		printf("创建直播服务错误\n");
		return;
	}
	
	Nlss_SetStatusCB(hNLSService, statusCBS);
	rtn = Nlss_GetDefaultParam(hNLSService, &stParam);
	if (rtn == NLSS_ERR)
	{
		
		printf("默认参数错误\n");
		return;
	}
	if (!SetVideoOutParam(&stParam.stVideoParam, EN_NLSS_VIDEOQUALITY_LVL(this->quality - 1), true)) {
		
		printf("创建直播输出设置错误\n");
		return;
	}
	stParam.enOutContent= EN_NLSS_OUTCONTENT_VIDEO;
	stParam.paOutUrl = new char[1024];
	
	memset(stParam.paOutUrl, 0, 1024);
	strcpy(stParam.paOutUrl, this->livingurl.c_str());
	if (NLSS_OK != Nlss_InitParam(hNLSService, &stParam))
	{
		delete[]stParam.paOutUrl;
		
	}
	delete[]stParam.paOutUrl;


	rtn = Nlss_Start(hNLSService);
	if (rtn == NLSS_ERR)
	{
		printf("开始直播错误\n");
		return;
	}

	rtn = Nlss_StartLiveStream(hNLSService);
	if (rtn == NLSS_ERR)
	{
		ac.load("直播错误请注意.mp3");
		ac.play();
		printf("开始推流错误\n");
		Sleep(5000);
		return;
	}
	ac.load("直播开始.mp3");
	ac.play();
	this->stChildVInParam.iCaptureFps = 20;
	this->stChildVInParam.enInType = EN_NLSS_VIDEOIN_CAMERA;
	this->stChildVInParam.u.stInCamera.paDevicePath = (char *)this->cameraPath.c_str();
	this->stChildVInParam.u.stInCamera.enLvl = EN_NLSS_VIDEOQUALITY_LVL(this->quality - 1);
	this->hChildVideoService1 = Nlss_ChildVideoOpen(this->hNLSService, &stChildVInParam);
	
	
	////////////720p摄像头只能画质使用到high， xps只能使用到middle
	Nlss_ChildVideoSetDisplayRect(hChildVideoService1, &stRect);
	rtn = Nlss_ChildVideoStartCapture(hChildVideoService1);
	if (rtn == NLSS_ERR)
	{
		ac.load("打开摄像头失败.mp3");
		ac.play();
		printf("打开摄像头失败\n");
	}
	_beginthread(NetThread, 0, this->hNLSService);
	this->Sever(nullptr);
	
}

void LivingMain::getcamera() {

	int  m_iVideoDeviceNum = 0;
	int  m_iAudioDeviceNum = 0;

	Nlss_GetFreeDevicesNum(&m_iVideoDeviceNum, &m_iAudioDeviceNum);
	while (true)
	{
		if (m_iVideoDeviceNum < 1)
		{
			ac.load("无法检测到摄像头.mp3");
			ac.play();
			Sleep(5000);
		}
		else
		{
			break;
		}
	}
	
	
	m_pVideoDevices = new ST_NLSS_INDEVICE_INF[m_iVideoDeviceNum];
	for (int i = 0; i < m_iVideoDeviceNum; i++)
	{
		m_pVideoDevices[i].paPath = new char[1024];
		m_pVideoDevices[i].paFriendlyName = new char[1024];
	}
	
	Nlss_GetFreeDeviceInf(m_pVideoDevices, m_iVideoDeviceNum, nullptr, 0);
	
	cameraPath = m_pVideoDevices[m_iVideoDeviceNum-1].paPath;
	printf("%s\n",cameraPath.c_str());
	return;
}

int LivingMain:: GetOutBitrate(int iWidth, int iHeight, int iFps)
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

bool LivingMain::SetVideoOutParam(ST_NLSS_VIDEOOUT_PARAM *pstVideoParam, EN_NLSS_VIDEOQUALITY_LVL enVideoQ, bool bWideScreen)
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





void LivingMain::Sever(void *lpParameter)
{
	
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
		if (strcmp(recvBuf, "c") == 0) {
			printf(" 关闭直播:\n");
			ac.load("直播暂停开始通话.mp3");
			ac.play();
			Nlss_ChildVideoClose(this->hChildVideoService1);
			
			
			Sleep(3000);
		}
		else if (strcmp(recvBuf, "e") == 0)
		{
			printf(" 开启直播:\n");
			ac.load("直播恢复关闭通话.mp3");
			ac.play();
			
			this->hChildVideoService1 = Nlss_ChildVideoOpen(this->hNLSService, &stChildVInParam);
			if (this->hChildVideoService1 == nullptr) {
				printf(" 重启失败step1:\n");
			}
			Nlss_ChildVideoSetDisplayRect(hChildVideoService1, &stRect);
			rtn = Nlss_ChildVideoStartCapture(hChildVideoService1);
			

			if (rtn == NLSS_ERR) {
				printf(" 重启失败step2:\n");

			}
			
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

