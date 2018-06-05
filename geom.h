#ifndef __geom_h
#define __geom_h


#include <vector>

using namespace std; 


//note that coordinates are, for now, integers
typedef struct _point3d {
  int x,y,z; 
} point3d;

//this is our map key
typedef struct _edge3d {
	int a,b;

  //becasue its an ordered map we need a way to keep it organized.
	friend bool operator<(const _edge3d x, const _edge3d y) {
    return std::tie(x.a,x.b)
    	 < std::tie(y.a,y.b);
}
} edge3d;

// bool _edge3d::operator<(const _edge3d& y) {
//     return (this->a < y.a || (this->a == y.a && this->b < y.b));
// }

// class edge3d
// {
// public:
// 	int a,b;
// 	bool operator<(const edge3d& edge); //operator<
// };

// bool edge3d::operator< (const edge3d& edge){
// 	bool result;
// 	result = (this->a < edge.a || (this->a == edge.a && this->b <edge.b));
// 	return result;
// }

typedef struct _triangle3d {
  int a,b,c; 
  //to avoid duplication of points, a triangle stores pointers to the
  //points (which are stored in the vector of points)
} triangle3d;


/* returns 6 times the signed volume of abcd. The volume is positive
   if d is behind abc,  negative if d is in front (i.e. same side
   as the normal) of abc, and 0 if abcd are coplanar.
 */
int signed_area3D(point3d a, point3d b, point3d c, point3d d);


/* return 1 if points are on the same plane, and 0 otherwise */
int coplanar(point3d a, point3d b, point3d c, point3d d);


/* return 1 if d is  strictly in front of abc; 0 otherwise */
int infront (point3d a, point3d b, point3d c, point3d d); 

vector<triangle3d> incremental_hull(vector<point3d> points);

/* compute and return the convex hull of the points */
vector<triangle3d> brute_force_hull(vector<point3d> points);


#endif
