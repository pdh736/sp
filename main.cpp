#include <iostream>
#include <iomanip>
#include <vector>
#include <io.h>
#include <algorithm>
#include <string>

#define MAX_SIZE 50

typedef struct file
{
	std::string name;
	int size;
	bool isFile;
	std::vector<struct file> fileList;
}FINDFILE;

bool NameCmpDesc(FINDFILE f1, FINDFILE f2)
{
	if(f1.name.compare(f2.name)>0)
		return true;
	else
		return false;
}


void ScanDir(char path[], FINDFILE *list)
{
	_finddata_t fd;
	char fullPath[MAX_SIZE];
	int result=1;
	int handle;

	strcpy(fullPath,path);
	strcat(fullPath,"\\*");

	handle=_findfirst(fullPath, &fd);
	if(handle==-1)
	{
		std::cout<<"error\n";
		return;
	}

	result=_findnext(handle, &fd);

	while(result!=-1)
	{
		FINDFILE temp;
		
		if(fd.attrib & _A_SUBDIR)
		{
			if(fd.name[0]=='.')
			{
				result=_findnext(handle, &fd);
				continue;
			}

			char tempPath[MAX_SIZE];
			strcpy(tempPath,path);
			strcat(tempPath,"\\");
			strcat(tempPath,fd.name);
			temp.name=fd.name;
			temp.isFile=false;
			list->fileList.push_back(temp);
			ScanDir(tempPath, &list->fileList.at(list->fileList.size()-1));
		}
		else
		{
			if(strstr(fd.name,"txt"))
			{
				temp.name=fd.name;
				temp.size=fd.size;
				temp.isFile=true;
				list->fileList.push_back(temp);
			}
		}
		result=_findnext(handle, &fd);
	}
	return;
}

void PrintList(FINDFILE &list, int depth)
{
	std::vector<struct file>::iterator iter;
	
	for(iter=list.fileList.begin();iter<list.fileList.end();iter++)
	{
		for(int i=0;i<depth;i++)
		{
			std::cout<<"  ";
		}
		std::cout.setf(std::ios::left);
		std::cout<<std::setw(50-(depth*2))<<iter->name;

		if(!iter->isFile)
		{
			std::cout<<std::endl;
			PrintList(*iter,depth+1);
			continue;
		}
		std::cout<<"size : "<<iter->size<<"byte"<<std::endl;
	}
	return;
}

void NameSortDesc(FINDFILE &list)
{
	std::vector<struct file>::iterator iter;
	for(iter=list.fileList.begin();iter<list.fileList.end();iter++)
	{
		if(!iter->isFile)
		{
			 NameSortDesc(*iter);
		}

	}
	sort(list.fileList.begin(), list.fileList.end(), NameCmpDesc);
}

int main()
{
	char path[MAX_SIZE];
	int depth=0;
	strcpy(path,".");

	FINDFILE dir;
	dir.isFile=false;
	dir.name=path;

	ScanDir(path, &dir);

	NameSortDesc(dir);

	PrintList(dir, depth);
	
	return 0;
}