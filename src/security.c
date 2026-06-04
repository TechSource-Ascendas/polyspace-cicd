/**
 *       Copyright 2015-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * This example shows defects caused by security errors.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */

#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include <arpa/inet.h>
#include <crypt.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <grp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <pwd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <unistd.h>

#include "globaldefs.h"
#include "lib_crypto_checkers.h"


/*============================================================================
 *  BAD NETWORK CONNECT ORDER
 *==========================================================================*/
void bug_badnetworkconnectorder_server(void) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[SIZE1025];
    time_t ticks;
    struct tm* ptm;
    /* Create socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        fatal_error();
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
    (void)bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    (void)listen(listenfd, 10);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        if (connfd == -1) {
            fatal_error();
        }
        ticks = time(NULL);
        ptm = gmtime(&ticks);
        if (ptm != NULL) {
            int n = snprintf(sendBuff, sizeof(sendBuff) - 1, "%2d:%2d:%2d\r\n", ptm->tm_hour,
                             ptm->tm_min, ptm->tm_sec);
            if (n > 0 && n < sizeof(sendBuff)) {
                sendBuff[n] = '\0';
                (void)write(listenfd, sendBuff,
                            sizeof(sendBuff)); /* Defect: Writing to wrong socket id */
            }
        }
        (void)close(connfd);
        sleep(1);
    }
}

void corrected_badnetworkconnectorder_server(void) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[SIZE1025];
    time_t ticks;
    struct tm* ptm;
    /* Create socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        fatal_error();
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(sendBuff, 0, sizeof(sendBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
    (void)bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    (void)listen(listenfd, 10);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        if (connfd == -1) {
            fatal_error();
        }
        ticks = time(NULL);
        ptm = gmtime(&ticks);
        if (ptm != NULL) {
            int n = snprintf(sendBuff, sizeof(sendBuff) - 1, "%2d:%2d:%2d\r\n", ptm->tm_hour,
                             ptm->tm_min, ptm->tm_sec);
            if (n > 0 && n < sizeof(sendBuff)) {
                sendBuff[n] = '\0';
                (void)write(connfd, sendBuff,
                            sizeof(sendBuff)); /* Fix: Writing to correct socket id */
            }
        }
        (void)close(connfd);
        sleep(1);
    }
}

const char serv_addr_str[] = "0:0:0:0:0:0:0:0";

int bug_badnetworkconnectorder_client(void) {
    volatile int rd;
    int sockfd = 0, n = 0;
    char recvBuff[SIZE1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, 0, sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        (void)printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, serv_addr_str, &serv_addr.sin_addr) <= 0) {
        (void)printf("\n inet_pton error occurred\n");
        return 1;
    }

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
    	/* Defect: Missing connection of socket */
        recvBuff[n] = 0;
        (void)printf("Reception: %s\n", recvBuff);
    }

    if (n < 0) {
        (void)printf("\n Read error \n");
    }

    return 0;
}

int corrected_badnetworkconnectorder_client(void) {
    int sockfd = 0, n = 0;
    char recvBuff[SIZE1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, 0, sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        (void)printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, serv_addr_str, &serv_addr.sin_addr) <= 0) {
        (void)printf("\n inet_pton error occurred\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <
        0) { /* Fix: Connecting the socket */
        (void)printf("\n Error : Connect Failed \n");
        return 1;
    }

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
        recvBuff[n] = 0;
        (void)printf("Reception: %s\n", recvBuff);
    }

    if (n < 0) {
        (void)printf("\n Read error \n");
    }

    return 0;
}


/*============================================================================
 *  UMASK USING WRONG MASK ARGUMENT
 *==========================================================================*/
const mode_t my_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; /* 0644 */
void bug_badumask(void) {
    umask(my_mode); /* Defect: Argument contains "or" combination of permission modes */
}
void corrected_badumask(void) {
    umask(~(my_mode)); /* Fix: Argument contains "and" combination of reversed permission modes */
}


/*============================================================================
 *  CHROOT MISUSE
 *==========================================================================*/
const char root_path[] = "/var/ftproot";
const char log_path[] = "file.log";
FILE* bug_chrootmisuse(void) {
    FILE* res;
    int r = chroot(root_path);
    if (r == -1) {
        fatal_error();
    }
    (void)chdir("base");        /* Defect: chdir should be used only with "/" */
    res = fopen(log_path, "r"); /* Defect: Trying to open a file without previous chdir("/") */
    if (res == NULL) {
        fatal_error();
    }
    return res;
}

FILE* corrected_chrootmisuse(void) {
    FILE* res;
    int r = chroot(root_path);
    if (r == -1) {
        fatal_error();
    }
    r = chdir("/"); /* Fix: chdir used with "/" */
    if (r == -1) {
        fatal_error();
    }
    res = fopen(log_path, "r");
    if (res == NULL) {
        fatal_error();
    }
    return res;
}


/*============================================================================
 *  USING DANGEROUS PERMISSIONS
 *==========================================================================*/
void bug_dangerouspermissions(void) {
    mode_t mode = S_IROTH | S_IXOTH | S_IWOTH;
    int fd = creat(log_path, mode); /* Defect: Creating file with more rights for other than for
                                       user (all rights for other only --------rwx) */
    if (fd >= 0) {
        (void)write(fd, "Hello\n", 6);
        (void)close(fd);
        (void)unlink(log_path);
    }
}

void corrected_dangerouspermissions(void) {
    mode_t mode = S_IRUSR | S_IXUSR | S_IWUSR;
    int fd = creat(log_path, mode); /* Fix: Creating file with all rights for user (--rwx------) */
    if (fd >= 0) {
        (void)write(fd, "Hello\n", 6);
        (void)close(fd);
        (void)unlink(log_path);
    }
}


/*============================================================================
 *  USING DANGEROUS STANDARD FUNCTIONS
 *==========================================================================*/
void bug_dangerousstdfunc(void) {
    char dst[SIZE1024];
    char src[SIZE1024];
    read_str(src);
    (void)sprintf(dst, "Writing to %s:%s", log_path,
                  src); /* Defect: src string has unknown length so writing to dst may overflow */
    print_str(dst);
}

void corrected_dangerousstdfunc(void) {
    char dst[SIZE1024];
    char src[SIZE1024];
    read_str(src);
    (void)snprintf(dst, sizeof(dst), "Writing to %s:%s", log_path,
                   src); /* Fix: Limit writing to dst to its length */
    print_str(dst);
}


/*============================================================================
 *  DATA LENGTH MISMATCH
 *==========================================================================*/
extern BUF_MEM b;

int bug_datalengthmismatch(BUF_MEM* a) {
    BUF_MEM* os = a;
    int num, length;

    if (a == 0x0) {
        return 0;
    }
    num = 0;

    length = *(unsigned short*)os->data;
    memcpy(&(b.data[num]), os->data + 2,
           length); /* Defect: length has no relation with copied memory */

    return (1);
}

int corrected_datalengthmismatch(BUF_MEM* a) {
    BUF_MEM* os = a;
    int num, length;

    if (a == 0x0) {
        return 0;
    }
    num = 0;

    length = *(unsigned short*)os->data;
    if (length < (os->max - 2)) {
        memcpy(&(b.data[num]), os->data + 2,
               length); /* Fix: length has been checked against copied buffer size */
    }

    return (1);
}

void call_datalengthmismatch(void) {
    BUF_MEM a;
    a.data = (char*)malloc(12 * sizeof(char));
    if (a.data) {
        strcpy(a.data, "Let us try!");
        a.max = 12;
        bug_datalengthmismatch(&a);
        corrected_datalengthmismatch(&a);
        free(a.data);
    }
}


/*============================================================================
 *  FUNCTION POINTER ASSIGNED WITH ABSOLUTE ADDRESS
 *==========================================================================*/
extern int func0(void);
typedef int (*FuncPtr)(void);
FuncPtr bug_funcptrabsoluteaddr(void) {
    return (FuncPtr)0x08040000; /* Defect: Using absolute address for a function pointer */
}

FuncPtr corrected_funcptrabsoluteaddr(void) {
    return &func0; /* Fix: Using address of an existing function */
}


/*============================================================================
 *  MISSING SWITCH CASE
 *==========================================================================*/
char my_user[] = "John";
typedef enum E { USER = 1, ADMIN, UNKNOWN = 0 } LoginType;

LoginType demo_getcategory(const char* user) {
    LoginType cat = UNKNOWN;
    if (strncmp(user, "toto", 4) == 0) {
        cat = USER;
    } else if (strncmp(user, "root", 4) == 0) {
        cat = ADMIN;
    }
    return cat;
}

void bug_missingswitchcase(void) {
    char c = ' ';
    switch (demo_getcategory(my_user)) {
    case USER:
        c = 'u';
        break;
    case ADMIN:
        c = 'a';
        break;
    } /* Defect: Missing UNKNOWN case */
    (void)printf("Welcome %s (%c)\n", my_user, c);
}

void corrected_missingswitchcase(void) {
    char c = ' ';
    switch (demo_getcategory(my_user)) {
    case USER:
        c = 'u';
        break;
    case ADMIN:
        c = 'a';
        break;
    case UNKNOWN: /* Fix:  Adding UNKNOWN case */
        c = '0';
        break;
    }
    (void)printf("Welcome %s (%c)\n", my_user, c);
}


/*============================================================================
 *  NON SECURE TEMPORARY FILE
 *==========================================================================*/
FILE* bug_nonsecuretempfile(void) {
    FILE* f = tmpfile(); /* Defect: tmpfile is not a secure function */
    if (f == NULL) {
        fatal_error();
    }
    return f;
}

int corrected_nonsecuretempfile(void) {
    char template_tmp[] = "abcXXXXXX";
    int fd = mkstemp(template_tmp); /* Fix: mkstemp is safer */
    if (fd == -1) {
        fatal_error();
    }
    return fd;
}


/*============================================================================
 *  USE OF OBSOLETE STANDARD FUNCTION
 *==========================================================================*/
unsigned short bug_obsoletestdfunc(void) {
    /* Returns the  number of seconds since the Epoch */
    struct timeb t;
    ftime(&t); /* Defect: ftime is obsolete */
    return t.time;
}

double corrected_obsoletestdfunc(void) {
    /* Returns the  number of seconds since the Epoch */
    time_t t;
    (void)time(&t); /* Fix: time replaces ftime */
    double s = difftime(t, 0);
    return s;
}


/*============================================================================
 *  PATH TRAVERSAL
 *==========================================================================*/
FILE* bug_pathtraversal(void) {
    char path[SIZE1024] = "";
    if (fgets(path, SIZE1024, stdin) == NULL) {
        return NULL;
    }
    FILE* f = fopen(path, "w"); /* Defect: Path is tainted and may contain absolute path or '..' */
    if (f == NULL) {
        fatal_error();
    }
    return f;
}

FILE* corrected_pathtraversal(void) {
    char path[SIZE1024] = "./tmp/file.log";
    FILE* f = fopen(path, "w"); /* Fix: Path is known */
    if (f == NULL) {
        fatal_error();
    }
    return f;
}


/*============================================================================
 *  RAND SEED CONSTANT
 *==========================================================================*/
extern int g(int i);
void bug_randseedconstant(void) {
    const int init = 123456;
    srand(init); /* Defect: Random initialization seed is constant, so may be predictable */
}

void corrected_randseedconstant(void) {
    time_t t;
    (void)time(&t);
    srand(g(t)); /* Fix: Random initialization seed is no more constant */
}


/*============================================================================
 *  RAND SEED PREDICTABLE
 *==========================================================================*/
void bug_randseedpredictable(void) {
    srand((unsigned int)getuid()); /* Defect: Random initialization is based on user id, so may be
                                      predictable */
}

void corrected_randseedpredictable(void) {
    time_t t;
    (void)time(&t);
    srand(g(getuid())); /* Fix: Random initialization is no more predictable */
}


/*============================================================================
 *  RELATIVE PATH COMMAND
 *==========================================================================*/
void bug_relativepathcmd(void) {
    char cmd[] = "ls";
    execl(cmd, cmd, NULL); /* Defect: Command has relative path */
}

void corrected_relativepathcmd(void) {
    char cmd[] = "/usr/bin/ls";
    execl(cmd, cmd, NULL); /* Fix: Command has absolute path */
}


/*============================================================================
 *  RELATIVE PATH LIBRARY
 *==========================================================================*/
void bug_relativepathlib(void) {
    char libpath[] = "my_lib.so";
    dlopen(libpath, RTLD_LAZY); /* Defect: Library has relative path */
}

void corrected_relativepathlib(void) {
    char libpath[] = "/usr/lib/my_lib.so";
    dlopen(libpath, RTLD_LAZY); /* Fix: Library has absolute path */
}


/*============================================================================
 *  PRINTING OF SENSITIVE DATA
 *==========================================================================*/
extern void verify_null(const char* buf);
void bug_sensitivedataprint(void) {
    struct passwd *result, pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    char buf[SIZE1024] = "";
    if (bufsize < 0) {
        fatal_error();
    }
    int r = getpwnam_r(my_user, &pwd, buf, bufsize, &result);
    if (r == -1) {
        fatal_error();
    }
    (void)puts("Name\n");
    (void)puts(pwd.pw_name);
    (void)puts("PassWord\n");
    (void)puts(pwd.pw_passwd); /* Defect: Password is printed */
    memset(buf, 0, sizeof(buf));
    verify_null(buf);
}

void corrected_sensitivedataprint(void) {
    struct passwd *result, pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    char buf[SIZE1024] = "";
    if (bufsize < 0) {
        fatal_error();
    }
    int r = getpwnam_r(my_user, &pwd, buf, bufsize, &result);
    if (r == -1) {
        fatal_error();
    }
    (void)puts("Name\n");
    (void)puts(pwd.pw_name);
    (void)puts("PassWord\n");
    (void)puts("XXXXXXXX\n"); /* Fix: Password is obfuscated */
    memset(buf, 0, sizeof(buf));
    verify_null(buf);
}


/*============================================================================
 *  SENSITIVE DATA IN HEAP NOT CLEARED
 *==========================================================================*/
void bug_sensitiveheapnotcleared(void) {
    struct passwd *result, pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize < 0) {
        fatal_error();
    }
    char* buf = (char*)malloc(bufsize);
    if (buf == NULL) {
        return;
    }
    int r = getpwnam_r(my_user, &pwd, buf, bufsize, &result);
    if (r == -1) {
        fatal_error();
    }
    free(buf); /* Defect: buf has not been cleared */
}

void corrected_sensitiveheapnotcleared(void) {
    struct passwd *result, pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize < 0) {
        fatal_error();
    }
    char* buf = (char*)malloc(bufsize);
    if (buf) {
        int r = getpwnam_r(my_user, &pwd, buf, bufsize, &result);
        if (r == -1) {
            fatal_error();
        }
        memset(buf, 0, (size_t)bufsize);
        verify_null(buf);
        free(buf); /* Fix: buf has been cleared */
    }
}


/*============================================================================
 *  SENSITIVE DATA IN STACK NOT CLEARED
 *==========================================================================*/
void bug_sensitivestacknotcleared(void) {
    struct passwd *result, pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    char buf[SIZE1024] = "";
    int r;
    if (bufsize < 0) {
        fatal_error();
    }
    r = getpwnam_r(my_user, &pwd, buf, bufsize, &result);
    if (r == -1) {
        fatal_error();
    }
} /* Defect: buf has not been cleared */

void corrected_sensitivestacknotcleared(void) {
    struct passwd *result, pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    char buf[SIZE1024] = "";
    int r;
    if (bufsize < 0) {
        fatal_error();
    }
    r = getpwnam_r(my_user, &pwd, buf, bufsize, &result);
    if (r == -1) {
        fatal_error();
    }
    memset(buf, 0, sizeof(buf)); /* Fix: buf is cleared */
    verify_null(buf);
}


/*============================================================================
 *  TIME OF CHECK / TIME OF USE (TOCTOU)
 *==========================================================================*/
extern void print_tofile(FILE* f);
void bug_toctou(void) {
    if (access(log_path, W_OK) == 0) {
        FILE* f = fopen(log_path, "w"); /* Defect: File may have changed since check */
        if (f) {
            print_tofile(f);
            (void)fclose(f);
        }
    }
}

void corrected_toctou(void) {
    int fd = open(log_path, O_WRONLY); /* Fix: Using open/fdopen prevents TOCTOU problem */
    if (fd != -1) {
        FILE* f = fdopen(fd, "w");
        if (f) {
            print_tofile(f);
            (void)fclose(f);
        }
    }
}


/*============================================================================
 *  UNSAFE STANDARD ENCRYPTION FUNCTION
 *==========================================================================*/
extern const char* salt;
extern const char* pwd;
char* bug_unsafestdcrypt(void) {
    char* result = NULL;
    result = crypt(pwd, salt); /* Defect: Use of crypt function that uses risky algorithm */
    if (result == NULL) {
        fatal_error();
    }
    return result;
}

char* corrected_unsafestdcrypt(void) {
    char* result = NULL;
    struct crypt_data data;
    data.initialized = 0;
    result = crypt_r(pwd, salt, &data); /* Fix: Use of crypt_r function */
    if (result == NULL) {
        fatal_error();
    }
    return result;
}


/*============================================================================
 *  UNSAFE STANDARD FUNCTION
 *==========================================================================*/
char* bug_unsafestdfunc(void) {
    char* loc_login_name = getlogin(); /* Defect: Function getlogin is non-reentrant */
    if (loc_login_name == NULL) {
        fatal_error();
    }
    return loc_login_name;
}

char login_name[SIZE128];

char* corrected_unsafestdfunc(void) {
    int r = getlogin_r(login_name, sizeof(login_name)); /* Fix: Function getlogin_r is reentrant */
    return r == -1 ? NULL : login_name;
}


/*============================================================================
 *  USING VULNERABLE PSEUDO-RANDOM-NUMBER-GENERATOR
 *==========================================================================*/
int bug_vulnerableprng(void) {
    return rand(); /* Defect: Function rand uses vulnerable algorithm */
}

unsigned char key_prng[SIZE16];

unsigned char* corrected_vulnerableprng(void) {
    RAND_bytes(key_prng, SIZE16); /* Fix: Use OpenSSL secure random function */
    return key_prng;
}


/*============================================================================
 *  BAD PRIVILEGE DROP ORDER
 *==========================================================================*/
static void sanitize_privilege_drop_check(uid_t olduid, gid_t oldgid) {
    if (seteuid(olduid) != -1) {
        /* Privileges can be restored, handle error */
        fatal_error();
    }
    if (setegid(oldgid) != -1) {
        /* Privileges can be restored, handle error */
        fatal_error();
    }
}

void bug_badprivilegedroporder(void) {
    uid_t newuid = getuid(), olduid = geteuid();
    gid_t newgid = getgid(), oldgid = getegid();

    if (setuid(newuid) == -1) {
        /* handle error condition */
        fatal_error();
    }
    if (setgid(newgid) == -1) {
        /* Defect: dropping group privileges using setgid()
         * needs elevated user privileges that have been dropped
         */
        /* handle error condition */
        fatal_error();
    }
    if (olduid == 0) {
        /* drop ancillary groups IDs only possible for root */
        if (setgroups(1, &newgid) == -1) {
            /* Defect: dropping ancillary groups privileges needs
             * elevated primary group privileges that have been dropped
             */
            /* handle error condition */
            fatal_error();
        }
    }

    /* It is still possible to regain group privileges due to incorrect relinquishment order */

    /*
     * Additional cautions are necessary to ensure that it's impossible to regain previous
     * privileges
     */
    sanitize_privilege_drop_check(olduid, oldgid);
}

void corrected_badprivilegedroporder(void) {
    uid_t newuid = getuid(), olduid = geteuid();
    gid_t newgid = getgid(), oldgid = getegid();

    if (olduid == 0) {
        /* drop ancillary groups IDs only possible for root */
        if (setgroups(1, &newgid) == -1) {
            /* handle error condition */
            fatal_error();
        }
    }
    if (setgid(getgid()) == -1) {
        /* handle error condition */
        fatal_error();
    }
    if (setuid(getuid()) == -1) {
        /* handle error condition */
        fatal_error();
    }

    /* Not possible to regain group privileges
     *
     * Additional cautions to ensure that it's impossible to regain previous privileges
     */
    sanitize_privilege_drop_check(olduid, oldgid);
}


/*============================================================================
 *  MISSING PRIVILEGE DROP CHECK
 *==========================================================================*/
extern int need_more_privileges;
void bug_missingprivilegedropcheck(void) {
    /* Code intended to run with elevated privileges */

    /* Temporarily drop elevated privileges */
    if (seteuid(getuid()) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* Code intended to run with lower privileges */
    if (need_more_privileges) {
        /* Restore elevated privileges */
        if (seteuid(0) != 0) {
            /* Handle error */
            fatal_error();
        }
        /* Code intended to run with elevated privileges */
    }
    /* ... */
    /* Permanently drop elevated privileges */
    if (setuid(getuid()) != 0) {
        /* Handle error */
        fatal_error();
    }

    /*
     * Code intended to run with lower privileges,
     * but if privilege relinquishment failed,
     * attacker can regain elevated privileges!
     */
} /* Defect: dropping privileges not checked before exit from function body */

void corrected_missingprivilegedropcheck(void) {
    /* Store the privileged ID for later verification */
    uid_t privid = geteuid();
    /* Code intended to run with elevated privileges   */

    /* Temporarily drop elevated privileges */
    if (seteuid(getuid()) != 0) {
        /* Handle error */
        fatal_error();
    }
    /* Code intended to run with lower privileges  */
    if (need_more_privileges) {
        /* Restore elevated Privileges */
        if (seteuid(privid) != 0) {
            /* Handle error */
            fatal_error();
        }
        /* Code intended to run with elevated privileges   */
    }
    /* ... */
    /* Restore privileges if needed */
    if (geteuid() != privid) {
        if (seteuid(privid) != 0) {
            /* Handle error */
            fatal_error();
        }
    }
    /* Permanently drop privileges */
    if (setuid(getuid()) != 0) {
        /* Handle error */
        fatal_error();
    }
    if (setuid(0) != -1) {
        /* Privileges can be restored, handle error */
        fatal_error();
    }

    /*
     * Code intended to run with lower privileges;
     * attacker cannot regain elevated privileges
     */
}


/*============================================================================
 *  RETURN NOT CHECKED
 *==========================================================================*/
extern void* start_routine(void*);

void bug_returnnotchecked(void) {
    pthread_t thread_id;
    pthread_attr_t attr;
    void* res;

    pthread_attr_init(&attr); /* Defect: sensitive function return ignored */
    (void)pthread_create      /* Defect: pthread_create is a critical sensitive function,
                               * its return value must be checked. For critical sensitive
                               * functions, the return cannot be ignored by cast to void.
                               */
        (&thread_id, &attr, &start_routine, NULL);
    pthread_join(thread_id, &res); /* Defect: return of current call is ignored (particularly
                                    * this function uses the output of previous function).
                                    */
}

void corrected_returnnotchecked(void) {
    pthread_t thread_id;
    pthread_attr_t attr;
    void* res;
    int result;

    (void)pthread_attr_init(&attr); /* Fix: explicit cast to void of a non-critical function return.
                                     * Ideally, the return value should be checked.
                                     */
    result = pthread_create(&thread_id, &attr, &start_routine, NULL);
    if (result != 0) { /* Fix: check return of a critical function as soon as possible */
        /* Handle error */
        fatal_error();
    }

    result = pthread_join(thread_id, &res);
    if (result != 0) { /* Fix: check return of a critical function as soon as possible */
        /* Handle error */
        fatal_error();
    }
}


/*============================================================================
 *  ERRNO NOT CHECKED
 *==========================================================================*/
#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 29 // setkey() and encrypt() deprecated in glibc 2.29
void bug_errnonotchecked(char key[SIZE64], char txt[SIZE64]) {
    setkey(key);     /* Defect: missing check of errno after call. Test errno
                      * is the only way to verify the call to POSIX 'setkey'
                      * performing encryption key setting is successful.
                      */
    encrypt(txt, 0); /* Defect: missing check of errno after call. Test errno
                      * is the only way to verify the call to POSIX 'encrypt'
                      * performing encryption is successful.
                      */
}

int corrected_errnonotchecked(char key[SIZE64], char txt[SIZE64]) {
    errno = 0;
    setkey(key);
    if (errno == 0) { /* Fix: check errno for zero after call */
        encrypt(txt, 0);
        if (errno == 0) { /* Fix: check errno for zero after call */
            return 0;
        }
    }
    return -1;
}
#endif // glibc < 2.29

/*============================================================================
 *  READLINK MISUSE
 *==========================================================================*/
extern void display_path(const char*);

void bug_readlinkmisuse(void) {
    char buf[SIZE1024];
    ssize_t len = readlink("/usr/bin/perl", buf,
                           sizeof(buf)); /* Defect: readlink does not null-terminate the
                                          * read buffer, but returns a length that programmer
                                          * could use as index to explicitly null-terminate the
                                          * buffer. Unfortunately, using this value as index is
                                          * problematic due to the fact that it can be -1 (when an
                                          * error occurs) or sizeof(buf).
                                          */
    if (len > 0) {           /* avoided off-by-one buffer-underflow when readlink returns -1 */
        buf[len - 1] = '\0'; /* avoided off-by-one buffer-overflow when len = sizeof(buf) */
    }                        /* but buf is non-null-terminated when len = 0 */
    display_path(buf);
}

void corrected_readlinkmisuse(void) {
    char buf[SIZE1024];
    ssize_t len =
        readlink("/usr/bin/perl", buf, sizeof(buf) - 1); /* Fix: reading in only sizeof(buf)-1
                                                          * characters and properly null-terminate
                                                          * the buffer with length check to prevent
                                                          * off-by-one buffer overflow or underflow.
                                                          */
    if (len != -1) {
        buf[len] = '\0';
        display_path(buf);
    } else {
        /* Handle error */
        fatal_error();
    }
}


/*============================================================================
 * FILE EXPOSURE TO CHILD
 *==========================================================================*/
const char* test_file = "/home/user/test.txt";

void bug_fileexposuretochild(void) {
    char c;
    pid_t pid;
    int fd = open(test_file, O_RDWR); /* create file descriptor in read and write mode */
    if (fd == -1) {
        /* Handle error */
        fatal_error();
    }
    pid = fork();
    if (pid == -1) {
        /* Handle error */
        fatal_error();
    } else if (pid == 0) {
        /* Child process inherits all opened file descriptors from the parent */
        (void)read(fd, &c, 1); /* Defect: race condition with parent process.
                                * Also inherited file descriptor expose the file
                                * content to the child process, vulnerable to attacks
                                * targeting the child process if the permission of
                                * the created file descriptor and the process privileges
                                * are not correctly managed. Inheritance of file
                                * descriptors reveals insufficient segregation between
                                * processes which is avoidable with a better design.
                                */
        (void)close(fd);
    } else { /* Parent */
        (void)read(fd, &c, 1);
        (void)close(fd);
    }
}

void corrected_fileexposuretochild(void) {
    char c;
    pid_t pid;

    /* Get the state of file for further file tampering checking */

    int fd = open(test_file, O_RDWR); /* create file descriptor in read and write mode */
    if (fd == -1) {
        /* Handle error */
        fatal_error();
    }

    /* Be sure the file was not tampered with while opening */

    pid = fork();
    if (pid == -1) {
        /* Handle error */
        (void)close(fd);
        fatal_error();
    } else if (pid == 0) { /* In the child process */
        (void)close(fd);
        fd = open(test_file, O_RDONLY); /* Fix: close the parent file descriptor
                                         * Reopen file with more restrict mode (read only).
                                         */
        if (fd == -1) {
            /* Handle error */
            fatal_error();
        }


        (void)read(fd, &c, 1);
        (void)close(fd);
    } else { /* Parent */
        (void)read(fd, &c, 1);
        (void)close(fd);
    }
}


/*============================================================================
 *  UNSAFE SYSTEM CALL
 *==========================================================================*/
void bug_unsafesystemcall(void) {
    if (system("/usr/bin/any_cmd arg1") == -1) { /* Defect: use of system function */
        /* Handle error */
        fatal_error();
    }
}

void corrected_unsafesystemcall(void) {
    char* const args[SIZE3] = {"any_cmd", "arg1", NULL};

    if (execv("/usr/bin/any_cmd", args) == -1) { /* Fix: use an execv function*/
        /* Handle error */
        fatal_error();
    }
}

/*============================================================================
 *  SENSITIVE INFORMATION LEAK VIA PADDING
 *==========================================================================*/
typedef struct s_padding_unsafe {
    char c;
    int i;                /* On 32-bit target, padding data can be introduced
                           * between 'c' and 'i' with no specific alignment instructions
                           */
    unsigned int bf1 : 1; /* Padding bits can be introduced around the bit-fields if they
                           * can not be regrouped as a whole addressable unit. This concern
                           * is unavoidable even with #pragma pack" (Microsoft Visual C/C++)
                           * or GNU __attribute__((__packed__)) on the structure.
                           */
    unsigned int bf2 : 2;
    unsigned char buffer[20];
} S_Padding_Unsafe;

#ifdef _MSC_VER
#pragma pack(push, 1) /* 1 byte aligned */
#endif

typedef struct s_padding_safe {
    char c;
    int i; /* No padding data between 'c' and 'i' with Microsoft Visual C/C++
            * #pragma pack or GNU __attribute__((__packed__))
            */
    unsigned int bf1 : 1;
    unsigned int bf2 : 2;
    unsigned int bf_filler : sizeof(unsigned) * CHAR_BIT - 3; /* Explicit padding bits */
    unsigned char buffer[20];
}
__attribute__((__packed__)) S_Padding_Safe;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

/* external function that copies 'in' to 'out' */
extern void copy_object(void* out, void* in, size_t s);

void bug_paddinginfoleak(void* out_buffer) {
    S_Padding_Unsafe s = {
        'A', 10, 1, 3, {}}; /* The existing padding data of 's' is not initialized, it may
                             * contain sensitive information from the previous execution.
                             */
    copy_object((void*)out_buffer, (void*)&s, sizeof(s)); /* Defect : possible sensitive information
                                                           * contained in the padding data escaped
                                                           * to the outside.
                                                           */
}

void corrected_paddinginfoleak(void* out_buffer) {
    S_Padding_Safe s = /* Fix : using GNU __attribute__((__packed__)) to prevent
                        * padding bytes in S_Padding_Safe, also explicitly define
                        * and initialize padding bits.
                        */
        {'A', 10, 1, 3, 0 /* padding bits */, {}};
    copy_object((void*)out_buffer, (void*)&s, sizeof(s)); /* No defect */
}

void demo_paddinginfoleak(void) {
    S_Padding_Unsafe s1;
    S_Padding_Safe s2;
    bug_paddinginfoleak(&s1);
    corrected_paddinginfoleak(&s2);
}
#endif /* __GNUC__ */
