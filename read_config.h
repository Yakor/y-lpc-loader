#ifndef READ_CONFIG_H
#define READ_CONFIG_H

#define CONFIG_PRIMARY_FILENAME "PrimaryFileName"
#define CONFIG_IRAM_ADDRESS "IRAMaddress"
#define CONFIG_SECONDARY_FILENAME "SecondaryFileName"
#define CONFIG_SDRAM_ADDRESS "SDRAMaddress"
#define CONFIG_PORT "Port"
#define CONFIG_PRINT_ALL_CHAR "PrintAllChar"
#define CONFIG_EXECUTABLES "Exec"
#define DEFAULT_CONFIG_FILE_NAME "~/.lpc3250.yaml"
#define DEFAULT_IRAM_ADDRESS 0x0000
#define DEFAULT_SDRAM_ADDRESS 0x80000004
#define DEFAULT_PORT "/dev/ttyS0"

typedef struct
{
  char             *primary_filename;        /**< start address of first executable */
  int               iram_address;            /**< start address of first executable */
  char             *secondary_filename;      /**< second executable file name */
  int               sdram_address;           /**< start address of second executable */
} executables_t;

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
