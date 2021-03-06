// QYLiving.cpp: 定义控制台应用程序的入口点。
//

#include "LivingMain.h"
#include <stdio.h>  
#include <stdlib.h>  
char url[0x100] = {};
char deviceid[0x100] = {};
char livingurl[0x100] = {};
int interval = 2;
int quality = 1;
#define configFilePath "../config.ini"
int main()
{
	/*GetPrivateProfileString("Attributes", "url", "123", url, sizeof(url), configFilePath);*/
	GetPrivateProfileString("Attributes", "livingurl", "123", livingurl, sizeof(livingurl), configFilePath);
	//GetPrivateProfileString("Attributes", "deviceid", "23", deviceid, sizeof(deviceid), configFilePath);
	//interval = GetPrivateProfileInt("Attributes", "interval", 2, configFilePath);
	quality = GetPrivateProfileInt("Attributes", "quality", 1, configFilePath);
	//printf("上传图片间隔时间：%d\n", interval);
	switch (quality)
	{
	case 1:
		printf("直播使用低画质\n");
		break;
	case 2:
		printf("直播使用普通画质\n");
		break;
	case 3:
		printf("直播使用高清画质\n");
		break;
	default:
		printf("画质错误");
		break;
	}
	if (quality > 3 || quality<1) {
		printf("画质设置错误 %d\n按任意键退出\n", quality);
		getchar();
		return -1;
	}
	//printf("设备编码:%s\n", deviceid);
	//printf("服务器地址:%s\n", url);
	printf("推流地址:%s\n", livingurl);

	
	LivingMain *livingMain = new LivingMain(quality,livingurl);
	livingMain->Start();
	
	return 0;
}

