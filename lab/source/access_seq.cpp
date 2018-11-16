int main() {
  constexpr int maxsize = 200;
  double a[maxsize][maxsize] = {};
  double b[maxsize][maxsize] = {};
  double c[maxsize][maxsize];

  for (int i=0; i<maxsize; ++i) {
    for (int j=0; j<maxsize; ++j) {
      c[i][j] = a[i][j] + b[i][j];
    }
  }

  return 0;
}
