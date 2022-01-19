///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	main.cpp
//	------------------------
//	
///////////////////////////////////////////////////

#include <QApplication>
#include "GeometricWidget.h"
#include <stdio.h>

#include "face2faceindex.h"
#include "faceindex2directededge.h"

#include <dirent.h>
#include <set>
#include <algorithm>
#include <fstream>


int main(int argc, char **argv)
	{ // main()
	// initialize QT
	QApplication app(argc, argv);

	// the geometric surface
	GeometricSurfaceFaceDS surface;

	Face2FaceIndex f2f;
	FaceIndexToDirectedEdgeIndex f2de;

	// check the args to make sure there's an input file
	if (argc == 2)
		{ // two parameters - read a file
		if (!surface.ReadFileTriangleSoup(argv[1]))
			{ // surface read failed 
			printf("Read failed for file %s\n", argv[1]);
			exit(0);
			} // surface read failed
		else
			{ // surface read succeeded
			//	create a window
			/*
			GeometricWidget aWindow(&surface, NULL);
	
			// 	set the initial size
			aWindow.resize(600, 600);

			// show the window
			aWindow.show();

			// set QT running
			return app.exec();
			*/

			DIR *dir;
			struct dirent *ent;
			std::string fileArgument(argv[1]);

			std::vector<std::string> allFiles;

			std::set<std::string> sortedAllFIles;
			if(fileArgument.compare("all") == 0)
			{
				if((dir = opendir("../models/triangles/")) != NULL)
				{
					while((ent = readdir(dir)) != NULL)
					{
						if(!ent->d_name || ent->d_name[0] == '.')
							continue;
						//std::cout << ent->d_name << "\n";
						/*
						std::string str(ent->d_name);

						std::string filePathInString = "../models/triangles/" + str;
						const char *faceFilePath = filePathInString.c_str();
						f2f.readModel((char*) faceFilePath);

						std::string diredgeFileName = "../" + f2f.objectName + ".face";
						const char *diredgeFilePath = diredgeFileName.c_str();
						f2de.readModel((char*) diredgeFilePath);

						f2f.clear();
						f2de.clear();
						*/
						allFiles.push_back(ent->d_name);
					}
					closedir(dir);

					// Sort the set in alphabetical order
					std::sort(allFiles.begin(), allFiles.end());

					std::string outputFileName = "../Result.txt";
					std::ofstream outfile((char *) outputFileName.c_str());

					for(unsigned int i = 0; i < allFiles.size(); i++)
					{
						std::string filePathInString = "../models/triangles/" + allFiles[i];
						const char *faceFilePath = filePathInString.c_str();
						f2f.readModel((char*) faceFilePath);

						std::string diredgeFileName = "../" + f2f.objectName + ".face";
						const char *diredgeFilePath = diredgeFileName.c_str();
						f2de.readModel((char*) diredgeFilePath);

						f2de.checkIfManifold(outfile);

						// Clear all memory
						f2f.clear();
						f2de.clear();
					}

					outfile.close();
				}
				else
				{
					std::cout << "Failed to read file" << "\n";
				}
			}
			else
			{
				std::ifstream fin(argv[1]);

				if(fin.is_open())
				{
					f2f.readModel(argv[1]);

					std::string diredgeFileName = "../" + f2f.objectName + ".face";
					const char *diredgeFilePath = diredgeFileName.c_str();
					f2de.readModel((char*) diredgeFilePath);
				}
				else
				{
					std::cout << "Failed to read file" << "\n";
				}
			}
			
			} // surface read succeeded			
		} // two parameters - read a file
	else 
		{ // too many parameters 
		printf("Usage: %s filename\n", argv[0]); 
		exit (0);
		} // too many parameters 

	// paranoid return value
	exit(0);
	} // main()
