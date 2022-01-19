# University of Leeds 2021-2022
# COMP 5812M: Geometry Assignment 1
# Yiu Chung Steven Ho
# 201573846
#

============================
LINUX (FENG-LINUX) COMPILE:
============================

To compile on the University Linux machines, you will need to do the following:

	[userid@machine triangle_renderer]$ module add qt/5.3.1
	[userid@machine triangle_renderer]$ qmake -project QT+=opengl LIBS+=-lGLU
	[userid@machine triangle_renderer]$ qmake
	[userid@machine triangle_renderer]$ make

You should see a compiler warning about an unused parameter, which can be ignored.

To execute the renderer, pass the file name on the command line:

	[userid@machine triangle_renderer]$ ./triangle_renderer ../models/triangles/tetrahedron.tri

	This would generate .face and .diredge file in "Assignment 1 Hand In" directory

You could also read all files from ../models/triangles/ directory by the command:

	[userid@machine triangle_renderer]$ ./triangle_renderer all

	This would also generate a "Result.txt" file outputing all data whether it is manifold or not.
	If it is a manifold mesh, it would show the genus of the surface.
	Otherwise, it will show which edge/vertex failed the manifold test.


===================================
Algorithmic complexity of the code
===================================

face2faceindex.cpp has a complexity of O(n) since it first read .tri for all raw_vertices, remove duplicate and read the file again to find all face indices

faceindex2directededge.cpp has a complexity of O(n^3) when finding edges of the .tri 
file. It would loop through all triangles in the vector, read all three vertex, add 
them to the edge list and compare if the edge already exist in the list.

It also has a complexity of O(n^2) when finding first directed edge.
It would loop through all vertex and find the first edge than is edge from vertex n.

The complexity of finding otherhalf is also O(n^2) as we are doing it in selection sort.