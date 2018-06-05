#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <map>

using namespace std;

//Purpose: create a matrix determinate and the 6 signed area of the 4 points
//parameters: int n the determinate that we are actually calling and the matrix that we create.
//return value: the  signed area of the points
double det(int n, double mat[10][10])
{
	double d = 0;
    int c, subi, i, j, subj;
    double submat[10][10];
    if (n == 2)
    {
        return( (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
    }
    else
    {
        for(c = 0; c < n; c++)
        {
            subi = 0;
            for(i = 1; i < n; i++)
            {
                subj = 0;
                for(j = 0; j < n; j++)
                {
                    if (j == c)
                    {
                        continue;
                    }
                    submat[subi][subj] = mat[i][j];
                    subj++;
                }
                subi++;
            }
        d = d + (pow(-1 ,c) * mat[0][c] * det(n - 1 ,submat));
        }
    }
    //return signed area.
    return d;
}


/* returns 6 times the signed volume of abcd. The volume is positive
	 if d is behind abc,  negative if d is in front (i.e. same side
	 as the normal) of abc, and 0 if abcd are coplanar.
 */
int signed_area3D(point3d a, point3d b, point3d c, point3d d)
{
		double mat[10][10];
		mat[0][0] = a.x;
		mat[1][0] = a.y;
		mat[2][0] = a.z;
		mat[0][1] = b.x;
		mat[1][1] = b.y;
		mat[2][1] = b.z;
		mat[0][2] = c.x;
		mat[1][2] = c.y;
		mat[2][2] = c.z;
		mat[0][3] = d.x;
		mat[1][3] = d.y;
		mat[2][3] = d.z;
		for(int i = 0; i < 4; i++)
		{
			mat[3][i] = 1;
		}

	return int(det(4,mat));
}


/* return 1 if points are on the same plane, and 0 otherwise */
int coplanar(point3d a, point3d b, point3d c, point3d d)
{

	if(signed_area3D(a,b,c,d)==0)
	{
		return 1;
	}
	return 0;
}


/* return 1 if d is  strictly in front of abc; 0 otherwise */
int infront (point3d a, point3d b, point3d c, point3d d)
{
	if(signed_area3D(a,b,c,d)<0)
	{
		return 1;
	}
	return 0;
}

//determines if the new points we have created 
int visible (point3d a, point3d b, point3d c, point3d d, point3d e)
{
	//take the signed area of point d which is 
	int temp1 = signed_area3D(a,b,c,d);
	int temp2 = signed_area3D(a,b,c,e);
	if(((temp1>0)-(temp1<0))!=(temp2>0)-(temp2<0))
	{
		if(temp1 != 0)
		{
			return 1;
		}
	}
	return 0;
}

//Pupose: Creates and adds to map edges that we find from visible areas.
//parameters: edge3d object to be added to map and a pointer to the map.
//Return value: 1
int addtoMap(edge3d edge, map<edge3d, int> *boundEdges){
	//create a map iterator for use
	map<edge3d, int>::iterator itr;
	itr = boundEdges->find(edge);
	//check to see if you find edge
	if(itr != boundEdges->end()){
		boundEdges->erase(itr);
	}
	//if your edge is not in the map
	else{
		//create a backwords edge so if ab, ba
		//check to see if map
		edge3d edgePrime;
		edgePrime.a = edge.b;
		edgePrime.b = edge.a;
		map<edge3d, int>::iterator itr2;
		itr2 = boundEdges->find(edgePrime);
		if(itr2 != boundEdges->end())
		{
			boundEdges->erase(itr);
		}
		//if really not in map add to map.
		else
		{
			boundEdges->insert(make_pair(edge, 1));
			// cout << "succes" << endl;
		}
	}
	return 0;
}


/* compute and return the convex hull of the points */
/* compute and return convex hull of points in incremental algorithm
* works nicely.
* parameters are nice and everything its just the vector of points provided by the initalized value
* returns a vector containing the points in the hull.
*/
vector<triangle3d> incremental_hull(vector<point3d> points)
{
	vector<triangle3d> result;
	point3d test;
	int xMax, xMin, yMax, yMin, zMax, zMin;
	xMax = 0;
	xMin = 0;
	yMax = 0;
	yMin = 0;
	zMax = 0;
	zMin = 0;

	//creates and finds average points for us to use as our starting four points
	for(int k = 0; k < points.size(); k++)
	{
		if(points.at(xMax).x < points.at(k).x)
		{
			xMax = k;
		}
		else if(points.at(xMin).x > points.at(k).x)
		{
			xMin = k;
		}
		if(points.at(yMax).y < points.at(k).y)
		{
			yMax = k;
		}
		else if(points.at(yMin).y > points.at(k).y)
		{
			yMin = k;
		}
		if(points.at(zMax).z < points.at(k).z)
		{
			zMax = k;
		}
		else if (points.at(zMin).z > points.at(k).z)
		{
			zMin = k;
		}
	}
	//if the starting points are coplanar
	if(coplanar(points.at(xMax),points.at(xMin),points.at(yMax),points.at(yMin)) != 1){
		triangle3d tetra;
		tetra.a = xMax;
		tetra.b = xMin;
		tetra.c = yMax;
		result.push_back(tetra);
		tetra.a = xMax;
		tetra.b = xMin;
		tetra.c = yMin;
		result.push_back(tetra);
		tetra.a = xMax;
		tetra.b = yMax;
		tetra.c = yMin;
		result.push_back(tetra);
		tetra.a = xMin;
		tetra.b = yMax;
		tetra.c = yMin;
		result.push_back(tetra);

		// point3d test;
		test.x = int((points.at(xMax).x + points.at(xMin).x + points.at(yMax).x + points.at(yMin).x)/4);
		test.y = int((points.at(xMax).y + points.at(xMin).y + points.at(yMax).y + points.at(yMin).y)/4);
		test.z = int((points.at(xMax).z + points.at(xMin).z + points.at(yMax).z + points.at(yMin).z)/4);
		cout << "here?" << endl;
		
		// for(int q = 0; q < result.size(); q++)
		// {
		// 	triangle3d temp = result.at(q);
		// 	cout << "(" << points.at(temp.a).x << "," << points.at(temp.a).y << "," << points.at(temp.a).z << ")" << endl;
		// 	cout << "(" << points.at(temp.b).x << "," << points.at(temp.b).y << "," << points.at(temp.b).z << ")" << endl;
		// 	cout << "(" << points.at(temp.c).x << "," << points.at(temp.c).y << "," << points.at(temp.c).z << ")" << endl;
		// }
		// cout << signed_area3D(points.at(xMax),points.at(xMin),points.at(yMax),points.at(yMin)) << " : is it coplanar" << endl;
	}
	else
	{
		triangle3d tetra;
		tetra.a = xMax;
		tetra.b = xMin;
		tetra.c = yMax;
		result.push_back(tetra);
		tetra.a = xMax;
		tetra.b = xMin;
		tetra.c = zMax;
		result.push_back(tetra);
		tetra.a = xMax;
		tetra.b = yMax;
		tetra.c = zMax;
		result.push_back(tetra);
		tetra.a = xMin;
		tetra.b = yMax;
		tetra.c = zMax;
		result.push_back(tetra);

		// point3d test;
		test.x = int((points.at(xMax).x + points.at(xMin).x + points.at(yMax).x + points.at(zMax).x)/4);
		test.y = int((points.at(xMax).y + points.at(xMin).y + points.at(yMax).y + points.at(zMax).y)/4);
		test.z = int((points.at(xMax).z + points.at(xMin).z + points.at(yMax).z + points.at(zMax).z)/4);
	}


	//for all the points in the vector
	for(int i = 0; i < points.size(); i++)
	{
		std::map<edge3d, int> boundEdges;
		//for all the points already in the correct points
		for(int j = result.size()-1; j >= 0; j--)
		{
			triangle3d tempTri = result.at(j);
			//if the point is visible
			if(visible(points.at(tempTri.a), points.at(tempTri.b), points.at(tempTri.c), points.at(i), test)==1)
			{
				edge3d tempEdge;
				tempEdge.a = tempTri.a;
				tempEdge.b = tempTri.b;
				//try to add all 3 edges to the map
				addtoMap(tempEdge, &boundEdges);
				tempEdge.a = tempTri.b;
				tempEdge.b = tempTri.c;
				addtoMap(tempEdge, &boundEdges);
				tempEdge.a = tempTri.a;
				tempEdge.b = tempTri.c;
				addtoMap(tempEdge, &boundEdges);

				//delete the face from the 3dhull
				result.erase(result.begin()+j);
			}
		}

		//create all the new faces using point i and all edges in the boundEdges map
		//iterate through map
		//pushback triangle with edge.a,edge.b,i
		for(std::map<edge3d,int>::iterator it = boundEdges.begin(); it != boundEdges.end(); ++it)
		{
			triangle3d tempAdd;
			tempAdd.a = it->first.a;
			tempAdd.b = it->first.b;
			tempAdd.c = i;
			result.push_back(tempAdd);
		}
	}


	return result;
}
//brute force the hull naivly
//parametrs vector of 3d points
//return value vector containing all the correct parts on the hull.
vector<triangle3d> brute_force_hull(vector<point3d> points)
{

	vector<triangle3d> result;
	//prints the number of vectors accepted in the hull.
	int acceptCount = 0;
	int rejectCount = 0;

	//for all the points in three directions
	for(int i = 0; i < points.size(); i++)
	{
		for(int j = i+1; j < points.size(); j++)
		{
			for(int k = j+1; k < points.size(); k++)
			{
				triangle3d tempTri;
				tempTri.a = i;
				tempTri.b = j;
				tempTri.c = k;

				// cout << tempTri.a << "FINDTHISHERE" << endl;

				int temp = 0;
				//while the next point isnt any of the points already used.
				while(temp == i || temp == j || temp == k || coplanar(points.at(i),points.at(j),points.at(k), points.at(temp)) == 1)
				{
					temp++;
				}
				//comparator
				int comparator = infront(points.at(i),points.at(j),points.at(k), points.at(temp));
				int shouldAdd = 1;
				int count = 0;

				while(count < points.size() && shouldAdd==1)
				{
					//if the point we are loooking at is on a dirrefent side than reject the point.
					if(infront(points.at(i),points.at(j),points.at(k), points.at(count)) != comparator && coplanar(points.at(i),points.at(j),points.at(k), points.at(count)) != 1)
					{
						shouldAdd = 0;
						acceptCount++;
					}
					count++;
				}
				if(shouldAdd==1)
				{
					result.push_back(tempTri);
					rejectCount++;
					//cout << "rejectCount is now " << rejectCount << endl;
				}
			}
		}
	}



	return result;
}
