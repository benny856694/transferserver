/*
 * @file    fs.c
 * @brief
 * @version 0.1.0
 * @data    2019年7月23日
 * @author  杨果
 */
#include "fs.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int create_directory(const char* dir) {
  int i, len;

  len = strlen(dir);
  char dir_path[4096];
  memset(dir_path, 0, sizeof(dir_path));
  strncpy(dir_path, dir, len);

  for (i = 0; i < len; i++) {
    if (dir_path[i] == '/' && i > 0) {
      dir_path[i] = '\0';
      if (access(dir_path, F_OK) < 0) {
        if (mkdir(dir_path, 0755) < 0) {
          return 0;
        }
      }
      dir_path[i] = '/';
    }
  }

  if (i == len) {
    if (access(dir_path, F_OK) < 0)
      if (mkdir(dir_path, 0755) < 0)
        return 0;
  }

  return 1;
}
