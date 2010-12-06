#include "url.h"

namespace URLLib
{

    void parse_url(char *url, URL *ret) {
        char *otherUrl = (char *)(malloc (strlen(url) + 1)); //new char(sizeof(url));
        strcpy(otherUrl, url);
        strcat(otherUrl, "/");

        char *tmp = (otherUrl == url)? (char *) strdup(url) : (char *) strdup(otherUrl);
        int len = 0;

        
        ret->protocol = (char *) strtok(tmp, "/");
        len = strlen(ret->protocol) + 2;
        ret->host = (char *) strtok(NULL, "/");
        len += strlen(ret->host);
        ret->path = (char *) strdup(&url[len]);
        ret->path = (char *) strtok(ret->path, "#");
        if(ret->path == NULL)
            ret->path = (char *)"/";
        
        ret->protocol = (char *) strtok(ret->protocol, ":");
        ret->host = (char *) strtok(ret->host, ":");
        tmp = (char *) strtok(NULL, ":");

        if (tmp == NULL) {
            if (strcmp(ret->protocol, "http") == 0) {
                ret->port = 80;
            } else if (strcmp(ret->protocol, "https") == 0) {
                ret->port = 443;
            }
        } else {
            ret->port = atoi(tmp);
        }
    }

    void free_url(URL *url) {
        free(url->path);
        free(url->protocol);
    }   
}