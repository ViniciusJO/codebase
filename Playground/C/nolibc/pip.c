typedef struct {
  double x;
  double y;
} Point;

typedef struct {
  int length;
  Point *vertices;
} Polygon;

int pnpoly(Polygon poly, Point point) {
  int i, j, c = 0;
  for (i = 0, j = poly.length; i < poly.length; j = i++) {
    if ( ((poly.vertices[i].y>point.y) != (poly.vertices[j].y>point.y)) &&
     (point.x < (poly.vertices[j].x-poly.vertices[i].x) * (point.y-poly.vertices[i].y) / (poly.vertices[j].y-poly.vertices[i].y) + poly.vertices[i].x) )
       c = !c;
  }
  return c;
}

extern int max(int, int);

int max(int a, int b) {
  return a > b ? a : b;
}

/*int main(int argc, char **argv) {*/
/*  (void)argv;*/
/*  return max(1,argc-1);*/
/*}*/

//int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy) {
//  int i, j, c = 0;
//  for (i = 0, j = nvert-1; i < nvert; j = i++) {
//    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
//     (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
//       c = !c;
//  }
//  return c;
//}


