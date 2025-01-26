#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

float clamp(float value, float minimum, float maximum)
{
   return fmin(fmax(value, minimum), maximum);
}

float map(float value, float minin, float maxin, float minout, float maxout)
{
   return minout + (value - minin) / (maxin - minin) * (maxout - minout);
}

bool ends_with(const char* str, const char* substr)
{
   int lenOfStr = strlen(str);
   int lenOfSubstr = strlen(substr);

   if (lenOfSubstr > lenOfStr)
   {
      return false;
   }

   for (int i = 0; i < lenOfSubstr; i++)
   {
      if (str[i + lenOfStr - lenOfSubstr] != substr[i])
      {
         return false;
      }
   }

   return true;
}

static int hex_to_decimal(char n1, char n2)
{
   int d1 = n1 >= '0' && n1 <= '9' ?
           n1 - '0' :
           (
            n1 >= 'A' && n1 <= 'F' ? n1 - 'A' + 10 : 0
           );
   int d2 = n2 >= '0' && n2 <= '9' ?
           n2 - '0' :
           (
            n2 >= 'A' && n2 <= 'F' ? n2 - 'A' + 10 : 0
           );
   return d2 + d1 * 16;
}

bool parse_hex_color(const char* hex, int* r, int* g, int* b)
{
   *r = 128;
   *g = 0;
   *b = 128;
   if (strlen(hex) != 6)
   {
      return false;
   }

   *r = hex_to_decimal(hex[0], hex[1]);
   *g = hex_to_decimal(hex[2], hex[3]);
   *b = hex_to_decimal(hex[4], hex[5]);

   return true;
}
