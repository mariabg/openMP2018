int main() {
  constexpr int maxsize = 100;

  double a[maxsize][maxsize] = {};
  double b[maxsize][maxsize] = {};
  double c[maxsize][maxsize];

  for (int i=0; i<maxsize; ++i) {
    for (int j=0; j<maxsize; ++j) {
      double r=0;
      for (int k=0; k<maxsize; ++k) {
        r += a[i][k] * b[k][j];
      }
      c[i][j] += r;
    }
  }
	
  return 0;
}
