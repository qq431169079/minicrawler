#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "h/global.h"
#include "h/struct.h"
#include "h/proto.h"

struct surl url[100];

int debug=0;
int timeout=5;
int writehead=0;

int gettimeint()
{
	long long static starttime=0;
	struct timeval tim;
	
        gettimeofday(&tim, NULL);
        
        if(starttime==0) {starttime=tim.tv_sec*1000+tim.tv_usec/1000;return 0;}
        
        return (int)((tim.tv_sec*1000+tim.tv_usec/1000)-starttime);
}

/** primitivni parsovatko url
 */
void simpleparseurl(struct surl *u)
{
	u->port=80;
	u->path[0]='/';

	sscanf(u->rawurl, "http://%[^/]/%s", u->host, u->path+1);

/*	sscanf(u->rawurl, "http://%99[^:]:%99d/%99[^\n]", u->host, &(u->port), u->path);*/

	/*printf("host = \"%s\"\n", u->host);
	printf("port = \"%d\"\n", u->port);
	printf("path = \"%s\"\n", u->path);*/
}


/** nacte url z prikazove radky do struktur
 */
void initurls(int argc, char *argv[])
{
	int t;
	int i=0;
 
	for(t=1;t<argc;t++) {
		if(!strcmp(argv[t],"-d")) {debug=1;continue;}
		if(!strcmp(argv[t],"-h")) {writehead=1;continue;}
		if(!strncmp(argv[t],"-t",2)) {timeout=atoi(argv[t]+2);continue;}
		
		strcpy(url[i].rawurl,argv[t]);
		simpleparseurl(&url[i]);
		url[i].state=S_JUSTBORN;
		url[i].index=i;
		url[i].bufp=0;
		i++;
		}

	strcpy(url[i].rawurl,""); // ukoncovaci znacka
}

/** a jedeeeem...
 */
int main(int argc, char *argv[])
{
	if(argc<2) {printf("\nUsage:   minicrawler [-d] [-h] [-tSECONDS] url [url2] [url3] [...]\n\nWhere:   -d enables debug messages (to stderr)\n         -tSECONDS sets timeout (default is 5 seconds)\n\n");exit(-1);}

	gettimeint(); // nastavi se
	
	initurls(argc,argv);
	go();
 
	exit(0);
}

