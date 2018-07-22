#pragma once
#pragma warning(disable:4996)
#include "include\arcsoft_fsdk_face_recognition.h"
#include "include\merror.h"
#include "include\arcsoft_fsdk_face_detection.h"
#include "stdafx.h"
#include <string>  
#include <io.h>  
#include <vector>
#include "include\jconfig.h"
#include "include\jerror.h"
#include "include\jmorecfg.h"
#include "include\jpeglib.h"
#include "include\json\json.h"
#include "stdafx.h"
using namespace std;
using namespace Json;
#ifdef _DEBUG
#pragma comment(lib,"json_vc71_libmtd.lib")
#else
#pragma comment(lib,"json_vc71_libmt.lib")
#endif

#pragma comment(lib,"libarcsoft_fsdk_face_recognition.lib")
#pragma comment(lib,"libarcsoft_fsdk_face_detection.lib")
#pragma comment(lib,"libjpeg.lib")
#define WORKBUF_SIZE        (40*1024*1024)
typedef struct {
	AFR_FSDK_FACEMODEL		Model;		// The extracted features
	string		User;	// The size of pbFeature	
}Faces;
static class Tool
{
	MHandle hEngine = nullptr;
	MHandle hEngineRecon = nullptr;
	MByte *pWorkMem = nullptr;
	MByte *pWorkMemRecon = nullptr;
	
	char APPID[45] = "99g18UxhXngJVYbVFhG9vzbajVdVfKQyQ4Mb3VLGjeDK";
	char SDKKey_DETECTION[45] = "BV7ACCvnVGg3qBMLtcKrUn2ozjCgJKfBQN9AnoQrSFjy";
	char SDKKey_RECOGNITION[45] = "BV7ACCvnVGg3qBMLtcKrUn3BUvz9Q2fdCqWijNe496eb";
	string CookerPath="cooker\\";
	vector<string> files;
	vector<Faces> faces;
	string ImageUrlSever = "http://forb.sckync.com/api/file";
public:
	Tool();
	~Tool();
	void InitModel();
	int flag = 0;
	void GetCurrenUserFace();
	void RecognitionFace(AFR_FSDK_FACEMODEL faceModel);
	void DealWithPic(char * path,unsigned char * image, unsigned long width, unsigned long height);
	bool DeleteImage(char * filename);
	int  SendImage(char * filename);
	bool SendImageID( int id);
	bool   Convert24Image(unsigned char * p32Img, unsigned char * p24Img, unsigned long dwSize32);
	int  bmp32_write(unsigned char * image, unsigned long imageWidth, unsigned long imageHeight);
	bool SaveImage(unsigned char * image, unsigned long imageWidth, unsigned long imageHeight);
	int synthese_jpeg(char * filename, unsigned char * bits, int width, int height, int depth);
	int savejpeg(char * filename, unsigned char * bits, int width, int height, int depth);
	int bmp32to24_write(unsigned char * image, unsigned long imageWidth, unsigned long imageHeight);
	//void  DealWithPic(char * path);
};

