#include <fstream>
#include <qgridlayout.h>
#include "EFIGenieEditor.h"
#include <ConfigWidget.h>
#include "TableEditWidget.h"
#include <QMdiSubWindow>
#include <iostream>
#include <windows.h>
#include <cwchar>

QMdiArea *MainArea;
void read_directory(const std::string& name, std::vector<std::string>& v)
{
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	std::wstring ws;
	ws.assign(pattern.begin(), pattern.end());
	if ((hFind = FindFirstFile(ws.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			std::wstring ws2(data.cFileName);
			std::string s;
			s.assign(ws2.begin(), ws2.end());
			v.push_back(s);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}

EFIGenieEditor::EFIGenieEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	MainArea = ui.mainArea;

	std::string path = "config";
	std::vector<std::string> files;
	read_directory(path, files);
	std::map<unsigned short, std::map<unsigned char, std::pair<std::string, std::string>>> definitions;
	std::string mainDefinition;
	for (std::string file : files)
	{
		int serviceId = 0;
		int typeId = 0;
		std::string serviceName = "";
		int pos = file.find("_");
		if (file.find("_") != -1)
		{
			auto params = Split(file, '_');
			if (params.size() < 3)
			{
				continue;
			}
			serviceId = atoi(params[0].c_str());
			typeId = atoi(params[1].c_str());
			serviceName = Split(params[2],'.')[0];
		}
		else if(file != "main.conf")
		{
			continue;
		}

		std::ifstream t;
		int length;
		t.open("config/" + file);      // open input file
		t.seekg(0, std::ios::end);    // go to the end
		length = t.tellg();           // report location (this is the length)
		t.seekg(0, std::ios::beg);    // go back to the beginning
		char *buffer = (char*)calloc(length + 1, length + 1);    // allocate memory for a buffer of appropriate dimension
		t.read(buffer, length);       // read the whole file into the buffer
		t.close();                    // close file handle
		std::string definition = std::string(buffer);
		delete buffer;

		if (file != "main.conf")
		{
			std::map<unsigned short, std::map<unsigned char, std::pair<std::string, std::string>>>::iterator it = definitions.find(serviceId);
			if (it == definitions.end())
			{
				std::map<unsigned char, std::pair<std::string, std::string>> typeDefinitions;
				typeDefinitions.insert(std::pair<unsigned char, std::pair<std::string, std::string>>(typeId, std::pair<std::string, std::string>(serviceName, definition)));
				definitions.insert(std::pair<unsigned short, std::map<unsigned char, std::pair<std::string, std::string>>>(serviceId, typeDefinitions));
			}
			else
			{
				it->second.insert(std::pair<unsigned char, std::pair<std::string, std::string>>(typeId, std::pair<std::string, std::string>(serviceName, definition)));
			}
		}
		else
		{
			mainDefinition = definition;
		}
	}


	QMdiSubWindow *subwindow = new QMdiSubWindow(ui.mainArea);
	ConfigWidget *configWidget = new ConfigWidget(mainDefinition, definitions);
	subwindow->setWidget(configWidget);

	setWindowTitle("Camera Window");
}