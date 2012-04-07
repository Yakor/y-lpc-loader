/**
 * @file   yaml_macro.h
 * @author Sergey Yakovlev <yakor.spb@gmail.com>
 *
 * @brief  macros for read yaml config
 *
 *
 */

#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#define MACRO_YAML_STRING(config_name,program_name,func)                \
  if(!strcmp((char *)event.data.scalar.value,(config_name)))            \
    {                                                                   \
      yaml_event_delete(&event);                                        \
      MACRO_YAML_NEXT;                                                  \
      if(event.type!=YAML_SCALAR_EVENT) return 0;                       \
      (program_name)=malloc(strlen((char*)event.data.scalar.value)+1);  \
      strcpy((program_name),(char *)event.data.scalar.value);           \
      (func);                                                           \
      break;                                                            \
    }

#define MACRO_YAML_NEXT                         \
  if (!yaml_parser_parse(&parser, &event))      \
    {                                           \
      yaml_parser_delete(&parser);              \
      return 0;                                 \
    }                                           \

#define MACRO_YAML_INT(config_name,program_name,func)                   \
  if(!strcmp((char *)event.data.scalar.value,(config_name)))            \
    {                                                                   \
      yaml_event_delete(&event);                                        \
      MACRO_YAML_NEXT;                                                  \
      if(event.type!=YAML_SCALAR_EVENT) return 0;                       \
      (program_name)=strtol((char *)event.data.scalar.value,NULL,0);    \
      (func);                                                           \
      break;                                                            \
    }

#define MACRO_YAML_SEQUENCE(config_name,index,mapping_start_function,amount,func) \
  if(!strcmp((char *)event.data.scalar.value,(config_name)))            \
    {                                                                   \
      yaml_event_delete(&event);                                        \
      MACRO_YAML_NEXT;                                                  \
      yaml_event_delete(&event);                                        \
      done=0;                                                           \
      (index)=0;                                                        \
      while(!done)                                                      \
        {                                                               \
          MACRO_YAML_NEXT;                                              \
          switch (event.type)                                           \
            {                                                           \
            case YAML_SCALAR_EVENT:                                     \
              {                                                         \
                (func);                                                 \
                break;                                                  \
              }                                                         \
            case YAML_SEQUENCE_END_EVENT:                               \
              {                                                         \
                done=1;                                                 \
                break;                                                  \
              }                                                         \
            case YAML_SEQUENCE_START_EVENT:                             \
              {                                                         \
                while(event.type!=YAML_SEQUENCE_END_EVENT)              \
                  {                                                     \
                    MACRO_YAML_NEXT;                                    \
                  }                                                     \
                break;                                                  \
              }                                                         \
            case YAML_MAPPING_START_EVENT:                              \
              {                                                         \
                (mapping_start_function);                               \
                break;                                                  \
              }                                                         \
            case YAML_MAPPING_END_EVENT:                                \
              {                                                         \
                (index)++;                                              \
                break;                                                  \
              }                                                         \
            default:                                                    \
              {                                                         \
                break;                                                  \
              }                                                         \
            }                                                           \
          yaml_event_delete(&event);                                    \
        }                                                               \
      (amount)=(index);                                                 \
      done=0;                                                           \
      break;                                                            \
    }

#define MACRO_YAML_MAPPING(config_name,func)                    \
  if(!strcmp((char *)event.data.scalar.value,(config_name)))    \
    {                                                           \
      yaml_event_delete(&event);                                \
      MACRO_YAML_NEXT;                                          \
      yaml_event_delete(&event);                                \
      done=0;                                                   \
      while(!done)                                              \
        {                                                       \
          MACRO_YAML_NEXT;                                      \
          switch (event.type)                                   \
            {                                                   \
            case YAML_SCALAR_EVENT:                             \
              {                                                 \
                (func);                                         \
                break;                                          \
              }                                                 \
            case YAML_MAPPING_END_EVENT:                        \
              {                                                 \
                done=1;                                         \
                break;                                          \
              }                                                 \
            case YAML_SEQUENCE_START_EVENT:                     \
              {                                                 \
                while(event.type!=YAML_SEQUENCE_END_EVENT)      \
                  {                                             \
                    MACRO_YAML_NEXT;                            \
                  }                                             \
                break;                                          \
              }                                                 \
            default:                                            \
              {                                                 \
                return 0;                                       \
                break;                                          \
              }                                                 \
            }                                                   \
          yaml_event_delete(&event);                            \
        }                                                       \
      done=0;                                                   \
      break;                                                    \
    }

#define MACRO_YAML_DOCUMENT(yaml_file,func)     \
  yaml_parser_t parser;                         \
  yaml_event_t event;                           \
  int done = 0;                                 \
  FILE *input;                                  \
  yaml_parser_initialize(&parser);              \
  input = fopen((yaml_file), "rb");             \
  if(!input) return 0;                          \
  yaml_parser_set_input_file(&parser, input);   \
  done=0;                                       \
  while (!done)                                 \
    {                                           \
      MACRO_YAML_NEXT;                          \
      switch (event.type)                       \
        {                                       \
        case YAML_SCALAR_EVENT:                 \
          {                                     \
            (func);                             \
            break;                              \
          }                                     \
        case YAML_DOCUMENT_END_EVENT:           \
          {                                     \
            done=1;                             \
            break;                              \
          }                                     \
        case YAML_STREAM_END_EVENT:             \
          {                                     \
            done=1;                             \
            break;                              \
          }                                     \
        default:                                \
          {                                     \
            break;                              \
          }                                     \
        }                                       \
      yaml_event_delete(&event);                \
    }                                           \
  yaml_parser_delete(&parser);

#define MACRO_YAML_DOCUMENT_FROM_STRING(yaml_string,func)               \
  yaml_parser_initialize(&parser);                                      \
  if(!strlen(yaml_string)) return 0;                                    \
  yaml_parser_set_input_string(&parser, yaml_string, strlen(yaml_string)); \
  done=0;                                                               \
  while (!done)                                                         \
    {                                                                   \
      MACRO_YAML_NEXT;                                                  \
      switch (event.type)                                               \
        {                                                               \
        case YAML_SCALAR_EVENT:                                         \
          {                                                             \
            (func);                                                     \
            break;                                                      \
          }                                                             \
        case YAML_DOCUMENT_END_EVENT:                                   \
          {                                                             \
            done=1;                                                     \
            break;                                                      \
          }                                                             \
        case YAML_STREAM_END_EVENT:                                     \
          {                                                             \
            done=1;                                                     \
            break;                                                      \
          }                                                             \
        default:                                                        \
          {                                                             \
            break;                                                      \
          }                                                             \
        }                                                               \
      yaml_event_delete(&event);                                        \
    }                                                                   \
  yaml_parser_delete(&parser);
