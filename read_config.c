/**
 * @file   read_config.c
 * @author  <yakor.spb@gmail.com>
 *
 * @brief  Read config
 *
 * @todo  Сделать проверки на правильность конфигов чтоб никогда не падала
 */

#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "yaml_macro.h"
#include "read_config.h"

int
read_config_yaml (char *file_name, config_t * config)
{
  int             i;

  config->executables = 0;

  MACRO_YAML_DOCUMENT (file_name,
		       {
                         MACRO_YAML_STRING (CONFIG_PORT, config->port);
                         MACRO_YAML_SEQUENCE (CONFIG_EXECUTABLES, i,
                                              {
                                                config->executables = realloc (config->executables, sizeof (executables_t) * (i + 1));
                                                memset (&config->executables[i], 0, sizeof (executables_t));
                                                default_setting (&config->executables[i]);
                                              }
                                              , config->qty_exec,
                                              {
                                                MACRO_YAML_STRING (CONFIG_PRIMARY_FILENAME,
                                                                   config->executables[i].primary_filename);
                                                MACRO_YAML_INT (CONFIG_IRAM_ADDRESS,
                                                                config->executables[i].iram_address,
                                                                {
                                                                }
                                                                );
                                                MACRO_YAML_STRING (CONFIG_SECONDARY_FILENAME,
                                                                   config->executables[i].secondary_filename);
                                                MACRO_YAML_INT (CONFIG_SDRAM_ADDRESS,
                                                                config->executables[i].sdram_address,
                                                                {
                                                                }
                                                                );
                                              }
                                              );
                       }
                       );

  return 1;
}

void
default_setting (executables_t * ex)
{
  ex->iram_address = 0x0000;
  ex->sdram_address = 0x80000004;
}
