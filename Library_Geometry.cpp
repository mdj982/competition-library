namespace geometry {
  struct coordinate {
    double x, y;
    bool operator<(const coordinate & another) const {
      return x != another.x ? x < another.x : y < another.y;
    }
  };
  struct line {
    double a, b, c;
    pair<double, double> domain; // if b == 0, domain means range 
  };
  enum linetype {
    LINE, HALFLINE, SEGMENT
  };
  coordinate projection(coordinate, line);
  double Euclid_dist(coordinate p, coordinate q) {
    return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
  }
  // ax + by + c = 0
  line make_line(double a, double b, double c, pair<double, double> domain = { -INFD, INFD }) {
    return{ a,b,c,domain };
  }
  // line from p through q when type is HALFLINE
  line make_line_through(coordinate p, coordinate q, linetype type = LINE) {
    line ret = { NAN, NAN, NAN,{ NAN,NAN } };
    if (p.x == q.x && p.y == q.y) return ret;
    else {
      if (p.x == q.x) {
        ret = { 1, 0, -p.x,{ -INFD, INFD } };
        switch (type) {
        case SEGMENT:
          if (p.y > q.y) ret.domain = { q.y, p.y };
          else ret.domain = { p.y, q.y };
          return ret;
        case HALFLINE:
          if (p.y > q.y) ret.domain = { -INFD, p.y };
          else ret.domain = { p.y, INFD };
          return ret;
        case LINE:
          return ret;
        }
      }
      else {
        ret = { (q.y - p.y), (p.x - q.x), q.x * p.y - p.x * q.y,{ -INFD, INFD } };
        switch (type) {
        case SEGMENT:
          if (p.x > q.x) ret.domain = { q.x, p.x };
          else ret.domain = { p.x, q.x };
          return ret;
        case HALFLINE:
          if (p.x > q.x) ret.domain = { -INFD, p.x };
          else ret.domain = { p.x, INFD };
          return ret;
        case LINE:
          return ret;
        }
      }
    }
  }
  // line through p
  line make_line_from_slope(double a, double b, coordinate p) {
    double c = -(a * p.x + b * p.y);
    return{ a, b, c,{ -INFD, INFD } };
  }
  // get y when substituting x for l
  double subst_x(double x, line l) {
    if (l.b == 0) {
      return -l.c / l.a;
    }
    else {
      return -(l.a * x + l.c) / l.b;
    }
  }
  // get x when substituting y for l
  double subst_y(double y, line l) {
    if (l.a == 0) {
      return -l.c / l.b;
    }
    else {
      return -(l.b * y + l.c) / l.a;
    }
  }
  class Two_Lines {
  private:
    line l;
    line m;
    coordinate crosspoint; // (NAN, NAN) when not existing
    bool in_range(coordinate p, line l) {
      if (l.b == 0) {
        if (l.domain.first <= p.y && p.y <= l.domain.second) return true;
        else return false;
      }
      else {
        if (l.domain.first <= p.x && p.x <= l.domain.second) return true;
        else return false;
      }
    }
  public:
    bool is_intersect; // include having common interval
    bool is_parallel; // include having common interval
    bool is_orthogonal;
    Two_Lines(line l, line m) {
      Two_Lines::l = l;
      Two_Lines::m = m;
      is_intersect = false;
      is_parallel = false;
      is_orthogonal = false;
      crosspoint = { NAN, NAN };
      double dn = l.a * m.b - m.a * l.b;
      if (dn == 0) {
        is_parallel = true;
        if (l.a * m.c - m.a * l.c);
        else if (l.b * m.c - m.b * l.c);
        else {
          if (l.domain.first <= m.domain.second && m.domain.first <= l.domain.second) is_intersect = true;
          else is_intersect = false;
        }
      }
      else {
        if (!(l.a * m.a + l.b * m.b)) is_orthogonal = true;
        crosspoint.x = (l.b * m.c - m.b * l.c) / dn;
        crosspoint.y = (m.a * l.c - l.a * m.c) / dn;
        if (in_range(crosspoint, l) && in_range(crosspoint, m)) {
          is_intersect = true;
        }
        else {
          crosspoint = { NAN, NAN };
        }
      }
    }
    double get_distance() {
      if (is_intersect) return 0;
      else {
        double ret = INFD;
        coordinate p[2], q[2];
        if (l.b == 0) {
          p[0] = { subst_y(l.domain.first, l), l.domain.first };
          p[1] = { subst_y(l.domain.second, l), l.domain.second };
        }
        else {
          p[0] = { l.domain.first, subst_x(l.domain.first, l) };
          p[1] = { l.domain.second, subst_x(l.domain.second, l) };
        }
        if (m.b == 0) {
          q[0] = { subst_y(m.domain.first, m), m.domain.first };
          q[1] = { subst_y(m.domain.second, m), m.domain.second };
        }
        else {
          q[0] = { m.domain.first, subst_x(m.domain.first, m) };
          q[1] = { m.domain.second, subst_x(m.domain.second, m) };
        }
        Loop(i, 2) {
          ret = min(ret, Euclid_dist(projection(p[i], m), p[i]));
          ret = min(ret, Euclid_dist(projection(q[i], l), q[i]));
          Loop(j, 2) {
            ret = min(ret, Euclid_dist(p[i], q[j]));
          }
        }
        return ret;
      }
    }
    coordinate get_crosspoint() {
      return crosspoint;
    }
  };
  double poligon_area(vector<coordinate> vp) {
    double ret = 0;
    int n = vp.size();
    if (n <= 2) return 0;
    vp.push_back(vp[0]);
    Loop(i, n) {
      ret += vp[i].x * vp[i + 1].y - vp[i + 1].x * vp[i].y;
    }
    ret = abs(ret) / 2;
    return ret;
  }
  // solve q on l such that pq is orthogonal to l
  coordinate projection(coordinate p, line l) {
    line m = make_line_from_slope(l.b, -l.a, p);
    Two_Lines twl(l, m);
    return twl.get_crosspoint();
  }
}
