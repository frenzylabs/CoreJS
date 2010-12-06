#include "common.h"

using namespace v8;

#ifndef URL_H
#define URL_H

typedef struct URL
{
    char *protocol;
    char *host;
    int port;
    char *path;
} URL;

#endif

namespace URLLib
{
    void parse_url(char *url, URL *ret);
}