#include "read_file.h"

void
skip_number (char *s, int &ind)
{
  while (s[ind] != '#' && s[ind] != '\0' && !(isspace(s[ind])))
    {
     s[ind] = ' ';
     ind++;
    }
}

void
skip_space_symbols (char *s, int &ind, FILE *f)
{
  while (true)
    {
      if (s[ind] == '#' || s[ind] == '\0')
        {
          char *symbols = fgets (s, LEN, f);
          ind = 0;
          if (symbols == nullptr)
            {
              s[ind] = '\0';
              return;
            }
        }
      else if (isspace (s[ind]))
        {
          ind++;
        }
      else
        {
          break;
        }
    }
}

std::optional<std::vector<std::pair<double, double>>>
read (const char * file)
{
  FILE * f = fopen (file, "r");

  if (f == nullptr)
    {
      printf("[ERROR] Can not open file \"%s\"\n", file);
      return {};
    }

  char buf[LEN] = {};
  int ind = 0;
  double temp = 0.;
  size_t max = 4 , counter = 0;
  std::vector<std::pair<double, double>> result = {};
  std::pair<double, double> temp_dot = {};
  buf[0] = '\0';
  while (counter < max)
    {
      skip_space_symbols (buf, ind, f);
      if (buf[ind] == '\0')
        {
          break;
        }
      if (sscanf(buf, "%lf", &temp) != 1)
        {
          printf("[ERROR] Invalid symbol in file, float number expected !!!\n");
	  return {};
        }
      if (counter % 2 == 1)
        {
          temp_dot.second = temp;
          result.push_back(temp_dot);
	}
      else
        {
          temp_dot.first = temp;
	}
      skip_number (buf, ind);
      counter++;
    }
  if (counter < max)
    {
      printf("[ERROR] Expected 4 numbers, %zu found!!!\n", counter);
      return {};
    }
  return result;
}

/*
int main()
{
  auto values = read("test.txt");
  if (!values)
    {
      printf("[FAIL] Input file is bad!!!\n");
      return 0;
    }
  std::vector vals = values.value();
  for (int i = 0; i < vals.size(); i++) {
    printf("%e %e \n", vals[i].first, vals[i].second);
  }
  return 0;
}
*/
