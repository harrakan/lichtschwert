
#include "rgb_conversion.h"
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#include <math.h>


#define M_PI_3   1.0471975512 /* pi/3 */

int calcP(float H, float S, float I)  {
  return 255 * I / 3 * (1 + S * cos(H) / cos(M_PI_3 - H));
}

int calcQ(float H, float S, float I) {
  return 255 * I / 3 * (1 + S * (1 - cos(H) / cos(M_PI_3 - H)));
}

int calcT(float H, float S, float I) {
  return 255 * I / 3 * (1 - S);
}

uint32_t hsi2rgb(float H, float S, float I) {
 uint8_t r,g,b;
  
  H = fmod(H, 360); // cycle H around to 0-360 degrees
  H = M_PI * H / (float)180; // Convert to radians.
  S = constrain(S, 0, 1);
  I = constrain(I, 0, 1);


  if (H < 2 * M_PI_3) {
    r = calcP(H, S, I);
    g = calcQ(H, S, I);
    b = calcT(H, S, I);
  } else if (H < 4 * M_PI_3) {
    H = H - 2 * M_PI_3;
    g = calcP(H, S, I);
    b = calcQ(H, S, I);
    r = calcT(H, S, I);
  } else {
    H = H - 4 * M_PI_3;
    b = calcP(H, S, I);
    r = calcQ(H, S, I);
    g = calcT(H, S, I);
  }
  
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}