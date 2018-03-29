#include <dirent.h>
#include <iostream>
#include <regex>
#include <string>
#include "cv.h"
#include "highgui.h"
#include "preprocess.h"

using namespace std;
using namespace cv;

vector<string> get_all_files(string path, string suffix)
{
    vector<string> files;
    files.clear();
    regex reg_obj(suffix);

    vector<string> paths;
    paths.push_back(path);

    for(int i = 0; i < paths.size(); i++)
    {
        string curr_path = paths[i];
        DIR *dp;
        struct dirent *dirp;
        if((dp = opendir(curr_path.c_str())) == NULL)
        {
            cerr << "can not open this file." << endl;
            continue;
        }
        while((dirp = readdir(dp)) != NULL)
        {
            if(dirp->d_type == 4)
            {
                if((dirp->d_name)[0] == '.') // 排除 '.' 和 '..'
                    continue;
                string tmp_path = curr_path + "/" + dirp->d_name;
                cout << tmp_path << " ";
                paths.push_back(tmp_path);
                cout << paths[paths.size() - 1] << endl;
            }
            else if(dirp->d_type == 8)
            {
                if(regex_match(dirp->d_name, reg_obj))
                {
                    string full_path = curr_path + "/" + dirp->d_name;
                    files.push_back(full_path);
                }
            }
        }
        closedir(dp);
    }
    return files;
}

int main() {  
    vector<string> files;
    char* src = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/videos/500px.avi";  
    char* tar_dir = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/result/";

    sampling(src, tar_dir);

    // files = get_all_files(src,".*(jpg|bmp|jpeg)$");
    // cout<< "==========start program========="<<endl;
    // for(int i = 0; i <files.size(); i++)  
    // {  
    //     cout<<files[i]<<endl;  
    // }
    return 0;  
}