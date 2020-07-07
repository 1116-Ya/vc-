#include "stdafx.h"
#include <stdio.h>
#include "Setting.h"
#include <Windows.h>
#define CHARBUFF 124

void getGurrentDirectory(char *currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);

	
}

int readInt(const char *section, const char *keyword, int defaultValue, const char *filePath) {
	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);
	char settingFile[CHARBUFF];
	sprintf_s(settingFile,"%s\\%s",currentDirectory , filePath);
	fprintf(stdout, "%s , %d\n", settingFile, GetPrivateProfileInt(section, keyword, defaultValue, settingFile));
	return GetPrivateProfileInt(section, keyword, defaultValue, settingFile);
	
}

bool readChar(const char *section, const char *keyword, const char *defaultValue, char *returnValue, const char *filePath) {

	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);
	
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s", currentDirectory, filePath);

	if (GetPrivateProfileString(section, keyword, defaultValue,returnValue,CHARBUFF, settingFile ) != 0) {
		fprintf(stdout, "%s , %s\n", settingFile, returnValue);
		return true;
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", settingFile, section, keyword);
		return false;
	}
	
}
double readDouble(const char *section, const char *keyword, double defaultValue, const char *filePath) {

	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);

	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s", currentDirectory, filePath);

	char returnValue[CHARBUFF];
	char defaultValue2[CHARBUFF];
	sprintf_s(defaultValue2, "%lf", defaultValue);
	double x;
	if (GetPrivateProfileString(section, keyword, defaultValue2, returnValue, CHARBUFF, settingFile) != 0) {
		fprintf(stdout, "%s , %s\n", settingFile, returnValue);
		
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", settingFile, section, keyword);
		
	}
	x = strtod(returnValue,NULL);
	return x;
}


bool writeChar(const char *section, const char *keyword, const char *returnValue, const char *filePath) {
	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);

	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s", currentDirectory, filePath);

	if (WritePrivateProfileString(section, keyword, returnValue, settingFile)) {
		return true;
	}
	else {
		fprintf(stderr, "failed to write [$s] %s to %s \n", section, keyword, settingFile);
		return false;
	}
}
bool writeInt(const char *section, const char *keyword, int returnValue, const char *filePath) {
	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);

	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s", currentDirectory, filePath);

	char returnValue2[CHARBUFF];
	sprintf_s(returnValue2, "%d", returnValue);
	if (WritePrivateProfileString(section, keyword, returnValue2, settingFile)) {
		return true;
	}
	else {
		fprintf(stderr, "failed to write [$s] %s to %s \n", section, keyword, settingFile);
		return false;
	}
}
bool writeDouble(const char *section, const char *keyword, double returnValue, const char *filePath) {
	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);

	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s", currentDirectory, filePath);

	char returnValue2[CHARBUFF];
	sprintf_s(returnValue2, "%lf", returnValue);
	if (WritePrivateProfileString(section, keyword, returnValue2, settingFile)) {
		return true;
	}
	else {
		fprintf(stderr, "failed to write [$s] %s to %s \n", section, keyword, settingFile);
		return false;
	}
}
