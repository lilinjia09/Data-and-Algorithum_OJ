// ======================= CG 迭代求解器 =======================
struct MyCGSolver {
  const MySparseMatrix *A = nullptr;
  int maxIter = 2000;
  double tol = 1e-12;

  void compute(const MySparseMatrix &A_) { A = &A_; }

  vector<double> solve(const vector<double> &b) const {
    int n = A->n;

    vector<double> x(n, 0.0);
    vector<double> r = b;
    vector<double> p = b;

    auto dot = [&](const vector<double> &a, const vector<double> &b) {
      double s = 0;
      for (int i = 0; i < n; i++)
        s += a[i] * b[i];
      return s;
    };

    double rsold = dot(r, r);

    for (int it = 0; it < maxIter; it++) {
      vector<double> Ap = A->mul(p);
      double pAp = dot(p, Ap);
      double alpha = rsold / (pAp == 0 ? 1e-30 : pAp);

      for (int i = 0; i < n; i++) {
        x[i] += alpha * p[i];
        r[i] -= alpha * Ap[i];
      }
      double rsnew = dot(r, r);

      if (sqrt(rsnew) < tol)
        break;

      double beta = rsnew / rsold;
      for (int i = 0; i < n; i++)
        p[i] = r[i] + beta * p[i];

      rsold = rsnew;
    }
    return x;
  }
};
