/*
 * @Author: USTB.mophy1109
 * @Date: 2018-04-08 14:07:18
 * @Last Modified by: USTB.mophy1109
 * @Last Modified time: 2018-04-08 15:48:31
 */

#include "CUtil.h"
#include <dirent.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <regex>

using namespace std;
vector<std::string> GetAllFiles(string path, string suffix) {
	vector<string> files;
	files.clear();
	regex reg_obj(suffix);

	vector<string> paths;
	paths.push_back(path);

	for (int i = 0; i < paths.size(); i++) {
		string curr_path = paths[i];
		DIR *dp;
		struct dirent *dirp;
		if ((dp = opendir(curr_path.c_str())) == NULL) {
			cerr << "can not open this file." << endl;
			continue;
		}
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_type == 4) {
				if ((dirp->d_name)[0] == '.') // 排除 '.' 和 '..'
					continue;
				string tmp_path = curr_path + "/" + dirp->d_name;
				cout << tmp_path << " ";
				paths.push_back(tmp_path);
				cout << paths[paths.size() - 1] << endl;
			} else if (dirp->d_type == 8) {
				if (regex_match(dirp->d_name, reg_obj)) {
					string full_path = curr_path + "/" + dirp->d_name;
					files.push_back(full_path);
				}
			}
		}
		closedir(dp);
	}
	return files;
}