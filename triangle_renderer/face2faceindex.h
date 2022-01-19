#ifndef _FACE_TO_FACE_INDEX_H
#define _FACE_TO_FACE_INDEX_H

#include <vector>
#include "Cartesian3.h"

class Face2FaceIndex
{
    public:
        std::string objectName;

        std::vector<Cartesian3> raw_vertices;

        std::vector<Cartesian3> vertices;

        std::vector<int> triangles;

        void readModel(char *fileName);

        void findVertices();

        void findFaces(char *fileName);

        void printResult();

        void outputFile();

        void clear();
};

#endif