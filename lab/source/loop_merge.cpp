int main(){
  constexpr int maxsize = 100000;
  double z[maxsize], t[maxsize], u[maxsize], v[maxsize];

  for (int i=0; i<maxsize; ++i) {
    u[i] = z[i] + t[i];
  }
  for (int i=0; i<maxsize; ++i) {
    v[i] = u[i] + t[i];
  }

  return 0;
}
