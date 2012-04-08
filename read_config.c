/**
 * @file   read_config.c
 * @author Sergey Yakovlev <yakor.spb@gmail.com>
 * @copyright (c) 2012, Sergey Yakovlev this code is released uder BSD license. Can be found in LICENSE.
 *
 * @brief  Read config
 *
 *
 */

#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "yaml_macro.h"
#include "read_config.h"

/**
 * read y-lpc-loader config YAML
 *
 * @param file_name config file name
 * @param config pointe to config_t struct
 *
 * @return 1 if succesfully otherwise 0
 */

int
read_config_yaml (char *file_name, config_t * config)
{
  int               i;
  unsigned long int tmp;

  config->executables = 0;
  config->qty_exec = 0;
  config->port = 0;
  config->prnt_all_char = 0;
  config->dont_exit = 0;

  MACRO_YAML_DOCUMENT (file_name,
                       {
                         MACRO_YAML_STRING (CONFIG_PORT, config->port,{});
                         MACRO_YAML_INT (CONFIG_PRINT_ALL_CHAR,
                                         tmp,
                                         {
                                           config->prnt_all_char=tmp;
                                         }
                                         );
                         MACRO_YAML_INT (CONFIG_DONT_EXIT,
                                         tmp,
                                         {
                                           config->dont_exit = tmp;
                                         }
                                         );
                         MACRO_YAML_SEQUENCE (CONFIG_EXECUTABLES, i,
                                              {
                                                config->executables = realloc (config->executables, sizeof (executables_t) * (i + 1));
                                                memset (&config->executables[i], 0, sizeof (executables_t));
                                                default_setting (&config->executables[i]);
                                              }
                                              , config->qty_exec,
                                              {
                                                MACRO_YAML_STRING (CONFIG_PRIMARY_FILENAME,
                                                                   config->executables[i].primary_filename,
                                                                   {}
                                                                   );
                                                MACRO_YAML_INT (CONFIG_IRAM_ADDRESS,
                                                                tmp,
                                                                {
                                                                  config->executables[i].iram_address=tmp;
                                                                }
                                                                );
                                                MACRO_YAML_STRING (CONFIG_SECONDARY_FILENAME,
                                                                   config->executables[i].secondary_filename,
                                                                   {}
                                                                   );
                                                MACRO_YAML_INT (CONFIG_SDRAM_ADDRESS,
                                                                tmp,
                                                                {
                                                                  config->executables[i].sdram_address=tmp;
                                                                }
                                                                );
                                              }
                                              );
                       }
                       );

  return 1;
}

/**
 * fills executables_t struct with default setting
 *
 * @param ex pointer to executables_t struct
 */

void
default_setting (executables_t * ex)
{
  ex->iram_address = DEFAULT_IRAM_ADDRESS;
  ex->sdram_address = DEFAULT_SDRAM_ADDRESS;
  ex->primary_filename = 0;
  ex->secondary_filename = 0;
}
