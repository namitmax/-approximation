#ifndef READ_FILE_H
#define READ_FILE_H

#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <vector>
#include <string>
#include <optional>

#define LEN 100

std::optional<std::vector<std::pair<double, double>>> read (const char *);

#endif /* READ_FILE_H */
