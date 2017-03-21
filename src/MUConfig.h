#ifndef __MUCONFIG_H__
#define __MUCONFIG_H__

class MUConfig {
public:
	MUConfig();
	~MUConfig();

public:

	void loadConfig(const char *file);

	int totalLine();

	bool nextLine();

	char* getLineKey();

	bool getLineBool();

	int getLineInt();

	float getLineFloat();

	char *getLineString();

private:

	const char *_filePath;
};

#endif //__MUCONFIG_H__
