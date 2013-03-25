/*
 * cubex.cpp
 * Cubex .505 by Eric Dietz (c) 2003 (26 Dec 03, 21 Jan 05) % 0
 * Cube Puzzle and Universal Solver.
 * Notes: readme.txt  Email: root@wrongway.org
 * NOTE: This program is unaffiliated with the Rubik's Cube Trademark.
 * This program MAY NOT be reproduced or modified outside the licensing terms
 * set forth in the readme.
 */

#include <cstdio>
#include <string>
#include <cstdlib>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>
#include <omp.h>
using namespace std;
#include "cubex.h"
#include "cubeOrientation.h"

// definition of cube class
// Cubex constructor & destructor & count
Cubex::Cubex()
{
  shorten = true;
  cenfix = 0;
  erval = 0;
  cubeinit = false;
  solution = "";
  ResetCube();
  numcubes++;
}
Cubex::~Cubex()
{
  numcubes--;
}
int Cubex::numcubes = 0;
// version & author of the solver
const char* Cubex::ver = ".505";
const char* Cubex::author = "Eric Dietz (root@wrongway.org)";
// test the (in)eqaulity of two cubes
const bool Cubex::operator==(const Cubex &q)
{
  if (!(q.cubeinit) || !(this->cubeinit)) return false;
  bool n = true;
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      if
      (q.Cub[i][N+1][j] != this->Cub[i][N+1][j] ||
       q.Cub[i][j][0]   != this->Cub[i][j][0]   ||
       q.Cub[0][i][j]   != this->Cub[0][i][j]   ||
       q.Cub[i][j][N+1] != this->Cub[i][j][N+1] ||
       q.Cub[N+1][i][j] != this->Cub[N+1][i][j] ||
       q.Cub[i][0][j]   != this->Cub[i][0][j]  )
        n = false;
    }
  }
  if (q.cenfix && this->cenfix) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        if
        (q.Cub[i][N][j] != this->Cub[i][N][j] ||
         q.Cub[i][j][1] != this->Cub[i][j][1] ||
         q.Cub[1][i][j] != this->Cub[1][i][j] ||
         q.Cub[i][j][N] != this->Cub[i][j][N] ||
         q.Cub[N][i][j] != this->Cub[N][i][j] ||
         q.Cub[i][1][j] != this->Cub[i][1][j])
          n = false;
      }
    }
  }
  return n;
}
const bool Cubex::operator!=(const Cubex &a) { return !(operator==(a)); }
// point a pretty cube value to an ugly array value
int *Cubex::face(int x, int y, int z)
{
  if (x+2 < 0 || x+2 > N+1 || y+2 < 0 || y+2 > N+1 || z+2 < 0 || z+2 > N+1)
    return NULL;
  return &Cub[x+2][y+2][z+2];
}
// show cube on the screen (e.g., for debugging)
const void Cubex::RenderScreen()
{
/*
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) printf("  ");
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", Cub[j][N+1][N+1-i]);
    printf("\n");
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++)
      printf(" %i", Cub[0][N+1-i][N+1-j]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", Cub[j][N+1-i][0]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", Cub[N+1][N+1-i][j]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", Cub[N+1-j][N+1-i][N+1]);
    printf("\n");
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) printf("  ");
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", Cub[j][0][i]);
    printf("\n");
  }
*/
  printf(
"\
        %i %i %i     %i      %i\n\
        %i %i %i   %i %i %i %i\n\
        %i %i %i     %i\n\
 %i %i %i  %i %i %i  %i %i %i  %i %i %i\n\
 %i %i %i  %i %i %i  %i %i %i  %i %i %i\n\
 %i %i %i  %i %i %i  %i %i %i  %i %i %i\n\
        %i %i %i\n\
        %i %i %i\n\
        %i %i %i\n\
",
Cub[-1+2][ 2+2][ 1+2], Cub[ 0+2][ 2+2][ 1+2], Cub[ 1+2][ 2+2][ 1+2],
Cub[ 0+2][ 1+2][ 0+2], cenfix,
Cub[-1+2][ 2+2][ 0+2], Cub[ 0+2][ 2+2][ 0+2], Cub[ 1+2][ 2+2][ 0+2],
Cub[-1+2][ 0+2][ 0+2], Cub[ 0+2][ 0+2][-1+2], Cub[ 1+2][ 0+2][ 0+2], Cub[ 0+2][ 0+2][ 1+2],
Cub[-1+2][ 2+2][-1+2], Cub[ 0+2][ 2+2][-1+2], Cub[ 1+2][ 2+2][-1+2],
Cub[ 0+2][-1+2][ 0+2],
Cub[-2+2][ 1+2][ 1+2], Cub[-2+2][ 1+2][ 0+2], Cub[-2+2][ 1+2][-1+2],
Cub[-1+2][ 1+2][-2+2], Cub[ 0+2][ 1+2][-2+2], Cub[ 1+2][ 1+2][-2+2],
Cub[ 2+2][ 1+2][-1+2], Cub[ 2+2][ 1+2][ 0+2], Cub[ 2+2][ 1+2][ 1+2],
Cub[ 1+2][ 1+2][ 2+2], Cub[ 0+2][ 1+2][ 2+2], Cub[-1+2][ 1+2][ 2+2],
Cub[-2+2][ 0+2][ 1+2], Cub[-2+2][ 0+2][ 0+2], Cub[-2+2][ 0+2][-1+2],
Cub[-1+2][ 0+2][-2+2], Cub[ 0+2][ 0+2][-2+2], Cub[ 1+2][ 0+2][-2+2],
Cub[ 2+2][ 0+2][-1+2], Cub[ 2+2][ 0+2][ 0+2], Cub[ 2+2][ 0+2][ 1+2],
Cub[ 1+2][ 0+2][ 2+2], Cub[ 0+2][ 0+2][ 2+2], Cub[-1+2][ 0+2][ 2+2],
Cub[-2+2][-1+2][ 1+2], Cub[-2+2][-1+2][ 0+2], Cub[-2+2][-1+2][-1+2],
Cub[-1+2][-1+2][-2+2], Cub[ 0+2][-1+2][-2+2], Cub[ 1+2][-1+2][-2+2],
Cub[ 2+2][-1+2][-1+2], Cub[ 2+2][-1+2][ 0+2], Cub[ 2+2][-1+2][ 1+2],
Cub[ 1+2][-1+2][ 2+2], Cub[ 0+2][-1+2][ 2+2], Cub[-1+2][-1+2][ 2+2],
Cub[-1+2][-2+2][-1+2], Cub[ 0+2][-2+2][-1+2], Cub[ 1+2][-2+2][-1+2],
Cub[-1+2][-2+2][ 0+2], Cub[ 0+2][-2+2][ 0+2], Cub[ 1+2][-2+2][ 0+2],
Cub[-1+2][-2+2][ 1+2], Cub[ 0+2][-2+2][ 1+2], Cub[ 1+2][-2+2][ 1+2]
  );
}
// return true if cube is solved
const bool Cubex::IsSolved()
{
  if (!cubeinit) return false;
  int c[7], d; bool n = true;
  c[1] = Cub[2][N+1][2]; c[2] = Cub[2][2][0];
  c[3] = Cub[0][2][2];   c[4] = Cub[2][2][N+1];
  c[5] = Cub[N+1][2][2]; c[6] = Cub[2][0][2];
  for (int i = 1; i <= 6 && n == true; i++) {
    d = 0;
    for (int j = 1; j <= 6 && n == true; j++) {
      if (c[i] == j) d++;
    }
    if (d != 1)
      n = false;
  }
  for (int i = 1; i <= N && n == true; i++) {
    for (int j = 1; j <= N && n == true; j++) {
      if
      (Cub[i][N+1][j] != c[1] ||
       Cub[i][j][0]   != c[2] ||
       Cub[0][i][j]   != c[3] ||
       Cub[i][j][N+1] != c[4] ||
       Cub[N+1][i][j] != c[5] ||
       Cub[i][0][j]   != c[6])
        n = false;
    }
  }
  if (cenfix) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        if
        (Cub[i][N][j] != 0 ||
         Cub[i][j][1] != 0 ||
         Cub[1][i][j] != 0 ||
         Cub[i][j][N] != 0 ||
         Cub[N][i][j] != 0 ||
         Cub[i][1][j] != 0)
          n = false;
      }
    }
  }
  return n;
}
// create default (solved) cube model and put it in Cub[]
const void Cubex::ResetCube()
{
  solution = "";
  for (int i = 0; i <= MOV; i++) mov[i] = 0;
  for (int i = 0; i <= N+1; i++) {
    for (int j = 0; j <= N+1; j++) {
      for (int k = 0; k <= N+1; k++) {
        if (!(i-2 == 0 && j-2 == 0 && k-2 == 0))
          Cub[i][j][k] = 0;
      }
    }
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      Cub[i][N+1][j] = 1;
      Cub[i][j][0]   = 2;
      Cub[0][i][j]   = 3;
      Cub[i][j][N+1] = 4;
      Cub[N+1][i][j] = 5;
      Cub[i][0][j]   = 6;
    }
  }
  cubeinit = true;
  erval = 0;
}
// this is the series of cube rotation functions
// copy cube model so we can change it and remember the original
const void Cubex::Ctemp()
{
  for (int i = 0; i <= N+1; i++) {
    for (int j = 0; j <= N+1; j++) {
      for (int k = 0; k <= N+1; k++) {
        Tmp[i][j][k] = Cub[i][j][k];
      }
    }
  }
}
// rotate given slice left
const bool Cubex::XML(int a, bool n = true)
{
  if (a < 1 || a > N) return false;
  Ctemp();
  for (int i = 1; i <= N; i++) {
    if (a == 1)
      for (int j = 1; j <= N; j++)
        Cub[i][0][j] = Tmp[N+1-j][0][i];
    else if (a == N)
      for (int j = 1; j <= N; j++)
        Cub[i][N+1][j] = Tmp[N+1-j][N+1][i];
    Cub[i][a][0]   = Tmp[N+1][a][i];
    Cub[i][a][N+1] = Tmp[0][a][i];
    Cub[0][a][i]   = Tmp[N+1-i][a][0];
    Cub[N+1][a][i] = Tmp[N+1-i][a][N+1];
  }
  if (a == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][1][j] = Tmp[N+1-j][1][i];
        if (n) {
          Cub[i][1][j]--;
          if (Cub[i][1][j] < 0) Cub[i][1][j] += 4;
        }
      }
    }
  }
  else if (a == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][N][j] = Tmp[N+1-j][N][i];
        if (n) {
          Cub[i][N][j]++;
          if (Cub[i][N][j] > 3) Cub[i][N][j] -= 4;
        }
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      Cub[i][a][1] = Tmp[N][a][i];
      Cub[i][a][N] = Tmp[1][a][i];
      Cub[1][a][i] = Tmp[N+1-i][a][1];
      Cub[N][a][i] = Tmp[N+1-i][a][N];
    }
  }
  return true;
}
// rotate given slice right
const bool Cubex::XMR(int a, bool n = true)
{
  if (a < 1 || a > N) return false;
  Ctemp();
  for (int i = 1; i <= N; i++) {
    if (a == 1)
      for (int j = 1; j <= N; j++)
        Cub[i][0][j] = Tmp[j][0][N+1-i];
    else if (a == N)
      for (int j = 1; j <= N; j++)
        Cub[i][N+1][j] = Tmp[j][N+1][N+1-i];
    Cub[i][a][0]   = Tmp[0][a][N+1-i];
    Cub[i][a][N+1] = Tmp[N+1][a][N+1-i];
    Cub[0][a][i]   = Tmp[i][a][N+1];
    Cub[N+1][a][i] = Tmp[i][a][0];
  }
  if (a == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][1][j] = Tmp[j][1][N+1-i];
        if (n) {
          Cub[i][1][j]++;
          if (Cub[i][1][j] > 3) Cub[i][1][j] -= 4;
        }
      }
    }
  }
  else if (a == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][N][j] = Tmp[j][N][N+1-i];
        if (n) {
          Cub[i][N][j]--;
          if (Cub[i][N][j] < 0) Cub[i][N][j] += 4;
        }
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      Cub[i][a][1] = Tmp[1][a][N+1-i];
      Cub[i][a][N] = Tmp[N][a][N+1-i];
      Cub[1][a][i] = Tmp[i][a][N];
      Cub[N][a][i] = Tmp[i][a][1];
    }
  }
  return true;
}
// rotate given slice up
const bool Cubex::XMU(int a, bool n = true)
{
  if (a < 1 || a > N) return false;
  Ctemp();
  for (int i = 1; i <= N; i++) {
    if (a == 1)
      for (int j = 1; j <= N; j++)
        Cub[0][i][j] = Tmp[0][j][N+1-i];
    if (a == N)
      for (int j = 1; j <= N; j++)
        Cub[N+1][i][j] = Tmp[N+1][j][N+1-i];
    Cub[a][i][0]   = Tmp[a][0][N+1-i];
    Cub[a][i][N+1] = Tmp[a][N+1][N+1-i];
    Cub[a][0][i]   = Tmp[a][i][N+1];
    Cub[a][N+1][i] = Tmp[a][i][0];
  }
  if (a == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[1][i][j] = Tmp[1][j][N+1-i];
        if (n) {
          Cub[1][i][j]--;
          if (Cub[1][i][j] < 0) Cub[1][i][j] += 4;
        }
      }
    }
  }
  else if (a == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[N][i][j] = Tmp[N][j][N+1-i];
        if (n) {
          Cub[N][i][j]++;
          if (Cub[N][i][j] > 3) Cub[N][i][j] -= 4;
        }
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      Cub[a][i][1] = Tmp[a][1][N+1-i];
      Cub[a][i][N] = Tmp[a][N][N+1-i];
      Cub[a][1][i] = Tmp[a][i][N];
      Cub[a][N][i] = Tmp[a][i][1];
    }
  }
  return true;
}
// rotate given slice down
const bool Cubex::XMD(int a, bool n = true)
{
  if (a < 1 || a > N) return false;
  Ctemp();
  for (int i = 1; i <= N; i++) {
    if (a == 1)
      for (int j = 1; j <= N; j++)
        Cub[0][i][j] = Tmp[0][N+1-j][i];
    if (a == N)
      for (int j = 1; j <= N; j++)
        Cub[N+1][i][j] = Tmp[N+1][N+1-j][i];
    Cub[a][i][0]   = Tmp[a][N+1][i];
    Cub[a][i][N+1] = Tmp[a][0][i];
    Cub[a][0][i]   = Tmp[a][N+1-i][0];
    Cub[a][N+1][i] = Tmp[a][N+1-i][N+1];
  }
  if (a == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[1][i][j] = Tmp[1][N+1-j][i];
        if (n) {
          Cub[1][i][j]++;
          if (Cub[1][i][j] > 3) Cub[1][i][j] -= 4;
        }
      }
    }
  }
  else if (a == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[N][i][j] = Tmp[N][N+1-j][i];
        if (n) {
          Cub[N][i][j]--;
          if (Cub[N][i][j] < 0) Cub[N][i][j] += 4;
        }
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      Cub[a][i][1] = Tmp[a][N][i];
      Cub[a][i][N] = Tmp[a][1][i];
      Cub[a][1][i] = Tmp[a][N+1-i][1];
      Cub[a][N][i] = Tmp[a][N+1-i][N];
    }
  }
  return true;
}
// rotate given slice clockwise
const bool Cubex::XMC(int a, bool n = true)
{
  if (a < 1 || a > N) return false;
  Ctemp();
  for (int i = 1; i <= N; i++) {
    if (a == 1)
      for (int j = 1; j <= N; j++)
        Cub[i][j][0] = Tmp[N+1-j][i][0];
    if (a == N)
      for (int j = 1; j <= N; j++)
        Cub[i][j][N+1] = Tmp[N+1-j][i][N+1];
    Cub[i][0][a]   = Tmp[N+1][i][a];
    Cub[i][N+1][a] = Tmp[0][i][a];
    Cub[0][i][a]   = Tmp[N+1-i][0][a];
    Cub[N+1][i][a] = Tmp[N+1-i][N+1][a];
  }
  if (a == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][j][1] = Tmp[N+1-j][i][1];
        if (n) {
          Cub[i][j][1]++;
          if (Cub[i][j][1] > 3) Cub[i][j][1] -= 4;
        }
      }
    }
  }
  else if (a == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][j][N] = Tmp[N+1-j][i][N];
        if (n) {
          Cub[i][j][N]--;
          if (Cub[i][j][N] < 0) Cub[i][j][N] += 4;
        }
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      Cub[i][1][a] = Tmp[N][i][a];
      Cub[i][N][a] = Tmp[1][i][a];
      Cub[1][i][a] = Tmp[N+1-i][1][a];
      Cub[N][i][a] = Tmp[N+1-i][N][a];
    }
  }
  return true;
}
// rotate given slice anticlockwise (looking from the front)
const bool Cubex::XMA(int a, bool n = true)
{
  if (a < 1 || a > N) return false;
  Ctemp();
  for (int i = 1; i <= N; i++) {
    if (a == 1)
      for (int j = 1; j <= N; j++)
        Cub[i][j][0] = Tmp[j][N+1-i][0];
    if (a == N)
      for (int j = 1; j <= N; j++)
        Cub[i][j][N+1] = Tmp[j][N+1-i][N+1];
    Cub[i][0][a]   = Tmp[0][N+1-i][a];
    Cub[i][N+1][a] = Tmp[N+1][N+1-i][a];
    Cub[0][i][a]   = Tmp[i][N+1][a];
    Cub[N+1][i][a] = Tmp[i][0][a];
  }
  if (a == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][j][1] = Tmp[j][N+1-i][1];
        if (n) {
          Cub[i][j][1]--;
          if (Cub[i][j][1] < 0) Cub[i][j][1] += 4;
        }
      }
    }
  }
  else if (a == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        Cub[i][j][N] = Tmp[j][N+1-i][N];
        if (n) {
          Cub[i][j][N]++;
          if (Cub[i][j][N] > 3) Cub[i][j][N] -= 4;
        }
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      Cub[i][1][a] = Tmp[1][N+1-i][a];
      Cub[i][N][a] = Tmp[N][N+1-i][a];
      Cub[1][i][a] = Tmp[i][N][a];
      Cub[N][i][a] = Tmp[i][1][a];
    }
  }
  return true;
}
// the remaining rotation functions are aliases to the above rotators (with nicer names)
const void Cubex::UL() { XML(N); } // rotate top left
const void Cubex::UR() { XMR(N); } // rotate top right
const void Cubex::DL() { XML(1); } // rotate bottom left
const void Cubex::DR() { XMR(1); } // rotate bottom right
const void Cubex::LU() { XMU(1); } // rotate left up
const void Cubex::LD() { XMD(1); } // rotate left down
const void Cubex::RU() { XMU(N); } // rotate right up
const void Cubex::RD() { XMD(N); } // rotate right down
const void Cubex::FC() { XMC(1); } // rotate front clockwise
const void Cubex::FA() { XMA(1); } // rotate front anticlockwise
const void Cubex::BC() { XMC(N); } // rotate back clockwise (looking from front)
const void Cubex::BA() { XMA(N); } // rotate back anticlockwise (looking from front)
const void Cubex::ML() { XML(2); } // rotate middle left
const void Cubex::MR() { XMR(2); } // rotate middle right
const void Cubex::MU() { XMU(2); } // rotate middle up
const void Cubex::MD() { XMD(2); } // rotate middle down
const void Cubex::MC() { XMC(2); } // rotate middle clockwise
const void Cubex::MA() { XMA(2); } // rotate middle anticlockwise
const void Cubex::CL() { for (int i = 1; i <= N; i++) XML(i); } // rotate whole cube left
const void Cubex::CR() { for (int i = 1; i <= N; i++) XMR(i); } // rotate whole cube right
const void Cubex::CU() { for (int i = 1; i <= N; i++) XMU(i); } // rotate whole cube up
const void Cubex::CD() { for (int i = 1; i <= N; i++) XMD(i); } // rotate whole cube down
const void Cubex::CC() { for (int i = 1; i <= N; i++) XMC(i); } // rotate whole cube clockwise
const void Cubex::CA() { for (int i = 1; i <= N; i++) XMA(i); } // rotate whole cube anticlockwise
const void Cubex::XCL() { for (int i = 1; i <= N; i++) XML(i, false); } // rotate whole cube left w/o altering centers
const void Cubex::XCR() { for (int i = 1; i <= N; i++) XMR(i, false); } // rotate whole cube right w/o altering centers
const void Cubex::XCU() { for (int i = 1; i <= N; i++) XMU(i, false); } // rotate whole cube up w/o altering centers
const void Cubex::XCD() { for (int i = 1; i <= N; i++) XMD(i, false); } // rotate whole cube down w/o altering centers
const void Cubex::XCC() { for (int i = 1; i <= N; i++) XMC(i, false); } // rotate whole cube clockwise w/o altering centers
const void Cubex::XCA() { for (int i = 1; i <= N; i++) XMA(i, false); } // rotate whole cube anticlockwise w/o altering centers
// scramble up the cube model
const void Cubex::ScrambleCube()
{
  // come up with a better calculation for a good number of random moves based on cube size later
  int a = (N-2)*25+10;
  int n, o; string s = "";
  a += rand() % a;
  ResetCube();
  for (int i = 1; i <= a; i++) {
    n = rand() % 6 + 1; // which dimension & direction
    o = rand() % N + 1; // which layer
    switch (n) {
      case 1: XML(o); break;
      case 2: XMR(o); break;
      case 3: XMU(o); break;
      case 4: XMD(o); break;
      case 5: XMC(o); break;
      case 6: XMA(o); break;
    }
  }
  cubeinit = true;
}
// execute solution
const void Cubex::DoSolution()
{
  if (!cubeinit) return;
  string a = "";
  for (int i = 1; i <= mov[0]; i++) {
    a = solution.substr(i * 3 - 3, 3);
    if      (a == "UL.") UL();
    else if (a == "UR.") UR();
    else if (a == "DL.") DL();
    else if (a == "DR.") DR();
    else if (a == "LU.") LU();
    else if (a == "LD.") LD();
    else if (a == "RU.") RU();
    else if (a == "RD.") RD();
    else if (a == "FC.") FC();
    else if (a == "FA.") FA();
    else if (a == "BC.") BC();
    else if (a == "BA.") BA();
    else if (a == "ML.") ML();
    else if (a == "MR.") MR();
    else if (a == "MU.") MU();
    else if (a == "MD.") MD();
    else if (a == "MC.") MC();
    else if (a == "MA.") MA();
    else if (a == "CL.") CL();
    else if (a == "CR.") CR();
    else if (a == "CU.") CU();
    else if (a == "CD.") CD();
    else if (a == "CC.") CC();
    else if (a == "CA.") CA();
  }
}
// return adjacent axis on which given center is found
const int Cubex::FindCent(int a)
{
  int f = 0, x, y, z; fx = 0; fy = 0; fz = 0;
  for (int i = -1; i <= 1; i = i + 2) {
    x = Cub[i*2+2][0+2][0+2];
    y = Cub[0+2][i*2+2][0+2];
    z = Cub[0+2][0+2][i*2+2];
    if      (x == a) {
      f = i * 1; fx = i * 2; return f;
    }
    else if (y == a) {
      f = i * 2; fy = i * 2; return f;
    }
    else if (z == a) {
      f = i * 3; fz = i * 2; return f;
    }
  }
  return f;
}
// return adjacent axis on which given edge is found
const int Cubex::FindEdge(int a, int b)
{
  int f = 0, x, y, z; fx = 0; fy = 0; fz = 0;
  for (int i = -1; i <= 1; i = i + 2) {
    for (int j = -1; j <= 1; j = j + 2) {
      x = Cub[i*2+2][j+2][0+2];
      y = Cub[i+2][j*2+2][0+2];
      if      (x == a && y == b) {
        f = i * 1; fx = i * 2; fy = j; return f;
      }
      else if (y == a && x == b) {
        f = j * 2; fx = i; fy = j * 2; return f;
      }
      x = Cub[i*2+2][0+2][j+2];
      z = Cub[i+2][0+2][j*2+2];
      if      (x == a && z == b) {
        f = i * 1; fx = i * 2; fz = j; return f;
      }
      else if (z == a && x == b) {
        f = j * 3; fx = i; fz = j * 2; return f;
      }
      y = Cub[0+2][i*2+2][j+2];
      z = Cub[0+2][i+2][j*2+2];
      if      (y == a && z == b) {
        f = i * 2; fy = i * 2; fz = j; return f;
      }
      else if (z == a && y == b) {
        f = j * 3; fy = i; fz = j * 2; return f;
      }
    }
  }
  return f;
}
// return adjacent axis on which given corner is found
const int Cubex::FindCorn(int a, int b, int c)
{
  int f = 0, x, y, z; fx = 0; fy = 0; fz = 0;
  for (int i = -1; i <= 1; i = i + 2) {
    for (int j = -1; j <= 1; j = j + 2) {
      for (int k = -1; k <= 1; k = k + 2) {
        x = Cub[i*2+2][j+2][k+2];
        y = Cub[i+2][j*2+2][k+2];
        z = Cub[i+2][j+2][k*2+2];
        if      (x == a && (y == b || y == c) && (z == b || z == c)) {
          f = i * 1; fx = i * 2; fy = j; fz = k; return f;
        }
        else if (y == a && (x == b || x == c) && (z == b || z == c)) {
          f = j * 2; fx = i; fy = j * 2; fz = k; return f;
        }
        else if (z == a && (x == b || x == c) && (y == b || y == c)) {
          f = k * 3; fx = i; fy = j; fz = k * 2; return f;
        }
      }
    }
  }
  return f;
}
// solve the cube (uses many other routines also)
// slightly complicated...
const int Cubex::SolveCube()
{
  // make sure cube was initialized
  if (!cubeinit) return 1;
  // set up buffers and counters and such...
  int Rub[5][5][5], Fac[7][2], mvs[MOV+1], n;
  static int m = -1;
  static string s = ""; 
  string t = ""; string p = "";
  // make sure that the cube has the proper cubelets...
  cubeinit = false;
  // check that all the centers are present
  for (int i = 1; i <= 6; i++) {
    if (FindCent(i) == 0) {
      erval = 1; return erval;
    }
  }
  // buffer the cube so we can interpolate it to a specific color arrangement to check for edges and corners...
  for (int i = -2; i <= 2; i++) {
    for (int j = -2; j <= 2; j++) {
      for (int k = -2; k <= 2; k++) {
        Rub[i+2][j+2][k+2] = Cub[i+2][j+2][k+2];
      }
    }
  }
  // interpolate the cube...
  Fac[0][0] = 0;
  Fac[1][0] = Cub[0+2][2+2][0+2]; Fac[2][0] = Cub[0+2][0+2][-2+2];
  Fac[3][0] = Cub[-2+2][0+2][0+2]; Fac[4][0] = Cub[0+2][0+2][2+2];
  Fac[5][0] = Cub[2+2][0+2][0+2]; Fac[6][0] = Cub[0+2][-2+2][0+2];
  for (int i = 0; i <= 6; i++) {
    Fac[Fac[i][0]][1] = i;
  }
  // apply interpolation
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      Cub[i+2][2+2][j+2] = Fac[Cub[i+2][2+2][j+2]][1];
      Cub[i+2][j+2][-2+2] = Fac[Cub[i+2][j+2][-2+2]][1];
      Cub[-2+2][i+2][j+2] = Fac[Cub[-2+2][i+2][j+2]][1];
      Cub[i+2][j+2][2+2] = Fac[Cub[i+2][j+2][2+2]][1];
      Cub[2+2][i+2][j+2] = Fac[Cub[2+2][i+2][j+2]][1];
      Cub[i+2][-2+2][j+2] = Fac[Cub[i+2][-2+2][j+2]][1];
    }
  }
  // check that all edges and corners are present
  for (int i = 1; i <= 4; i++) {
    int j = 1;
    if (i < 4) j = i + 1;
    if (FindEdge(1, i + 1) == 0 ||
     FindEdge(6, i + 1) == 0 ||
     FindEdge(i + 1, j + 1) == 0 ||
     FindCorn(1, i + 1, j + 1) == 0 ||
     FindCorn(6, i + 1, j + 1) == 0) {
      m = 0;
    }
  }
  // return cube to pre-interpolated status
  for (int i = -2; i <= 2; i++) {
    for (int j = -2; j <= 2; j++) {
      for (int k = -2; k <= 2; k++) {
        Cub[i+2][j+2][k+2] = Rub[i+2][j+2][k+2];
      }
    }
  }
  // if any flags went off during checking then return error 1 (improper cubelets)
  if (m == 0) { erval = 1; return erval; }
  cubeinit = true;
  // cube seems to have ok cubelets, so try to solve it...
  for (int i = 1; i <= MOV; i++) mvs[i] = 0;
  int num_moves = 0;
  
  struct timeval tp;
  double sec, usec, start, end;
  gettimeofday(&tp, NULL);
  sec = static_cast<double>(tp.tv_sec);
  usec = static_cast<double>(tp.tv_usec) / 1E6;
  start = sec + usec;
  
  omp_set_dynamic(0);
  omp_set_num_threads(8);
  // try to solve the cube from each possile starting orientation (to find the fastest solution)...	
  #pragma omp parallel for
  for (int q = 1; q <= 24; q++) {
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
      __transaction_atomic{
        m = co.numMoves;
      }
       s = co.solution;
      for (int i = 1; i <= MOV; i++) {
        mvs[i] = mov[i];
      }
    }
  }
  //printf("Total moves for 24 orientations is %d.\n", num_moves);
  
  gettimeofday(&tp, NULL);
  sec = static_cast<double>(tp.tv_sec);
  usec = static_cast<double>(tp.tv_usec) / 1E6;
  end = sec + usec;
  double time = end - start;
  printf("The elapsed clock cycles are: %f.\n", time);
  
  // set mov array...
  for (int i = 1; i <= MOV; i++) {
    mov[i] = mvs[i];
  }
  // return error if one was found
  if (!cubeinit) return erval;
  mov[0] = m;
  // set solution and return...
  solution = s;
  return 0;
}
// end of cube class definitions
