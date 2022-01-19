#include "face2faceindex.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <algorithm> 
#include <iomanip>
/*
* This function reads the data of a given model file
*/
void Face2FaceIndex::readModel(char *fileName)
{
    //std::cout << fileName <<std::endl;
    
    std::string fileNameInString(fileName);
    int lastDashIndex = 0;
    
    for(unsigned int i = 0; i < fileNameInString.length(); i++)
    {
        if(fileNameInString[i] == '/')
        {
            lastDashIndex = i;
        }
    }

    
    for(unsigned int i = lastDashIndex+1; i < fileNameInString.length(); i++)
    {
        if(fileNameInString[i] == '.')
            break;
        if(i == lastDashIndex+ (unsigned) 1)
            objectName = std::toupper(fileNameInString[i]);
        else 
            objectName += fileNameInString[i];
    }
    std::cout << "\n";

    std::ifstream infile(fileName);
    std::string line;

    std::getline(infile,line);
    //std::cout << "Number of triangles: " << line << "\n";

    while(std::getline(infile, line))
    {
        if(!line.empty())
        {
            float x = 0;
            float y = 0;
            float z = 0;

            std::string coordinate[3];

            std::stringstream ssin(line);
            int j = 0;
            while(ssin.good() && j < 3)
            {
                ssin >> coordinate[j];
                ++j;
            }

            x = atof(coordinate[0].c_str());
            y = atof(coordinate[1].c_str());
            z = atof(coordinate[2].c_str());

            Cartesian3 *vertex = new Cartesian3(x, y, z);
            raw_vertices.push_back(*vertex);
        } 
        else 
        {
            //std::cout << "\n\n";
        }
    }
    
    infile.close();

    findVertices();
    findFaces(fileName);
    //printResult();
    outputFile();
}

/*
* Find the number of vertices of a given model
*/
void Face2FaceIndex::findVertices()
{
    // Remove duplicates
    for(unsigned int i = 0; i < raw_vertices.size(); i++)
    {
        if(std::find(vertices.begin(), vertices.end(), raw_vertices[i]) == vertices.end())
        {
            vertices.push_back(raw_vertices[i]);
        }
    }
    
    /*
    int i = 0;
    for(std::vector<Cartesian3>::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        std::cout << "Vertex " << i << "    ";
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        std::cout << *it << "\n";
        i++;
    }
    */
}

/*
* Find each face with its face indices in CCW order.
*/
void Face2FaceIndex::findFaces(char *fileName){
    std::ifstream infile(fileName);
    std::string line;

    // Ignore first line
    std::getline(infile,line);

    while(std::getline(infile, line))
    {
        if(!line.empty())
        {
            float x = 0;
            float y = 0;
            float z = 0;

            std::string coordinate[3];

            std::stringstream ssin(line);
            int j = 0;
            while(ssin.good() && j < 3)
            {
                ssin >> coordinate[j];
                ++j;
            }

            x = atof(coordinate[0].c_str());
            y = atof(coordinate[1].c_str());
            z = atof(coordinate[2].c_str());

            Cartesian3 *vertex = new Cartesian3(x, y, z);

            for(unsigned int i = 0; i < vertices.size(); i++)
            {
                if(*vertex == vertices[i])
                {
                    triangles.push_back(i);
                }
            }
        } 
    }

    /*
    * Triangle data is stored in a vector.
    * Every three element in the vector is the three point of a triangle
    */
   /*
    for(unsigned int i = 0; i < triangles.size()/3; i++)
    {
        std::cout << "Face " << i << "    ";
        std::cout << triangles[i*3] << " " << triangles[i*3+1] << " " 
        << triangles[i*3+2] << "\n";
    }
    */
    
    infile.close();
}

void Face2FaceIndex::printResult()
{
    std::cout << "# University of Leeds 2021-2022" << "\n";
    std::cout << "# COMP 5812M Assignment 1" << "\n";
    std::cout << "# Yiu Chung Steven Ho" << "\n";
    std::cout << "# 201573846" << "\n";
    std::cout << "#\n";

    std::cout << "# Object Name: " << objectName << "\n";
    std::cout << "# Verticles=" << vertices.size() << " Faces=" << triangles.size()/3 << "\n";
    std::cout << "#\n";

    // Print all vertices
    int i = 0;
    for(std::vector<Cartesian3>::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        std::cout << "Vertex " << i << "    ";
        std::cout << std::fixed;
        std::cout << std::setprecision(6);
        std::cout << *it << "\n";
        i++;
    }

    // Print all faces
    for(unsigned int i = 0; i < triangles.size()/3; i++)
    {
        std::cout << "Face " << i << "    ";
        std::cout << triangles[i*3] << " " << triangles[i*3+1] << " " 
        << triangles[i*3+2] << "\n";
    }
}

void Face2FaceIndex::outputFile()
{
    std::string fileNameOutput =  "../" + objectName + ".face";
    std::ofstream outfile(fileNameOutput.c_str());

    outfile << "# University of Leeds 2021-2022" << "\n";
    outfile << "# COMP 5812M Assignment 1" << "\n";
    outfile << "# Yiu Chung Steven Ho" << "\n";
    outfile << "# 201573846" << "\n";
    outfile << "#\n";

    outfile << "# Object Name: " << objectName << "\n";
    outfile << "# Verticles=" << vertices.size() << " Faces=" << triangles.size()/3 << "\n";
    outfile << "#\n";

    // Print all vertices
    int i = 0;
    for(std::vector<Cartesian3>::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        outfile << "Vertex " << i << "    ";
        outfile << std::fixed;
        outfile << std::setprecision(6);
        outfile << *it << "\n";
        i++;
    }

    // Print all faces
    for(unsigned int i = 0; i < triangles.size()/3; i++)
    {
        outfile << "Face " << i << "    ";
        outfile << triangles[i*3] << " " << triangles[i*3+1] << " " 
        << triangles[i*3+2] << "\n";
    }

    outfile.close();
}

void Face2FaceIndex::clear()
{
    objectName.clear();
    raw_vertices.clear();
    vertices.clear();
    triangles.clear();
}