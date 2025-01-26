#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif


float clamp(float value, float minimum, float maximum);
float map(float value, float minin, float maxin, float minout, float maxout);
bool ends_with(const char* str, const char* substr);
bool parse_hex_color(const char* hex, int* r, int* g, int* b);


#ifdef __cplusplus
}
#endif

#endif

