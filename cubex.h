/*
 * cubex.h
 * Cubex by Eric Dietz (c) 2003
 * Cube Puzzle and Universal Solver.
 * Notes: readme.txt  Email: root@wrongway.org
 * NOTE: This program is unaffiliated with the Rubik's Cube Trademark.
 * This program MAY NOT be reproduced or modified outside the licensing terms
 * set forth in the readme.
 */

#ifndef _CUBEX_H_
#define _CUBEX_H_

// required includes/namespace
#include <string>
using namespace std;

// Class declaration - class members/methods, some encapsulated
class Cubex
{
public:
  Cubex();
  virtual ~Cubex();
  static int numcubes;
  const static char* ver;
  const static char* author;
  const static int N = 3; // <-- size of the cube (NxNxN)
  const static int MOV = 8;
  const bool operator==(const Cubex &q);
  const bool operator!=(const Cubex &q);
  int *face(int x, int y, int z);
  const void RenderScreen();
  const bool IsSolved();
  const void ResetCube();
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
  const void ScrambleCube();
  const void DoSolution();
  const int SolveCube();
  int Cub[N+2][N+2][N+2];
  bool shorten;
  bool cubeinit;
  int cenfix;
  int mov[MOV+1];
  int erval;
  string solution;
  const int FindCent(int a);
  const int FindEdge(int a, int b);
  const int FindCorn(int a, int b, int c);
  const string Concise(string a);
  const string Efficient(string a);
  int fx;
  int fy;
  int fz;

  int m;
  string sl;
  //string t;
  //string pp;
  int mvs[MOV+1];
  const static int NUM_THREADS = 6;  //the number of threads (pthreads)
  bool startThread();
  void waitToExit(int nn);
  static int threadIndex;
  static int orientationIndex;
  pthread_t threads[NUM_THREADS];
  pthread_mutex_t mutexSolve;
  static void * threadEntryFunction(void * This);
  void SolveCompare(int orientationIndex);

protected:
  //virtual void threadEntry()=0;

private:
  const void Ctemp();
  const string TopEdges();
  const string TopCorners();
  const string MiddleEdges();
  const string BottomEdgesOrient();
  const string BottomEdgesPosition();
  const string BottomCornersPosition();
  const string BottomCornersOrient();
  const string CentersRotate();
  int Tmp[N+2][N+2][N+2];
};
// end of header

#endif /* _CUBEX_H_ */

// many of the routines have been generalized for NxNxN, with a few exceptions,
// mainly to accomadate the CentersRotate feature.





/*
//here just convenience

    CubeOrientation co;
    co.InitOrientation(Cub, q);
    co.PreSolve();
    // solve the cube...

    string t = "";
    t = co.TopEdges();
    if (m > 0 && m < t.length() / 3) {
	continue;
    }
    t += co.TopCorners();
    if (m > 0 && m < t.length() / 3) {
	continue;
    }
    t += co.MiddleEdges();
    if (m > 0 && m < t.length() / 3) {
	continue;
    }
    if (!cubeinit && erval == 0) { erval = 4; }
    t += co.BottomEdgesOrient();
    if (m > 0 && m < t.length() / 3) {
	continue;
    }
    if (!cubeinit && erval == 0) { erval = 5; }
    t += co.BottomEdgesPosition();
    if (m > 0 && m < t.length() / 3) {
	continue;
    }
    if (!cubeinit && erval == 0) { erval = 2; }
    t += co.BottomCornersPosition();
    if (m > 0 && m < t.length() / 3) {
	continue;
    }
    if (!cubeinit && erval == 0) { erval = 6; }
    t += co.BottomCornersOrient();
    if (m > 0 && m < t.length() / 3) {
	continue; 
    }
    if (!cubeinit && erval == 0) { erval = 7; }
    t += co.CentersRotate();
    if (!cubeinit && erval == 0) { erval = 3; }
    // errors above:
    // 2-nondescript parity, 3-center orientation, 4-backward centers or corners,
    // 5-edge flip parity, 6-edge swap parity, 7-corner rotation parity
    if (shorten) { 
      mov[0] = -1; t = co.Concise(p + t); mov[0] = 0;
    }
    t = co.Efficient(t);
    co.numMoves = t.length() / 3;
    co.solution = t;
    printf("The moves for orientation %d is %d.\n", co.numOrientation, co.numMoves);
    // if this was shortest solution found so far, run with it...
    if (co.numMoves < m || m < 0) {
      m = co.numMoves; sl = co.solution;
      for (int i = 1; i <= MOV; i++) {
        mvs[i] = mov[i];
      }
    }

*/
