// http_request.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"

#include <iostream>
#include <fstream>

class DownCallbackClass
{
public:
	DownCallbackClass() :m_down_finished(false) {}
	~DownCallbackClass() {}
public:
	void DownResultCallback(int id, bool success, const std::string& data)
	{
		m_down_finished = true;
	}
	int down_callback(double total_size, double downloaded_size, void* userdata)
	{
		long tmp = static_cast<long>(downloaded_size / total_size * 100);
		printf("\r下载进度%d", tmp);
		return 0;
	}
	bool IsDownFinished(void) { return m_down_finished; }
private:
	bool m_down_finished;
};

class MyResultClass
{
public:
	MyResultClass() : m_request_finished(false) { }
	~MyResultClass() { }

public:
	void MyRequestResultCallback(int id, bool success, const std::string& data)
	{
		if (success)
		{
			std::ofstream outfile;
			outfile.open("baidu.html", std::ios_base::binary | std::ios_base::trunc);
			if (outfile.good()) outfile.write(data.c_str(), data.size());
		}
		m_request_finished = true;
	}
	bool IsRequestFinish(void) { return m_request_finished; }
private:
	bool m_request_finished;
};

