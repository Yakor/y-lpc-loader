#ifndef READ_CONFIG_H
#define READ_CONFIG_H

#define CONFIG_PRIMARY_FILENAME "PrimaryFileName"
#define CONFIG_IRAM_ADDRESS "IRAMaddress"
#define CONFIG_SECONDARY_FILENAME "SecondaryFileName"
#define CONFIG_SDRAM_ADDRESS "SDRAMaddress"
#define CONFIG_PORT "Port"
#define CONFIG_EXECUTABLES "Exec"
#define CONFIG_FILE_NAME "~/.lpc3250.yaml"

typedef struct
{
  char              primary_filename[500];
  int               iram_address;
  char              secondary_filename[500];
  int               sdram_address;
} executables_t;

typedef struct
{
  char              port[150];
  int               qty_exec;
  executables_t    *executables;
} config_t;

int               read_config_yaml (char *file_name, config_t * config);
void              default_setting (executables_t * ex);

#endif /* READ_CONFIG_H */
