// ======================= 稀疏矩阵 Triplet =======================
struct MyTriplet {
  int r, c;
  double v;
  MyTriplet(int r_, int c_, double v_) : r(r_), c(c_), v(v_) {}
};

// ======================= CSR 稀疏矩阵 =======================
struct MySparseMatrix {
  int n;
  vector<int> row_ptr, col;
  vector<double> val;

  MySparseMatrix(int n_ = 0) : n(n_) {}

  void setFromTriplets(const vector<MyTriplet> &tri_src) {
    vector<MyTriplet> tri = tri_src;

    // 显式类型比较器
    sort(tri.begin(), tri.end(), [](const MyTriplet &a, const MyTriplet &b) {
      return (a.r < b.r) || (a.r == b.r && a.c < b.c);
    });

    row_ptr.assign(n + 1, 0);

    // 不用 auto
    for (size_t i = 0; i < tri.size(); i++) {
      row_ptr[tri[i].r + 1]++;
    }

    for (int i = 0; i < n; i++)
      row_ptr[i + 1] += row_ptr[i];

    int nnz = tri.size();
    col.resize(nnz);
    val.resize(nnz);

    vector<int> cur = row_ptr;

    for (size_t i = 0; i < tri.size(); i++) {
      const MyTriplet &t = tri[i];
      int pos = cur[t.r]++;
      col[pos] = t.c;
      val[pos] = t.v;
    }
  }

  vector<double> mul(const vector<double> &x) const {
    vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
      double s = 0;
      for (int k = row_ptr[i]; k < row_ptr[i + 1]; k++)
        s += val[k] * x[col[k]];
      y[i] = s;
    }
    return y;
  }
};
