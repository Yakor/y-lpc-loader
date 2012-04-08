/**
 * @file   read_config.h
 * @author Sergey Yakovlev <yakor.spb@gmail.com>
 *
 * @brief  Defines and struct needed to configure y-lpc-loader
 *
 *
 */

#ifndef READ_CONFIG_H
#define READ_CONFIG_H

#define CONFIG_PRIMARY_FILENAME "PrimaryFileName"        /**< Name of option of first exec in config file */
#define CONFIG_IRAM_ADDRESS "IRAMaddress"                /**< Name of option of first exec load address in config file */
#define CONFIG_SECONDARY_FILENAME "SecondaryFileName"    /**< Name of option of second exec in config file */
#define CONFIG_SDRAM_ADDRESS "SDRAMaddress"              /**< Name of option of second exec load address in config file */
#define CONFIG_PORT "Port"                               /**< Name of option of port in config file */
#define CONFIG_PRINT_ALL_CHAR "PrintAllChar"             /**< Name of otion  that swith output type*/
#define CONFIG_EXECUTABLES "Exec"                        /**< Name of option that start sewuence of execs */
#define DEFAULT_CONFIG_FILE_NAME "~/.lpc3250.yaml"       /**< Default config name */
#define DEFAULT_IRAM_ADDRESS 0x0000                      /**< Default first exec load address */
#define DEFAULT_SDRAM_ADDRESS 0x80000004                 /**< Default secont exec load address */
#define DEFAULT_PORT "/dev/ttyS0"                        /**< Default port */

/// struct that includes execs options

typedef struct
{
  char             *primary_filename;        /**< start address of first executable */
  int               iram_address;            /**< start address of first executable */
  char             *secondary_filename;      /**< second executable file name */
  int               sdram_address;           /**< start address of second executable */
} executables_t;

/// struct that include all configurable options

typedef struct
{
  char             *port;                    /**< port */
  int               prnt_all_char;           /**< if 1 print all incoming char */
  int               qty_exec;                /**< qty of first execs */
  executables_t    *executables;             /**< pointer to array of executables */
} config_t;

int               read_config_yaml (char *file_name, config_t * config);
void              default_setting (executables_t * ex);

#endif /* READ_CONFIG_H */
