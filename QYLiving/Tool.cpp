#include "Tool.h"
#include "stdafx.h"
bool Tool::DeleteImage(char * filename) {
	string path =  filename;
	// int remove(char *filename);
	// 删除文件，成功返回0，否则返回-1
	if (-1 == remove(path.c_str()))
	{
		return false;
	}

	return true;
}
size_t http_data_writer(void* data, size_t size, size_t nmemb, void* content)
{
	long totalSize = size * nmemb;
	std::string* symbolBuffer = (std::string*)content;
	if (symbolBuffer)
	{
		symbolBuffer->append((char *)data, ((char*)data) + totalSize);
	}
	return totalSize;
}
int  Tool::SendImage(char * filename)
{
	CURL *curl = NULL;
	CURLcode res;
	char  posturl[0x100] = {};
	extern char url[0x100];
	
	sprintf_s(posturl, 0x100, "%s/api/file", url);
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	struct curl_slist *headerlist = NULL;
	int timeout = 5;

	std::string strData;
	Json::Reader reader;
	Json::Value root;
	if (filename == NULL )
		return -1;

	printf("URL: %s\n", url);
	printf("filename: %s\n", filename);

	/* Add simple file section */
	if (curl_formadd(&post, &last, CURLFORM_COPYNAME, "file_info",
		CURLFORM_FILE, filename, CURLFORM_END) != 0)
	{
		fprintf(stderr, "curl_formadd error.\n");
		goto out;
	}

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&post, &last,
		CURLFORM_COPYNAME, "submit",
		CURLFORM_COPYCONTENTS, "OK",
		CURLFORM_END);

	//curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == NULL)
	{
		fprintf(stderr, "curl_easy_init() error.\n");
		goto out;
	}
	//char  send_data[] = "reduce_tag";
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_data);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_URL, posturl); /*Set URL*/
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_data_writer);
	//设置写数据  
	
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strData);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform[%d] error.\n", res);
		goto out;
	}
	
	curl_easy_cleanup(curl);
	if (reader.parse(strData.c_str(), root)) {
		int code = root["code"].asInt();
		if (code != 200) {
			fprintf(stderr, "server return code is %d", code);
			goto out;
		}
		int id = root["result"]["id"].asInt();
		SendImageID(id);
	}
		


	
out:
	curl_formfree(post);
	//curl_global_cleanup();
	return 0;
}
bool Tool::SendImageID(int id) {
	CURL *curl = NULL;
	
	CURLcode res;
	char  posturl[0x100] = {};
	extern char url[0x100];
	extern char deviceid[0x100];
	sprintf_s(posturl, 0x100, "%s/api/file", url);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, posturl); /*Set URL*/
	char  send_data[100] = "";
	sprintf_s(send_data, 100, "id=%d", id);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_data);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform[%d] error.\n", res);		
	}
	curl_easy_cleanup(curl);
	return  true;
}
bool Tool::Convert24Image(unsigned char * p32Img, unsigned char * p24Img, unsigned long dwSize32)
{



	if (p32Img != NULL && p24Img != NULL && dwSize32>0)
	{

		DWORD dwSize24;

		dwSize24 = (dwSize32 * 3) / 4;

		BYTE *pTemp, *ptr;

		pTemp = p32Img;
		ptr = p24Img;

		int ival = 0;
		for (DWORD index = 0; index < dwSize32 / 4; index++)
		{
			unsigned char r = *(pTemp++);
			unsigned char g = *(pTemp++);
			unsigned char b = *(pTemp++);
			(pTemp++);//skip alpha  

			*(ptr++) = r;
			*(ptr++) = g;
			*(ptr++) = b;
		}
	}
	else
	{
		return false;
	}

	return true;
}

int  Tool::bmp32_write(unsigned char *cpyImage, unsigned long imageWidth, unsigned long imageHeight)
{
	unsigned char * cpyimage24 = new unsigned char[imageWidth*imageHeight *3];
	unsigned char header[54] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 32, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};

	long file_size = (long)imageWidth * (long)imageHeight * 4 + 54;
	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;

	long width = imageWidth;
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	long height = imageHeight;
	header[22] = height & 0x000000ff;
	header[23] = (height >> 8) & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;

	char fname_bmp[128];
	time_t now;
	int unixTime = (int)time(&now);
	sprintf(fname_bmp, "%s_%d.bmp", FILENAME, unixTime);

	FILE *fp;
	if (!(fp = fopen(fname_bmp, "wb")))
		return -1;

	fwrite(header, sizeof(unsigned char), 54, fp);
	fwrite(cpyImage, sizeof(unsigned char), (size_t)(long)imageWidth * imageHeight * 4, fp);
	//svpng(fp, imageWidth, imageHeight,cpyImage,1);
	
	fclose(fp);

	//DealWithPic(cpyImage, width, height, fname_bmp);
	return 0;
}
//构建BMP位图文件头  
void ContructBhh(int nWidth, int nHeight, BITMAPFILEHEADER& bhh) //add 2010-9-04  
{
	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8; //每行实际占用的大小（每行都被填充到一个4字节边界）  
	bhh.bfType = ((WORD)('M' << 8) | 'B');  //'BM'  
	bhh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + widthStep * nHeight;
	bhh.bfReserved1 = 0;
	bhh.bfReserved2 = 0;
	bhh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

}


//构建BMP文件信息头  
void ConstructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih)
{
	int widthStep = (((nWidth * 24) + 31) & (~31)) / 8;

	bih.biSize = 40;       // header size  
	bih.biWidth = nWidth;
	bih.biHeight = nHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 24;     // RGB encoded, 24 bit  
	bih.biCompression = BI_RGB;   // no compression 非压缩  
	bih.biSizeImage = widthStep * nHeight * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

}

void SetUpDown(BYTE *pData, int image_width, int image_height, int bpp)
{
	int index = bpp / 8;
	int h = 0, w = 0;
	for (h = 0; h < image_height / 2; h++)
	{
		for (w = 0; w < image_width; w++)
		{
			const int iCoordM = index * (h*(image_width) + w);
			const int iCoordN = index * ((image_height - h - 1)*(image_width) + w);
			BYTE Tmp = pData[iCoordM];
			pData[iCoordM] = pData[iCoordN];
			pData[iCoordN] = Tmp;
			Tmp = pData[iCoordM + 1];
			pData[iCoordM + 1] = pData[iCoordN + 1];
			pData[iCoordN + 1] = Tmp;
			Tmp = pData[iCoordM + 2];
			pData[iCoordM + 2] = pData[iCoordN + 2];
			pData[iCoordN + 2] = Tmp;
		}
	}
}
bool Tool::SaveImage(unsigned char * image, unsigned long imageWidth, unsigned long imageHeight) {
	BITMAPINFOHEADER bih;
	ConstructBih(imageWidth, imageHeight, bih);
	BITMAPFILEHEADER bhh;
	ContructBhh(imageWidth, imageHeight, bhh);

	TCHAR BMPFileName[1024];
	int widthStep = (((imageWidth * 24) + 31) & (~31)) / 8; //每行实际占用的大小（每行都被填充到一个4字节边界）  
	int DIBSize = widthStep * imageHeight;  //buffer的大小 （字节为单位）  

										//save  
	char path[1024];
	char str[1024];
	char fname_bmp[128];
	time_t now;
	int unixTime = (int)time(&now);
	tm* Time;
	
	Time = localtime(&now);
	char string1[255];
	strftime(string1, 255, "%Y-%m-%d-%H-%M-%S", Time);
	/*sprintf(fname_bmp, "%s_%d.bmp", FILENAME, unixTime);

	FILE *fp;
	if (!(fp = fopen(fname_bmp, "wb")))
		return false;

	fwrite((LPSTR)&bhh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite((LPSTR)&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(image, sizeof(char), DIBSize, fp);
	fclose(fp);*/
	unsigned char * dst_buffer = new unsigned char[imageWidth*imageHeight*3];
	unsigned char *src_point;
	unsigned char *dst_point;

	
	src_point = image + imageWidth * 3*(imageHeight - 1);
	dst_point = dst_buffer + imageWidth * 3*(imageHeight - 1);
	for (unsigned long i = 0; i<imageHeight; i++)
	{
		for (unsigned long j = 0; j<imageWidth*3; j += 3)
		{
			
				dst_point[j + 2] = src_point[j + 0];
				dst_point[j + 1] = src_point[j + 1];
				dst_point[j + 0] = src_point[j + 2];
			
		}
		dst_point -= imageWidth * 3;
		src_point -= imageWidth * 3;
	}
	
	char fname_jpg[128];
	sprintf(fname_jpg, "%s_%s.jpg", FILENAME, string1);
	synthese_jpeg(fname_jpg, dst_buffer, imageWidth, imageHeight, 3);
	/*if (SendImage(fname_jpg)==0)
	{
		if (!DeleteImage(fname_jpg)) {
			fprintf(stdout, "delete %s image failed\n", fname_jpg);
		}
	}*/
	if (!DeleteImage(fname_jpg)) {
		fprintf(stdout, "delete %s image failed\n", fname_jpg);
	}
	delete dst_buffer;
	return true;

}
int Tool::synthese_jpeg(char *filename, unsigned char *bits, int width, int height, int depth)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPARRAY buffer;
	FILE * outfile;
	
	unsigned char *point;

	cinfo.err = jpeg_std_error(&jerr);        //libjpeg各种配置
	jpeg_create_compress(&cinfo);
	if ((outfile = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "can't open %s/n", filename);
		return -1;
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = depth;
	if (depth == 1)
		cinfo.in_color_space = JCS_GRAYSCALE;
	else
		cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 50, TRUE);    //中间的值为压缩质量，越大质量越好
	jpeg_start_compress(&cinfo, TRUE);

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, width*depth, 1);

	point = bits + width * depth*(height - 1);
	while (cinfo.next_scanline<height)
	{
		memcpy(*buffer, point, width*depth);
		jpeg_write_scanlines(&cinfo, buffer, 1);
		point -= width * depth;
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return 0;
}
#define JPEG_QUALITY 50 
int Tool::savejpeg(char *filename, unsigned char *bits, int width, int height, int depth)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;                 //target file   
	JSAMPROW row_pointer[1];        //pointer to JSAMPLE row[s]   
	int     row_stride;             //physical row width in image buffer   
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "can't open %s/n", filename);
		return -1;
	}
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = width;      //image width and height, in pixels   
	cinfo.image_height = height;
	cinfo.input_components = 3;         //# of color components per pixel   
	cinfo.in_color_space = JCS_RGB;         //colorspace of input image   
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE);//limit to baseline-JPEG values   
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = width * depth; // JSAMPLEs per row in image_buffer   
	//while (cinfo.next_scanline < cinfo.image_height)
	//{
	//	//这里我做过修改，由于jpg文件的图像是倒的，所以改了一下读的顺序  
	//	//这是原代码：  
	//	//row_pointer[0] = & bits[cinfo.next_scanline * row_stride];  
	//	row_pointer[0] = &bits[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];
	//	(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	//}
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return 0;
}
int Tool::bmp32to24_write(unsigned char * image, unsigned long imageWidth, unsigned long imageHeight)
{
	unsigned char * cpyimage24 = new unsigned char[imageWidth*imageHeight * 3];
	
	Convert24Image(image,cpyimage24,imageHeight*imageWidth*4);
	
	
	if (flag == 1) {
		SetUpDown(cpyimage24, imageWidth, imageHeight, 24);
		SaveImage(cpyimage24, imageWidth, imageHeight);
		
		flag = 2;
	}
	
	DealWithPic(nullptr ,cpyimage24,imageWidth,imageHeight);
	
	//free(cpyimage24);
	return 0;
}
void getFiles(string path, string exd, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
Tool::Tool() {
	return;

	/* 初始化引擎和变量 */
	MRESULT nRet = MERR_UNKNOWN;
	
	MInt32 nScale = 16;
	MInt32 nMaxFace = 10;
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE);
	if (pWorkMem == nullptr)
	{
		return;
	}
	
	nRet = AFD_FSDK_InitialFaceEngine(APPID, SDKKey_DETECTION, pWorkMem, WORKBUF_SIZE, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
	if (nRet != MOK)
	{
		fprintf(stdout, "注册人脸检测服务失败");
		return;
	}
	/* 打印版本信息 */
	const AFR_FSDK_Version * pVersionInfo = nullptr;
	pVersionInfo = AFR_FSDK_GetVersion(hEngine);
	/*fprintf(stdout, "%d %d %d %d %d\n", pVersionInfo->lCodebase, pVersionInfo->lMajor, pVersionInfo->lMinor, pVersionInfo->lBuild, pVersionInfo->lFeatureLevel);
	fprintf(stdout, "%s\n", pVersionInfo->Version);
	fprintf(stdout, "%s\n", pVersionInfo->BuildDate);
	fprintf(stdout, "%s\n", pVersionInfo->CopyRight);*/
	pWorkMemRecon = (MByte *)malloc(WORKBUF_SIZE);
	if (pWorkMemRecon == nullptr)
	{
		return;
	}

	nRet = AFR_FSDK_InitialEngine(APPID, SDKKey_RECOGNITION, pWorkMemRecon, WORKBUF_SIZE, &hEngineRecon);
	if (nRet != MOK)
	{
		fprintf(stdout, "注册人脸识别服务失败");
		return;
	}

	//获取该路径下的所有jpg文件  
	getFiles(CookerPath, "bmp", files);
	InitModel();

}
Tool::~Tool()
{
	 AFD_FSDK_UninitialFaceEngine(hEngine);
	
	 free(pWorkMem);
}
bool readBmp24(const char* path, uint8_t **imageData, int *pWidth, int *pHeight)
{
	if (path == NULL || imageData == NULL || pWidth == NULL || pHeight == NULL)
	{
		return false;
	}
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
	{
		return false;
	}
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	*pWidth = head.biWidth;
	*pHeight = head.biHeight;
	int biBitCount = head.biBitCount;
	if (24 == biBitCount)
	{
		int lineByte = ((*pWidth) * biBitCount / 8 + 3) / 4 * 4;
		*imageData = (uint8_t *)malloc(lineByte * (*pHeight));
		uint8_t * data = (uint8_t *)malloc(lineByte * (*pHeight));
		fseek(fp, 54, SEEK_SET);
		fread(data, 1, lineByte * (*pHeight), fp);
		for (int i = 0; i < *pHeight; i++)
		{
			for (int j = 0; j < *pWidth; j++)
			{
				memcpy((*imageData) + i * (*pWidth) * 3 + j * 3, data + (((*pHeight) - 1) - i) * lineByte + j * 3, 3);
			}
		}
		free(data);
	}
	else if (32 == biBitCount) {
		
	}
	else
	{
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}
bool readBmpFromBuffer(const string& sOutBmp, uint8_t **imageData, int *pWidth, int *pHeight)
{
	if (sOutBmp.empty() || imageData == NULL || pWidth == NULL || pHeight == NULL)
	{
		return false;
	}
	const char* pBuffer = sOutBmp.c_str();
	BITMAPINFOHEADER head;
	memcpy(&head, pBuffer + sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
	*pWidth = head.biWidth;
	*pHeight = head.biHeight;
	int biBitCount = head.biBitCount;
	if (24 == biBitCount)
	{
		int lineByte = ((*pWidth) * biBitCount / 8 + 3) / 4 * 4;
		*imageData = (uint8_t *)malloc(lineByte * (*pHeight));
		for (int i = 0; i < *pHeight; i++)
		{
			memcpy(*imageData + i * (*pWidth) * 3, pBuffer + (*pHeight - 1 - i) * lineByte + 54, (*pWidth) * 3);
		}
	}
	else if (32 == biBitCount)
	{
		*imageData = (uint8_t *)malloc((*pWidth) * (*pHeight) * 3);
		for (int i = 0; i < *pHeight; i++)
		{
			for (int j = 0; j < *pWidth; j++)
			{
				memcpy((*imageData) + i * (*pWidth) * 3 + j * 3, pBuffer + (((*pHeight) - 1) - i) * (*pWidth) * 4 + j * 4 + 54, 3);
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}
bool ConvertImageToBmp(string sInputImg, string& pOutBmpImg)
{
	FILE *fp = fopen(sInputImg.c_str(), "rb");
	if (fp == NULL)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int lSize = ftell(fp);
	rewind(fp);

	/* 分配内存存储整个文件 */
	char*	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return false;
	}

	/* 将文件拷贝到buffer中 */
	int result = fread(buffer, 1, lSize, fp);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		free(buffer);
		fclose(fp);
		return false;
	}
	fclose(fp);
	pOutBmpImg.resize(lSize);
	memcpy((void*)pOutBmpImg.c_str(), buffer, lSize);
	free(buffer);
	return true;
}
bool readBmp(const char* sInputImg, uint8_t **imageData, int *pWidth, int *pHeight)
{
	string sOutBmp;
	if (ConvertImageToBmp(sInputImg, sOutBmp))
	{
		return readBmpFromBuffer(sOutBmp, imageData, pWidth, pHeight);
	}
	return false;
}
void Tool::GetCurrenUserFace() {

}
void Tool::InitModel() {
	ASVLOFFSCREEN offInput = { 0 };
	AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
	MRESULT nRet;
	offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
	offInput.ppu8Plane[0] = nullptr;
	for (size_t j = 0; j < files.size(); j++)
	{
		readBmp(files[j].c_str(), (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);
		if (!offInput.ppu8Plane[0])
		{
			fprintf(stderr, "fail to ReadBmp(%s)\n", files[j]);
			AFR_FSDK_UninitialEngine(hEngine);
			free(pWorkMem);
			return;
		}
		offInput.pi32Pitch[0] = offInput.i32Width * 3;
		LPAFD_FSDK_FACERES	FaceRes = nullptr;
		nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
		if (nRet != MOK)
		{
			fprintf(stderr, "Face Detection failed, error code: %d\n", nRet);
		}
		else
		{
			//fprintf(stdout, "The number of face: %d\n", FaceRes->nFace);
			for (int i = 0; i < FaceRes->nFace; ++i)
			{
				//fprintf(stdout, "Face[%d]: rect[%d,%d,%d,%d], Face orient: %d\n", i, FaceRes->rcFace[i].left, FaceRes->rcFace[i].top, FaceRes->rcFace[i].right, FaceRes->rcFace[i].bottom, FaceRes->lfaceOrient[i]);

				AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
				AFR_FSDK_FACEINPUT faceInput;
				faceInput.lOrient = FaceRes->lfaceOrient[i];//人脸方向
															//人脸框位置
				faceInput.rcFace.left = FaceRes->rcFace[i].left;
				faceInput.rcFace.top = FaceRes->rcFace[i].top;
				faceInput.rcFace.right = FaceRes->rcFace[i].right;
				faceInput.rcFace.bottom = FaceRes->rcFace[i].bottom;
				nRet = AFR_FSDK_ExtractFRFeature(hEngineRecon, &offInput, &faceInput, &LocalFaceModels);
				if (nRet != MOK)
				{
					fprintf(stderr, "fail to Extract 1st FR Feature, error code: %d\n", nRet);
					continue;
				}
				
				/* 拷贝人脸特征结果 */
				AFR_FSDK_FACEMODEL faceModels1 = { 0 };
				faceModels1.lFeatureSize = LocalFaceModels.lFeatureSize;
				faceModels1.pbFeature = (MByte*)malloc(faceModels1.lFeatureSize);
				memcpy(faceModels1.pbFeature, LocalFaceModels.pbFeature, faceModels1.lFeatureSize);
				Faces result = { 0 };
				result.Model = faceModels1;
				result.User = files[j];
				fprintf(stdout, "Add Face Model from path：%s\n", files[j].c_str());
				faces.push_back(result);
			}
		}
	}
}
void Tool::RecognitionFace(AFR_FSDK_FACEMODEL faceModel) {			
		
		
	for ( int i = 0; i < faces.size(); i++)
	{
		MFloat  fSimilScore = 0.0f;
		MRESULT nRet = AFR_FSDK_FacePairMatching(hEngineRecon, &faceModel, &(faces[i].Model), &fSimilScore);
		if (nRet == MOK)
		{
			if (fSimilScore > 0.6) {
			
				fprintf(stdout, "User = %s , fSimilScore =  %f\n", faces[i].User.c_str(), fSimilScore);
			}
			
		}
		else
		{
			fprintf(stderr, "FacePairMatching failed , errorcode is %d \n", nRet);
		}
	}
	
	//readBmp24(INPUT_IMAGE_PATH, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);

}
void Tool::DealWithPic(char * path,unsigned char * image ,unsigned long width,unsigned long height)
{
	ASVLOFFSCREEN offInput = { 0 };
	offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
	offInput.ppu8Plane[0] = nullptr;
	offInput.i32Height = height;
	offInput.i32Width = width;
	offInput.ppu8Plane[0]=image;
	//readBmp24(path, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);
	if (!offInput.ppu8Plane[0])
	{
		fprintf(stderr, "Fail to ReadBmp(%s)\n","tupian");
		AFD_FSDK_UninitialFaceEngine(hEngine);
		free(offInput.ppu8Plane[0]);
		free(pWorkMem);
		return ;
	}
	else
	{
		//fprintf(stdout, "Picture width : %d , height : %d \n", offInput.i32Width, offInput.i32Height);
	}
	offInput.pi32Pitch[0] = offInput.i32Width * 3;
	LPAFD_FSDK_FACERES	FaceRes = nullptr;
	unsigned long nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
	if (nRet != MOK)
	{
		fprintf(stderr, "Face Detection failed, error code: %d\n", nRet);
	}
	else
	{
		//fprintf(stdout, "The number of face: %d\n", FaceRes->nFace);
		if (FaceRes->nFace == 0) {
		//fprintf(stdout,)
			free(offInput.ppu8Plane[0]);
			return;
		}
		for (int i = 0; i < FaceRes->nFace; ++i)
		{
			//fprintf(stdout, "Face[%d]: rect[%d,%d,%d,%d], Face orient: %d\n", i, FaceRes->rcFace[i].left, FaceRes->rcFace[i].top, FaceRes->rcFace[i].right, FaceRes->rcFace[i].bottom, FaceRes->lfaceOrient[i]);
			AFR_FSDK_FACEINPUT faceInput;
			faceInput.lOrient = FaceRes->lfaceOrient[i];//人脸方向
											   //人脸框位置
			faceInput.rcFace.left = FaceRes->rcFace[i].left ;
			faceInput.rcFace.top = FaceRes->rcFace[i].top ;
			faceInput.rcFace.right = FaceRes->rcFace[i].right ;
			faceInput.rcFace.bottom = FaceRes->rcFace[i].bottom ;
			AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
			nRet = AFR_FSDK_ExtractFRFeature(hEngineRecon, &offInput, &faceInput, &LocalFaceModels);
			if (nRet != MOK)
			{
				//fprintf(stderr, "fail to Extract 1st FR Feature, error code: %d\n", nRet);
				continue;
			}
			/* 拷贝人脸特征结果 */
			
			RecognitionFace(LocalFaceModels);

		}
	}
	free(offInput.ppu8Plane[0]);
	
	
}
