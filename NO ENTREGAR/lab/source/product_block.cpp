int main() {
  constexpr int maxsize = 100;

  double a[maxsize][maxsize] = {};
  double b[maxsize][maxsize] = {};
  double c[maxsize][maxsize];

  constexpr int bsize = 20;  
  static_assert(maxsize % bsize == 0,
      "size must be multiple of blocksize");

  for (int bj=0; bj<maxsize; bj+=bsize) {
    for (int bk=0; bk<maxsize; bk+=bsize) {
      for (int i=0; i<maxsize; ++i) {
        for (int j=bj; j<bj+bsize; ++j) {
          double r=0;
          for (int k=bk; k<bk+bsize; ++k) {
            r += a[i][k] * b[k][j];
          }
          c[i][j] += r;
        }
      }
    }
  }              
	
  return 0;
}
