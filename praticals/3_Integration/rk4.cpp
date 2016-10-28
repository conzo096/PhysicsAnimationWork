 #include "main.h"
#include <glm/glm.hpp>
using namespace std;
using namespace glm;

static dvec3 gravity = dvec3(0, -10.0, 0);
struct Derivative {
  dvec3 dx, dv;
};

dvec3 acceleration(const sBall &body, dvec3 x, dvec3 v, float t) {
  // we could be summing accelerations, or doing other cool things here
  return dvec3(0, -10, 0);
}

Derivative compute(const sBall &body, const double t, const double dt, const Derivative &d) {
  //where would we be and how fast at dt 
  dvec3 x = body.position + d.dx * dt;
  dvec3 v = body.velocity + d.dv * dt;

  Derivative output;
  output.dx = v;
  //what would the acceleration be at this point?
  // *********************************
  output.dv = acceleration(body,output.dx,output.dv,dt);
  // *********************************
  return output;
}

void UpdatePhysics_rk4(const double t, const double dt) {
  for (size_t i = 0; i < balls.size(); i++) {
    Derivative a, b, c, d;
    //Incrementally compute for various dt
    a = compute(balls[i], t, 0.0f, {dvec3(0), dvec3(0)});
    b = compute(balls[i], t, dt * 0.5f, a);
    c = compute(balls[i], t, dt * 0.5f, b);
    d = compute(balls[i], t, dt, c);

    //compute the final derivitive
    // *********************************
	balls[i].position += (a.dx + (2.0 * a.dx) + (2.0 * a.dx) + a.dx) * dt / 6.0;
	balls[i].velocity += (a.dv + (2.0 * a.dv) + (2.0 * a.dv) + a.dv) * dt / 6.0;
    //apply against dt and get final outputs


    // *********************************

    if (balls[i].position.y <= 0.0f) {
      balls[i].velocity.y = -balls[i].velocity.y;
    }
  }
}