#ifndef _FACE_INDEX_TO_DIRECTED_EDGE_INDEX_H
#define _FACE_INDEX_TO_DIRECTED_EDGE_INDEX_H

#include <vector>
#include "Cartesian3.h"

class FaceIndexToDirectedEdgeIndex
{
    public:
        std::string objectName;

        std::string heading;

        std::vector<Cartesian3> vertices;

        std::vector<int> triangles;

        std::vector<int> edges;

        std::vector<int> firstDirectedEdge;

        std::vector<int> otherHalves;

        void readModel(char *fileName);

        void findEdges();

        void findFirstDirectedEdge();

        void findOtherHalf();

        void printResult();

        void outputFile();

        void checkIfManifold(std::ofstream &outfile);

        void clear();
};

#endif