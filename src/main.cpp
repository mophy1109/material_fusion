#include <dirent.h>
#include <iostream>
#include <regex>
#include <string>
#include "cv.h"
#include "highgui.h"

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
                if((dirp->d_name)[0] == '.') // 这里很奇怪，每个文件夹下都会有两个文件： '.'  和   '..'
                    continue;
                // cout << dirp->d_name << " ";
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

// void fusionImages(vector<string> image_files){
//     # 读取第一张图作为初始结果
//     result = cv2::imread("zirconSmall/1/{}".format(image_files[0]), 0)
//     imgin1 = result

//     for img in image_files:
//         print ("Reading file {}".format(img))
//         imgin2 = cv2.imread("zirconSmall/1/{}".format(img), 0)
//         result, imgin1 = fusion.detect(imgin1, imgin2, result)
//         del imgin2
//     cv2.imwrite("result/" + save_name, result)
// }


int main() {  
    vector<string> files;
    string path = "/media/cwh1001/Workspace/CUDA_projects/material_fusion/data/500X 500PIC";  
  
    files = get_all_files(path,".*(jpg|bmp|jpeg)$");
    cout<< "==========start program========="<<endl;
    for(int i = 0; i <files.size(); i++)  
    {  
        cout<<files[i]<<endl;  
    }

    return 0;  
}  