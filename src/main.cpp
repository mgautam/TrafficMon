#include "common.h"


int main(int argc, char* argv[])
{
  intersection* i1 = new intersection(0,0);
  intersection* i2 = new intersection(4,0);
  intersection* i3 = new intersection(0,4);
  intersection* i4 = new intersection(-4,0);
  intersection* i5 = new intersection(0,-4);

  road* r12 = new road(i1, i2);
  road* r13 = new road(i1, i3);
  road* r14 = new road(i1, i4);
  road* r15 = new road(i1, i5);

  road* r21 = new road(i2, i1);
  road* r31 = new road(i3, i1);
  road* r41 = new road(i4, i1);
  road* r51 = new road(i5, i1);
}
