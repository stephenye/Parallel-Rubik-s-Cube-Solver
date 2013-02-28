

#ifndef _CUBEORIENTATION_H_
#define _CUBEORIENTATION_H_

#include <string>
using namespace std;

//Class declaration
class CubeOrientation
{
public:
  CubeOrientation();
  const static int N = 3; // <-- size of the cube (NxNxN)
  const static int MOV = 8;
  const bool IsSolved();
  const bool XML(int a, bool n);
  const bool XMR(int a, bool n);
  const bool XMU(int a, bool n);
  const bool XMD(int a, bool n);
  const bool XMC(int a, bool n);
  const bool XMA(int a, bool n);
  const void UL();
  const void UR();
  const void DL();
  const void DR();
  const void LU();
  const void LD();
  const void RU();
  const void RD();
  const void FC();
  const void FA();
  const void BC();
  const void BA();
  const void ML();
  const void MR();
  const void MU();
  const void MD();
  const void MC();
  const void MA();
  const void CL();
  const void CR();
  const void CU();
  const void CD();
  const void CC();
  const void CA();
  const void XCL();
  const void XCR();
  const void XCU();
  const void XCD();
  const void XCC();
  const void XCA();
  const int FindCent(int a);
  const int FindEdge(int a, int b);
  const int FindCorn(int a, int b, int c);
  const void Ctemp();
  const string Concise(string a);
  const string Efficient(string a);
  int Cub[N+2][N+2][N+2];
  bool shorten;
  bool cubeinit;
  int cenfix;
  int erval;
  int numOrientation;
  string solution;
  string p;
  int numMoves;
  int fx;
  int fy;
  int fz;
  int mov[MOV+1];
  void InitOrientation(int inputCube[N+2][N+2][N+2], int q);
  void PreSolve();
  const string TopEdges();
  const string TopCorners();
  const string MiddleEdges();
  const string BottomEdgesOrient();
  const string BottomEdgesPosition();
  const string BottomCornersPosition();
  const string BottomCornersOrient();
  const string CentersRotate();
protected:
private:
  int Tmp[N+2][N+2][N+2];
};

#endif
