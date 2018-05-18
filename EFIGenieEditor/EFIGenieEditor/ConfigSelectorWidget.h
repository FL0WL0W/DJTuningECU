#include <string>
#include <vector>
#include <Functions.h>
#include <QGridLayout>
#include <QComboBox>
#include <IConfigWidget.h>
#include <vector>
#include <NumberConfigWidget.h>
#include <Table1ConfigWidget.h>
#include <Table2ConfigWidget.h>
#include <ConfigWidget.h>

#ifndef ConfigSelectorWidget_H
#define ConfigSelectorWidget_H
class ConfigSelectorWidget : public QWidget, public IConfigWidget
{
	Q_OBJECT
public slots:
	void currentIndexChanged(int index);
public:
	ConfigWidget *configWidget = 0;
	QComboBox *Selection;
	std::map<unsigned short, std::map<unsigned char, std::pair<std::string, std::string>>> Definitions;
	std::map<unsigned char, std::pair<std::string, std::string>> TypeDefinitions;
	QGridLayout *layout = new QGridLayout;
	unsigned char ServiceId;

	ConfigSelectorWidget(unsigned short serviceId, std::map<unsigned short, std::map<unsigned char, std::pair<std::string, std::string>>> definitions);

	void setServiceId(unsigned char serviceId);

	void * getValue();

	void setValue(void *);

	void * getConfigValue();

	void setConfigValue(void *val);

	unsigned int configSize();

	bool isConfigPointer();

	std::string getConfigType();
};
#endif
