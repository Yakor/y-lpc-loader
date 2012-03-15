/**
 * @file   lpc3250_loader.c
 * @author  <yakor.spb@gmail.com>
 * @date   Thu Mar 15 19:27:35 2012
 *
 * @brief lpc3250 loader
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "read_config.h"

int main  (int argc, char *argv[], char *env[] )
{
  int i;
  config_t config;

  printf ("lpc3250_loader\n");

  read_config_yaml(".lpc3250.yaml",&config);

  printf("Port: %s\n",config.port);
  for(i=0;i<config.qty_exec;i++)
    {
      printf("i: %i\n",i);
      printf("PrimaryFileName: %s\n",config.executables[i].primary_filename);
      printf("IRAMaddress: %x\n",config.executables[i].iram_address);
      printf("SecondaryFileName: %s\n",config.executables[i].secondary_filename);
      printf("SDRAMaddress: %x\n",config.executables[i].sdram_address);
    }

  return 0;
}
