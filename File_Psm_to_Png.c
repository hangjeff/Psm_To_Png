#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <stdbool.h>
char path[1024];
const unsigned char png_Header[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
bool flag;

void End_Message(){
	printf("Enter any key to close the program\n");
	getchar();
	getchar();
}

void File_Convert(char *inputName, char *outputName){
	
	
	FILE *inputFile = fopen(inputName, "rb");
	if(!inputFile){
		perror(" Faild to open input file\n");
		flag = false;
		End_Message();
		return;
	}
	
	FILE *outputFile = fopen(outputName, "wb");
	if(!outputFile){
		perror(" Faild to open input file\n");
		flag = false;
		End_Message();
		return;
	}
	
	fwrite(png_Header, 1, 8, outputFile);
	
	unsigned char buffer[1024];
	// The count of bytes the program read
	size_t bytesRead;
	fseek(inputFile, 8, SEEK_SET);
	fseek(outputFile, 8, SEEK_SET);
	while((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)  ) > 0){
		fwrite(buffer, 1, bytesRead, outputFile);
	}
	
	fclose(outputFile);	
	fclose(inputFile);
	
}

void process(){
	
	DIR *dir = opendir(path);
	if(!dir){
		perror("Faild to open the folder\n");
		flag = false;
		End_Message();
		return;
	}
	
	struct dirent *entry;
					
	while((entry = readdir(dir) )  != NULL ){
		if(entry -> d_type == DT_REG){
			const char *filenameExtension =
				strrchr(entry -> d_name, '.');
			if(filenameExtension
				&& strcmp(filenameExtension, ".psm")
				== 0)
				{
					printf("Get File: %s", entry -> d_name);
					// Constract paths of input file and output file
					char inputName[1024] = "";
					char outputName[1024] = "";
					snprintf(inputName, sizeof(inputName), "%s/%s", path, entry -> d_name);
					
					
					char *temp = entry -> d_name;
					char *namePart[1024];
					strncpy(namePart, temp, strlen(temp) - 4);
					snprintf(outputName, sizeof(outputName), "%s/%s.png", path
						, namePart );
					
					File_Convert(inputName, outputName);
					printf(" Output File: %s\n", outputName);
				}
		}
	}
	closedir(path);
}

int main(){
	flag = true;
	printf("Enter directory path: ");
	
	char exePath[1024] = "";
    GetModuleFileName(NULL, exePath, 1024);
	char *lastSlash = strrchr(exePath, '\\');
	if (lastSlash) {
		*lastSlash = '\0';
		_chdir(exePath);
	}
	if(scanf("%1023s", path) != 1){
		fprintf(stderr, "Error reading the directory path.\n");
		flag = false;
		End_Message();
		return 1;
	}
	printf("\n");
	process();
	if(flag){
		printf("Done!\n");
		End_Message();
	}
	
	return 0;
}