#include "faceindex2directededge.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <algorithm> 
#include <iomanip>
#include <string>

/*
* This function reads the data of a given model file
*/
void FaceIndexToDirectedEdgeIndex::readModel(char *fileName)
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

    std::string tempLine;
    // Ignore the first 8 lines of text
    for(unsigned int i = 0; i < 8; i++)
    {
        std::getline(infile,tempLine);
        heading += tempLine + "\n";
    }

    while(std::getline(infile, line))
    {
        if(!line.empty())
        {
           if(line.find("Vertex") != std::string::npos)
           {
                float x = 0;
                float y = 0;
                float z = 0;

                std::string coordinate[5];

                std::stringstream ssin(line);
                int j = 0;
                while(ssin.good() && j < 5)
                {
                    ssin >> coordinate[j];
                    ++j;
                }

                x = atof(coordinate[2].c_str());
                y = atof(coordinate[3].c_str());
                z = atof(coordinate[4].c_str());

                Cartesian3 *vertex = new Cartesian3(x, y, z);
                vertices.push_back(*vertex);
           }
           else if(line.find("Face") != std::string::npos)
           {
                float x = 0;
                float y = 0;
                float z = 0;

                std::string coordinate[5];

                std::stringstream ssin(line);
                int j = 0;

                while(ssin.good() && j < 5)
                {
                    ssin >> coordinate[j];
                    ++j;
                }

                x = atof(coordinate[2].c_str());
                y = atof(coordinate[3].c_str());
                z = atof(coordinate[4].c_str());

                triangles.push_back(x);
                triangles.push_back(y);
                triangles.push_back(z);
           }
        } 
    }
    
    infile.close();

    findEdges();
    findFirstDirectedEdge();
    findOtherHalf();

    //printResult();
    outputFile();
    //checkIfManifold();
    //clear();
}

void FaceIndexToDirectedEdgeIndex::findEdges(){
    
    for(unsigned int i = 0; i < triangles.size()/3; i++)
    {
        // Reading three vertex
        for(unsigned int j = 0; j < 3; j++)
        {
            int element1 = (i*3+j);
            int element2 = (i*3+j+1);
            if(j == 2)
                element2 = i*3;

            // Check if edge already exists
            bool edgeExists = false;
            for(unsigned int k = 0; k < edges.size()/2; k++)
            {
                if(edges[k*2] == triangles[element1] && edges[k*2+1] == triangles[element2])
                {
                    edgeExists = true;
                }
            }

            // Add to edge list if it does not exist in our list
            if(!edgeExists)
            {
                edges.push_back(triangles[element1]);
                edges.push_back(triangles[element2]);
            }
        }
    }

    /*
    for(unsigned int i = 0; i < edges.size()/2; i++)
    {
        std::cout << "Edge " << i << "    ";
        std::cout << edges[i*2] << " " << edges[i*2+1] << "\n";
    }
    */
    
}

void FaceIndexToDirectedEdgeIndex::findFirstDirectedEdge()
{
    for(unsigned int vertex = 0; vertex < vertices.size(); vertex++)
    {
        bool directedEdgeFound = false;
        for(unsigned int i = 0; i < edges.size()/2; i++)
        {
            if(edges[i*2] == (int) vertex && !directedEdgeFound)
            {
                firstDirectedEdge.push_back(i);
                directedEdgeFound = true;
            }
        }
    }

    /*
    for(unsigned int i = 0; i < firstDirectedEdge.size(); i++)
    {
        std::cout << "FirstDirectedEdge " << i << " " << firstDirectedEdge[i] << "\n";
    }
    */
}

void FaceIndexToDirectedEdgeIndex::findOtherHalf()
{
    // Initialise all otherhalf to -1;
    otherHalves.resize(edges.size()/2, -1);

    // Find other directed half edge pointing in the 
    // opposite direction
    for(unsigned int i = 0; i < otherHalves.size(); i++)
    {
        for(unsigned int otherEdge = 0; otherEdge < edges.size()/2; otherEdge++)
        {
            // I have no idea what I'm doing
            // But it should be working
            if(edges[i*2] == edges[otherEdge*2+1] && edges[i*2+1] == edges[otherEdge*2])
            {
                otherHalves[i] = otherEdge;
            }
        }
    }
    
    /*
    for(unsigned int i = 0; i < otherHalves.size(); i++)
    {
        std::cout << "OtherHalves " << i << " : " << otherHalves[i] << "\n";
    }
    */
}

void FaceIndexToDirectedEdgeIndex::printResult()
{   
    std::cout << heading;

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

    // Print all first directed edge
    for(unsigned int i = 0; i < firstDirectedEdge.size(); i++)
    {
        std::cout << "FirstDirectedEdge " << i << " " << firstDirectedEdge[i] << "\n";
    }

    // Print all faces
    for(unsigned int i = 0; i < triangles.size()/3; i++)
    {
        std::cout << "Face " << i << "  ";
        std::cout << triangles[i*3] << " " << triangles[i*3+1] << " " 
        << triangles[i*3+2] << "\n";
    }

    // Print all other half edges
    for(unsigned int i = 0; i < otherHalves.size(); i++)
    {
        std::cout << "OtherHalf " << i << " " << otherHalves[i] << "\n";
    }
}

void FaceIndexToDirectedEdgeIndex::outputFile()
{
    std::string fileNameOutput =  "../" + objectName + ".diredge";
    std::ofstream outfile(fileNameOutput.c_str());

    outfile << heading;

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

    // Print all first directed edge
    for(unsigned int i = 0; i < firstDirectedEdge.size(); i++)
    {
        outfile << "FirstDirectedEdge " << i << " " << firstDirectedEdge[i] << "\n";
    }

    // Print all faces
    for(unsigned int i = 0; i < triangles.size()/3; i++)
    {
        outfile << "Face " << i << "    ";
        outfile << triangles[i*3] << " " << triangles[i*3+1] << " " 
        << triangles[i*3+2] << "\n";
    }

    // Print all other half edges
    for(unsigned int i = 0; i < otherHalves.size(); i++)
    {
        outfile << "OtherHalf " << i << " " << otherHalves[i] << "\n";
    }

    outfile.close();
}

void FaceIndexToDirectedEdgeIndex::checkIfManifold(std::ofstream &outfile)
{
    bool notManifold = false;

    bool edgeTestFailed = false;
    bool vertexTestFailed = false;

    int wrongEdgeId = -1;
    // Check if there are no pinch points at vertices
    for(unsigned int i = 0; i < otherHalves.size(); i++)
    {
        if(notManifold)
            break;
        
        if(otherHalves[i] == -1)
        {
            notManifold = true;
            edgeTestFailed = true;
            wrongEdgeId = i;
        }
    }

    int wrongVertexId = -1;

    if(!notManifold)
    {
        for(unsigned int vertex = 0; vertex < firstDirectedEdge.size(); vertex++)
        {
            int cycleCount = 0;
            int numberOfEdge = 0;

            // Find the number of edges related to vertex i
            for(unsigned int i = 0; i < edges.size()/2; i++)
            {
                if(edges[i*2] == (int) vertex)
                {
                    numberOfEdge++;
                }
            }

            int edgeCount = 0;
            int currentEdge = firstDirectedEdge[vertex];
            do
            {
                // Find first half edge
                currentEdge = otherHalves[currentEdge];
                //edgeCount++;

                // Find next edge
                if((currentEdge + 1) % 3 == 0)
                {
                    currentEdge = currentEdge-2;
                }
                else
                {
                    currentEdge++;
                }
                edgeCount++;

                if(currentEdge == firstDirectedEdge[vertex])
                {
                    cycleCount++;
                }
            }
            while(edgeCount != numberOfEdge);

            //std::cout << "Vertex " << vertex << " Cycle Count: " << cycleCount << "\n";

            if(cycleCount > 1)
            {
                notManifold = true;
                vertexTestFailed = true;
                wrongVertexId = vertex;
                break;
            }
        }
    }

    std::cout << objectName << ".face" << "\n";
    outfile << objectName << ".face" << "\n";

    if(notManifold)
    {   
        if(edgeTestFailed)
        {
            std::cout << "Edge " << wrongEdgeId << " manifold test failed, no other half found." << "\n";
            outfile << "Edge " << wrongEdgeId << " manifold test failed, no other half found." << "\n\n";
        }
        if(vertexTestFailed)
        {
            std::cout << "Vertex " << wrongVertexId << " manifold test failed" << "\n";
            outfile << "Vertex " << wrongVertexId << " manifold test failed" << "\n\n";
        }
    }
    else
    {
        std::cout << "It is manifold" << "\n";
        outfile << "It is manifold" << "\n";

        // Use Euler formula to determine the genus of the surface
        int numberOfVertices = vertices.size();
        // Divided by 4 because there are a pair of half edge inside the vector
        int numberOfEdges = edges.size()/4;
        int numberOfFaces = triangles.size()/3;

        // Euler Formula outputing the result of genus
        int genus = (numberOfVertices - numberOfEdges + numberOfFaces - 2) / -2;
        std::cout << "Genus of the surface: " << genus << "\n";
        outfile << "Genus of the surface: " << genus << "\n\n";
    }
}

void FaceIndexToDirectedEdgeIndex::clear()
{
    objectName.clear();
    heading.clear();
    vertices.clear();
    triangles.clear();
    edges.clear();
    firstDirectedEdge.clear();
    otherHalves.clear();
}