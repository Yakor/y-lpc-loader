#ifndef READ_CONFIG_H
#define READ_CONFIG_H

#define CONFIG_PRIMARY_FILENAME "PrimaryFileName"
#define CONFIG_IRAM_ADDRESS "IRAMaddress"
#define CONFIG_SECONDARY_FILENAME "SecondaryFileName"
#define CONFIG_SDRAM_ADDRESS "SDRAMaddress"
#define CONFIG_PORT "Port"
#define CONFIG_EXECUTABLES "Exec"
#define DEFAULT_CONFIG_FILE_NAME "~/.lpc3250.yaml"
#define DEFAULT_IRAM_ADDRESS 0x0000
#define DEFAULT_SDRAM_ADDRESS 0x80000004
#define DEFAULT_PORT "/dev/ttyS0"

typedef struct
{
  char             *primary_filename;
  int               iram_address;
  char             *secondary_filename;
  int               sdram_address;
} executables_t;

typedef struct
{
  char             *port;
  int               qty_exec;
  executables_t    *executables;
} config_t;

int               read_config_yaml (char *file_name, config_t * config);
void              default_setting (executables_t * ex);

#endif /* READ_CONFIG_H */
