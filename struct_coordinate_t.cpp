typedef ll coordinate_t;
struct coordinate {
  coordinate_t x, y;
};

/*
after trimming, s -> (0, 0), t -> placed in the first quadrant or x = 0 (y >= 0)
*/
class Board_Trimming {
private:
  int rot_angle;
  coordinate move;
public:
  Board_Trimming(coordinate s, coordinate t) {
    move = { s.x * -1, s.y * -1 };
    if (s.x == t.x && s.y == t.y) rot_angle = 0;
    else if (s.x <= t.x && s.y < t.y) rot_angle = 0;
    else if (s.x < t.x && s.y >= t.y) rot_angle = 90;
    else if (s.x >= t.x && s.y > t.y) rot_angle = 180;
    else rot_angle = 270;
  }
  coordinate trans(coordinate a) {
    coordinate ret;
    a.x += move.x;
    a.y += move.y;
    if (rot_angle == 0) ret = { a.x, a.y };
    else if (rot_angle == 90) ret = { a.y * -1, a.x };
    else if (rot_angle == 180) ret = { a.x * -1, a.y * -1 };
    else ret = { a.y, a.x * -1 };
    return ret;
  }
  coordinate invtrans(coordinate a) {
    coordinate ret;
    if (rot_angle == 0) ret = { a.x, a.y };
    else if (rot_angle == 90) ret = { a.y, a.x * -1 };
    else if (rot_angle == 180) ret = { a.x * -1, a.y * -1 };
    else ret = { a.y * -1, a.x };
    ret.x -= move.x;
    ret.y -= move.y;
    return ret;
  }
};