/* File generated by GAA 1.6.3
 */
#define GAA_NO_WIN32
#line 1 "certtool.gaa"


/* C declarations */

#include <config.h>
#ifdef _WIN32
# include <io.h>
#endif

void print_license(void);
void certtool_version(void);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef GAA_NO_WIN32
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(WINDOWS)
#define GAA_WIN32
#endif
#endif

void* gaa_malloc( size_t size) {
void* ret;
	ret = malloc(size);
	if (ret==NULL) {
		fprintf(stderr, "gaa: could not allocate memory");
		exit(1);
	}
	return ret;
}

void __gaa_helpsingle(char short_name, char *name, 
	char *arg_desc, char *opt_help)
{
     int col1, col3, col4, tabsize = 3, curr;
     int i;

     col1 = 5; /* Default values */
     col3 = 30;
     col4 = 70;

     curr = 0;
     for(i = 0; i < col1; i++)
        {
        printf(" ");
        curr++;
        }
     if(short_name)
        {
        if(name && *name)
          {
            printf("-%c, ", short_name);
            curr += 4;
          }
        else
          {
            printf("-%c ", short_name);
            curr += 3;
          }
        }
     if(name && *name)
        {
          printf("--%s ", name);
          curr += 3 + strlen(name);
        }
     if(arg_desc && *arg_desc)
        {
          printf("%s ", arg_desc);
          curr += 1 + strlen(arg_desc);
        }
     if(curr >= col3)
        {
          printf("\n");
          curr = 0;
        }
     if(opt_help) /* let's print the option's help body */
        {
        const char *str = opt_help;
        while(*str)
          {
             while(curr < col3)
               {
                 printf(" ");
                 curr++;
               }
             switch(*str)
               {
                 case '\n':
                     printf("\n");
                     curr = 0;
                     break;
                 case '\t':
                     do
                        {
                        printf(" ");
                        curr++;
                        }
                     while((curr - col3) % tabsize != 0 && curr < col4);
                 case ' ':
                     if(*str == ' ')
                        {
                        curr++;
                        printf(" ");
                        }
                     for(i = 1; str[i] && str[i] != ' ' && str[i] != '\n'
                        && str[i] != '\t'; i++);
                     if(curr + i - 1 >= col4)
                        curr = col4;
                     break;
                default:
                     printf("%c", *str);
                     curr++;
               }
             if(curr >= col4)
               {
                 printf("\n");
                 curr = 0;
               }
             str++;
          }
        }
     printf("\n");
}

void gaa_help(void)
{
	printf("Certtool help\nUsage: certtool [options]\n");
	__gaa_helpsingle('s', "generate-self-signed", "", "Generate a self-signed certificate.");
	__gaa_helpsingle('c', "generate-certificate", "", "Generate a signed certificate.");
	__gaa_helpsingle('u', "update-certificate", "", "Update a signed certificate.");
	__gaa_helpsingle('p', "generate-privkey", "", "Generate a private key.");
	__gaa_helpsingle('q', "generate-request", "", "Generate a PKCS #10 certificate request.");
	__gaa_helpsingle('e', "verify-chain", "", "Verify a PEM encoded certificate chain. The last certificate in the chain must be a self signed one.");
	__gaa_helpsingle(0, "generate-dh-params", "", "Generate PKCS #3 encoded Diffie Hellman parameters.");
	__gaa_helpsingle(0, "load-privkey", "FILE ", "Private key file to use.");
	__gaa_helpsingle(0, "load-request", "FILE ", "Certificate request file to use.");
	__gaa_helpsingle(0, "load-certificate", "FILE ", "Certificate file to use.");
	__gaa_helpsingle(0, "load-ca-privkey", "FILE ", "Certificate authority's private key file to use.");
	__gaa_helpsingle(0, "load-ca-certificate", "FILE ", "Certificate authority's certificate file to use.");
	__gaa_helpsingle(0, "password", "PASSWORD ", "Password to use.");
	__gaa_helpsingle('i', "certificate-info", "", "Print information on a certificate.");
	__gaa_helpsingle('l', "crl-info", "", "Print information on a CRL.");
	__gaa_helpsingle(0, "p12-info", "", "Print information on a PKCS #12 structure.");
	__gaa_helpsingle(0, "p7-info", "", "Print information on a PKCS #7 structure.");
	__gaa_helpsingle('k', "key-info", "", "Print information on a private key.");
	__gaa_helpsingle(0, "to-p12", "", "Generate a PKCS #12 structure.");
	__gaa_helpsingle('8', "pkcs8", "", "Use PKCS #8 format for private keys.");
	__gaa_helpsingle(0, "dsa", "", "Use DSA keys.");
	__gaa_helpsingle(0, "export-ciphers", "", "Use weak encryption algorithms.");
	__gaa_helpsingle(0, "inder", "", "Use DER format for input certificates and private keys.");
	__gaa_helpsingle(0, "outder", "", "Use DER format for output certificates and private keys.");
	__gaa_helpsingle(0, "bits", "BITS ", "specify the number of bits for key generation.");
	__gaa_helpsingle(0, "outfile", "FILE ", "Output file.");
	__gaa_helpsingle(0, "infile", "FILE ", "Output file.");
	__gaa_helpsingle('d', "debug", "LEVEL ", "specify the debug level. Default is 1.");
	__gaa_helpsingle('h', "help", "", "shows this help text");
	__gaa_helpsingle('v', "version", "", "shows the program's version");
	__gaa_helpsingle(0, "copyright", "", "shows the program's license");

#line 100 "gaa.skel"
}
/* Copy of C area */

#line 104 "gaa.skel"
/* GAA HEADER */
#ifndef GAA_HEADER_POKY
#define GAA_HEADER_POKY

typedef struct _gaainfo gaainfo;

struct _gaainfo
{
#line 86 "certtool.gaa"
	int debug;
#line 83 "certtool.gaa"
	char *infile;
#line 80 "certtool.gaa"
	char *outfile;
#line 77 "certtool.gaa"
	int bits;
#line 74 "certtool.gaa"
	int outcert_format;
#line 71 "certtool.gaa"
	int incert_format;
#line 68 "certtool.gaa"
	int export;
#line 65 "certtool.gaa"
	int dsa;
#line 62 "certtool.gaa"
	int pkcs8;
#line 47 "certtool.gaa"
	char *pass;
#line 44 "certtool.gaa"
	char *ca;
#line 41 "certtool.gaa"
	char *ca_privkey;
#line 38 "certtool.gaa"
	char *cert;
#line 35 "certtool.gaa"
	char *request;
#line 32 "certtool.gaa"
	char *privkey;
#line 17 "certtool.gaa"
	int action;

#line 114 "gaa.skel"
};

#ifdef __cplusplus
extern "C"
{
#endif

    int gaa(int argc, char *argv[], gaainfo *gaaval);

    void gaa_help(void);
    
    int gaa_file(char *name, gaainfo *gaaval);
    
#ifdef __cplusplus
}
#endif


#endif

#line 135 "gaa.skel"

/* C declarations */

#define GAAERROR(x)    \
{                   \
gaa_error = 1;      \
return x;        \
}

char *gaa_current_option;
int gaa_error = 0;

/* Generated by gaa */

#include <string.h>
#include <stdlib.h>


#define GAA_OK                       -1

#define GAA_ERROR_NOMATCH            0
#define GAA_ERROR_NOTENOUGH_ARGS     1
#define GAA_ERROR_INVALID_ARG        2
#define GAA_ERROR_UNKNOWN            3

#define GAA_NOT_AN_OPTION       0
#define GAA_WORD_OPTION         1
#define GAA_LETTER_OPTION       2
#define GAA_MULTIPLE_OPTION     3

#define GAA_REST                0
#define GAA_NB_OPTION           31
#define GAAOPTID_copyright	1
#define GAAOPTID_version	2
#define GAAOPTID_help	3
#define GAAOPTID_debug	4
#define GAAOPTID_infile	5
#define GAAOPTID_outfile	6
#define GAAOPTID_bits	7
#define GAAOPTID_outder	8
#define GAAOPTID_inder	9
#define GAAOPTID_export_ciphers	10
#define GAAOPTID_dsa	11
#define GAAOPTID_pkcs8	12
#define GAAOPTID_to_p12	13
#define GAAOPTID_key_info	14
#define GAAOPTID_p7_info	15
#define GAAOPTID_p12_info	16
#define GAAOPTID_crl_info	17
#define GAAOPTID_certificate_info	18
#define GAAOPTID_password	19
#define GAAOPTID_load_ca_certificate	20
#define GAAOPTID_load_ca_privkey	21
#define GAAOPTID_load_certificate	22
#define GAAOPTID_load_request	23
#define GAAOPTID_load_privkey	24
#define GAAOPTID_generate_dh_params	25
#define GAAOPTID_verify_chain	26
#define GAAOPTID_generate_request	27
#define GAAOPTID_generate_privkey	28
#define GAAOPTID_update_certificate	29
#define GAAOPTID_generate_certificate	30
#define GAAOPTID_generate_self_signed	31

#line 168 "gaa.skel"

#define GAA_CHECK1STR(a,b)      \
if(a[0] == str[0])              \
{                               \
    gaa_current_option = a;     \
    return b;                   \
}

#define GAA_CHECKSTR(a,b)                \
if(strcmp(a,str) == 0)                   \
{                                        \
    gaa_current_option = a;              \
    return b;                            \
}

#define GAA_TESTMOREARGS                                                  \
if(!OK)                                                                     \
{                                  \
while((gaa_last_non_option != gaa_index) && (gaa_arg_used[gaa_index] == 1)) \
    gaa_index++;                                                            \
if(gaa_last_non_option == gaa_index)                                        \
    return GAA_ERROR_NOTENOUGH_ARGS; \
}

#define GAA_TESTMOREOPTIONALARGS                                                  \
if(!OK) \
{ \
while((gaa_last_non_option != gaa_index) && (gaa_arg_used[gaa_index] == 1)) \
    gaa_index++;                                                            \
if(gaa_last_non_option == gaa_index)                                        \
    OK = 1; \
}

#define GAA_FILL_2ARGS(target, func)           \
target = func(GAAargv[gaa_index]);       \
gaa_arg_used[gaa_index] = 1;             \
if(gaa_error == 1)                       \
{                                        \
    gaa_error = 0;                       \
    return GAA_ERROR_INVALID_ARG;        \
} 



#define GAA_FILL(target, func, num)           \
if(!OK) \
{ \
target = func(GAAargv[gaa_index]);       \
gaa_arg_used[gaa_index] = 1;             \
if(gaa_error == 1)                       \
{                                        \
    gaa_error = 0;                       \
    return GAA_ERROR_INVALID_ARG;        \
} \
num = 1;  \
} \
else \
{ \
num = 0; \
}

#define GAA_LIST_FILL(target, func, type ,num)                      \
if(!OK) \
{ \
num = 0;                                                            \
target = NULL;                                                      \
if ( gaa_last_non_option - gaa_index > 0)                           \
  target = gaa_malloc((gaa_last_non_option - gaa_index) * sizeof(type));\
for(; gaa_index < gaa_last_non_option; gaa_index++)                 \
{                                                                   \
    if(gaa_arg_used[gaa_index] == 0)                                \
    {                                                               \
        GAA_FILL_2ARGS(target[num], func);                          \
        num++;                                                      \
    }                                                               \
}                                                                   \
if(num == 0)                                                        \
    return GAA_ERROR_NOTENOUGH_ARGS; \
}

#define GAA_OPTIONALLIST_FILL(target, func, type ,num)                      \
if(!OK) \
{ \
num = 0;                                                            \
target = NULL;                                                      \
if ( gaa_last_non_option - gaa_index > 0)                           \
  target = gaa_malloc((gaa_last_non_option - gaa_index) * sizeof(type));\
for(; gaa_index < gaa_last_non_option; gaa_index++)                 \
{                                                                   \
    if(gaa_arg_used[gaa_index] == 0)                                \
    {                                                               \
        GAA_FILL_2ARGS(target[num], func);                                \
        num++;                                                      \
    }                                                               \
} \
}

#define GAA_OBLIGAT(str)                                            \
k = 0;                                                              \
for(i = 0; i < strlen(str); i++)                                    \
{                                                                   \
    j = gaa_get_option_num(str + i, GAA_LETTER_OPTION);           \
    if(j == GAA_ERROR_NOMATCH)                                       \
    {                                                               \
        printf("Error: invalid 'obligat' set\n");                  \
        exit(-1);                                                   \
    }                                                               \
    if(opt_list[j] == 1)                                            \
        k = 1;                                                      \
}                                                                    \
if(k == 0)                                                            \
{                                                                      \
    if(strlen(str) == 1)                                                \
        printf("You must give the -%s option\n", str);                     \
    else                                                                  \
        printf("You must give at least one option of '%s'\n", str);          \
    return 0;         \
}
        
#define GAA_INCOMP(str)                                                \
k = 0;                                                              \
for(i = 0; i < strlen(str); i++)                                    \
{                                                                   \
    j = gaa_get_option_num(str + i, GAA_LETTER_OPTION);           \
    if(j == GAA_ERROR_NOMATCH)                                      \
    {                                                               \
        printf("Error: invalid 'obligat' set\n");                  \
        exit(-1);                                                   \
    }                                                               \
    if(opt_list[j] == 1)                                            \
        k++;                                                      \
}                   \
if(k > 1)                                                            \
{                                                                      \
    printf("The options '%s' are incompatible\n", str);              \
    return 0;                                                          \
}
        

char **GAAargv;
int GAAargc;
char *gaa_arg_used;
int gaa_processing_file = 0;
int inited = 0;

int gaa_getint(char *arg)
{
    int tmp;
    char a;
    if(sscanf(arg, "%d%c", &tmp, &a) < 1)
    {
        printf("Option %s: '%s' isn't an integer\n", gaa_current_option, arg);
        GAAERROR(-1);
    }
    return tmp;
}

char gaa_getchar(char *arg)
{
    if(strlen(arg) != 1)
    {
        printf("Option %s: '%s' isn't an character\n", gaa_current_option, arg);
        GAAERROR(-1);
    }
    return arg[0];
}

char* gaa_getstr(char *arg)
{
    return arg;
}
float gaa_getfloat(char *arg)
{
    float tmp;
    char a;
    if(sscanf(arg, "%f%c", &tmp, &a) < 1)
    {
        printf("Option %s: '%s' isn't a float number\n", gaa_current_option, arg);
        GAAERROR(-1);
    }
    return tmp;
}
/* option structures */

struct GAAOPTION_debug 
{
	int arg1;
	int size1;
};

struct GAAOPTION_infile 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_outfile 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_bits 
{
	int arg1;
	int size1;
};

struct GAAOPTION_password 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_load_ca_certificate 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_load_ca_privkey 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_load_certificate 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_load_request 
{
	char* arg1;
	int size1;
};

struct GAAOPTION_load_privkey 
{
	char* arg1;
	int size1;
};
         
#line 349 "gaa.skel"
int gaa_is_an_argument(char *str)
{
#ifdef GAA_WIN32
    if(str[0] == '/' && str[1] != 0)
	return GAA_MULTIPLE_OPTION;
#endif
    if(str[0] != '-')
        return GAA_NOT_AN_OPTION;
    if(str[1] == 0)
        return GAA_NOT_AN_OPTION;
    if(str[1] == '-')
    {
        if(str[2] != 0)
            return GAA_WORD_OPTION;
        else
            return GAA_NOT_AN_OPTION;
    }
    if(str[2] == 0)
        return GAA_LETTER_OPTION;
    else
        return GAA_MULTIPLE_OPTION;
}

int gaa_get_option_num(char *str, int status)
{
    switch(status)
        {
        case GAA_LETTER_OPTION:
			GAA_CHECK1STR("d", GAAOPTID_debug);
			GAA_CHECK1STR("", GAAOPTID_infile);
			GAA_CHECK1STR("", GAAOPTID_outfile);
			GAA_CHECK1STR("", GAAOPTID_bits);
			GAA_CHECK1STR("", GAAOPTID_password);
			GAA_CHECK1STR("", GAAOPTID_load_ca_certificate);
			GAA_CHECK1STR("", GAAOPTID_load_ca_privkey);
			GAA_CHECK1STR("", GAAOPTID_load_certificate);
			GAA_CHECK1STR("", GAAOPTID_load_request);
			GAA_CHECK1STR("", GAAOPTID_load_privkey);
        case GAA_MULTIPLE_OPTION:
#line 375 "gaa.skel"
			GAA_CHECK1STR("", GAAOPTID_copyright);
			GAA_CHECK1STR("v", GAAOPTID_version);
			GAA_CHECK1STR("h", GAAOPTID_help);
			GAA_CHECK1STR("", GAAOPTID_outder);
			GAA_CHECK1STR("", GAAOPTID_inder);
			GAA_CHECK1STR("", GAAOPTID_export_ciphers);
			GAA_CHECK1STR("", GAAOPTID_dsa);
			GAA_CHECK1STR("8", GAAOPTID_pkcs8);
			GAA_CHECK1STR("", GAAOPTID_to_p12);
			GAA_CHECK1STR("k", GAAOPTID_key_info);
			GAA_CHECK1STR("", GAAOPTID_p7_info);
			GAA_CHECK1STR("", GAAOPTID_p12_info);
			GAA_CHECK1STR("l", GAAOPTID_crl_info);
			GAA_CHECK1STR("i", GAAOPTID_certificate_info);
			GAA_CHECK1STR("", GAAOPTID_generate_dh_params);
			GAA_CHECK1STR("e", GAAOPTID_verify_chain);
			GAA_CHECK1STR("q", GAAOPTID_generate_request);
			GAA_CHECK1STR("p", GAAOPTID_generate_privkey);
			GAA_CHECK1STR("u", GAAOPTID_update_certificate);
			GAA_CHECK1STR("c", GAAOPTID_generate_certificate);
			GAA_CHECK1STR("s", GAAOPTID_generate_self_signed);

#line 277 "gaa.skel"
        break;
        case GAA_WORD_OPTION:
			GAA_CHECKSTR("copyright", GAAOPTID_copyright);
			GAA_CHECKSTR("version", GAAOPTID_version);
			GAA_CHECKSTR("help", GAAOPTID_help);
			GAA_CHECKSTR("debug", GAAOPTID_debug);
			GAA_CHECKSTR("infile", GAAOPTID_infile);
			GAA_CHECKSTR("outfile", GAAOPTID_outfile);
			GAA_CHECKSTR("bits", GAAOPTID_bits);
			GAA_CHECKSTR("outder", GAAOPTID_outder);
			GAA_CHECKSTR("inder", GAAOPTID_inder);
			GAA_CHECKSTR("export-ciphers", GAAOPTID_export_ciphers);
			GAA_CHECKSTR("dsa", GAAOPTID_dsa);
			GAA_CHECKSTR("pkcs8", GAAOPTID_pkcs8);
			GAA_CHECKSTR("to-p12", GAAOPTID_to_p12);
			GAA_CHECKSTR("key-info", GAAOPTID_key_info);
			GAA_CHECKSTR("p7-info", GAAOPTID_p7_info);
			GAA_CHECKSTR("p12-info", GAAOPTID_p12_info);
			GAA_CHECKSTR("crl-info", GAAOPTID_crl_info);
			GAA_CHECKSTR("certificate-info", GAAOPTID_certificate_info);
			GAA_CHECKSTR("password", GAAOPTID_password);
			GAA_CHECKSTR("load-ca-certificate", GAAOPTID_load_ca_certificate);
			GAA_CHECKSTR("load-ca-privkey", GAAOPTID_load_ca_privkey);
			GAA_CHECKSTR("load-certificate", GAAOPTID_load_certificate);
			GAA_CHECKSTR("load-request", GAAOPTID_load_request);
			GAA_CHECKSTR("load-privkey", GAAOPTID_load_privkey);
			GAA_CHECKSTR("generate-dh-params", GAAOPTID_generate_dh_params);
			GAA_CHECKSTR("verify-chain", GAAOPTID_verify_chain);
			GAA_CHECKSTR("generate-request", GAAOPTID_generate_request);
			GAA_CHECKSTR("generate-privkey", GAAOPTID_generate_privkey);
			GAA_CHECKSTR("update-certificate", GAAOPTID_update_certificate);
			GAA_CHECKSTR("generate-certificate", GAAOPTID_generate_certificate);
			GAA_CHECKSTR("generate-self-signed", GAAOPTID_generate_self_signed);

#line 281 "gaa.skel"
	break;
        default: break;
        }
    return GAA_ERROR_NOMATCH;
}

int gaa_try(int gaa_num, int gaa_index, gaainfo *gaaval, char *opt_list)
{
    int OK = 0;
    int gaa_last_non_option;
	struct GAAOPTION_debug GAATMP_debug;
	struct GAAOPTION_infile GAATMP_infile;
	struct GAAOPTION_outfile GAATMP_outfile;
	struct GAAOPTION_bits GAATMP_bits;
	struct GAAOPTION_password GAATMP_password;
	struct GAAOPTION_load_ca_certificate GAATMP_load_ca_certificate;
	struct GAAOPTION_load_ca_privkey GAATMP_load_ca_privkey;
	struct GAAOPTION_load_certificate GAATMP_load_certificate;
	struct GAAOPTION_load_request GAATMP_load_request;
	struct GAAOPTION_load_privkey GAATMP_load_privkey;

#line 393 "gaa.skel"
#ifdef GAA_REST_EXISTS
    struct GAAREST GAAREST_tmp;
#endif

    opt_list[gaa_num] = 1;
    
    for(gaa_last_non_option = gaa_index;
        (gaa_last_non_option != GAAargc) && (gaa_is_an_argument(GAAargv[gaa_last_non_option]) == GAA_NOT_AN_OPTION);
        gaa_last_non_option++);

    if(gaa_num == GAA_REST)
    {
        gaa_index = 1;
        gaa_last_non_option = GAAargc;
    }
    
    switch(gaa_num)
    {
	case GAAOPTID_copyright:
	OK = 0;
#line 92 "certtool.gaa"
{ print_license(); exit(0); ;};

		return GAA_OK;
		break;
	case GAAOPTID_version:
	OK = 0;
#line 91 "certtool.gaa"
{ certtool_version(); exit(0); ;};

		return GAA_OK;
		break;
	case GAAOPTID_help:
	OK = 0;
#line 89 "certtool.gaa"
{ gaa_help(); exit(0); ;};

		return GAA_OK;
		break;
	case GAAOPTID_debug:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_debug.arg1, gaa_getint, GAATMP_debug.size1);
		gaa_index++;
#line 87 "certtool.gaa"
{ gaaval->debug = GAATMP_debug.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_infile:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_infile.arg1, gaa_getstr, GAATMP_infile.size1);
		gaa_index++;
#line 84 "certtool.gaa"
{ gaaval->infile = GAATMP_infile.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_outfile:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_outfile.arg1, gaa_getstr, GAATMP_outfile.size1);
		gaa_index++;
#line 81 "certtool.gaa"
{ gaaval->outfile = GAATMP_outfile.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_bits:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_bits.arg1, gaa_getint, GAATMP_bits.size1);
		gaa_index++;
#line 78 "certtool.gaa"
{ gaaval->bits = GAATMP_bits.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_outder:
	OK = 0;
#line 75 "certtool.gaa"
{ gaaval->outcert_format=1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_inder:
	OK = 0;
#line 72 "certtool.gaa"
{ gaaval->incert_format=1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_export_ciphers:
	OK = 0;
#line 69 "certtool.gaa"
{ gaaval->export=1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_dsa:
	OK = 0;
#line 66 "certtool.gaa"
{ gaaval->dsa=1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_pkcs8:
	OK = 0;
#line 63 "certtool.gaa"
{ gaaval->pkcs8=1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_to_p12:
	OK = 0;
#line 60 "certtool.gaa"
{ gaaval->action = 8; ;};

		return GAA_OK;
		break;
	case GAAOPTID_key_info:
	OK = 0;
#line 58 "certtool.gaa"
{ gaaval->action = 6; ;};

		return GAA_OK;
		break;
	case GAAOPTID_p7_info:
	OK = 0;
#line 56 "certtool.gaa"
{ gaaval->action = 12; ;};

		return GAA_OK;
		break;
	case GAAOPTID_p12_info:
	OK = 0;
#line 54 "certtool.gaa"
{ gaaval->action = 9; ;};

		return GAA_OK;
		break;
	case GAAOPTID_crl_info:
	OK = 0;
#line 52 "certtool.gaa"
{ gaaval->action = 11; ;};

		return GAA_OK;
		break;
	case GAAOPTID_certificate_info:
	OK = 0;
#line 50 "certtool.gaa"
{ gaaval->action = 2; ;};

		return GAA_OK;
		break;
	case GAAOPTID_password:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_password.arg1, gaa_getstr, GAATMP_password.size1);
		gaa_index++;
#line 48 "certtool.gaa"
{ gaaval->pass = GAATMP_password.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_load_ca_certificate:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_load_ca_certificate.arg1, gaa_getstr, GAATMP_load_ca_certificate.size1);
		gaa_index++;
#line 45 "certtool.gaa"
{ gaaval->ca = GAATMP_load_ca_certificate.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_load_ca_privkey:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_load_ca_privkey.arg1, gaa_getstr, GAATMP_load_ca_privkey.size1);
		gaa_index++;
#line 42 "certtool.gaa"
{ gaaval->ca_privkey = GAATMP_load_ca_privkey.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_load_certificate:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_load_certificate.arg1, gaa_getstr, GAATMP_load_certificate.size1);
		gaa_index++;
#line 39 "certtool.gaa"
{ gaaval->cert = GAATMP_load_certificate.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_load_request:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_load_request.arg1, gaa_getstr, GAATMP_load_request.size1);
		gaa_index++;
#line 36 "certtool.gaa"
{ gaaval->request = GAATMP_load_request.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_load_privkey:
	OK = 0;
		GAA_TESTMOREARGS;
		GAA_FILL(GAATMP_load_privkey.arg1, gaa_getstr, GAATMP_load_privkey.size1);
		gaa_index++;
#line 33 "certtool.gaa"
{ gaaval->privkey = GAATMP_load_privkey.arg1 ;};

		return GAA_OK;
		break;
	case GAAOPTID_generate_dh_params:
	OK = 0;
#line 30 "certtool.gaa"
{ gaaval->action=10; ;};

		return GAA_OK;
		break;
	case GAAOPTID_verify_chain:
	OK = 0;
#line 28 "certtool.gaa"
{ gaaval->action=5; ;};

		return GAA_OK;
		break;
	case GAAOPTID_generate_request:
	OK = 0;
#line 26 "certtool.gaa"
{ gaaval->action=3; ;};

		return GAA_OK;
		break;
	case GAAOPTID_generate_privkey:
	OK = 0;
#line 24 "certtool.gaa"
{ gaaval->action=1; ;};

		return GAA_OK;
		break;
	case GAAOPTID_update_certificate:
	OK = 0;
#line 22 "certtool.gaa"
{ gaaval->action=7; ;};

		return GAA_OK;
		break;
	case GAAOPTID_generate_certificate:
	OK = 0;
#line 20 "certtool.gaa"
{ gaaval->action=4; ;};

		return GAA_OK;
		break;
	case GAAOPTID_generate_self_signed:
	OK = 0;
#line 18 "certtool.gaa"
{ gaaval->action=0; ;};

		return GAA_OK;
		break;

#line 413 "gaa.skel"
    default: break;
    }
    return GAA_ERROR_UNKNOWN;
}

int gaa(int argc, char **argv, gaainfo *gaaval)
{
    int tmp1, tmp2;
    int i, j;
    char *opt_list;

    GAAargv = argv;
    GAAargc = argc;

    opt_list = (char*) gaa_malloc(GAA_NB_OPTION + 1);

    for(i = 0; i < GAA_NB_OPTION + 1; i++)
        opt_list[i] = 0;
    /* initialization */
    if(inited == 0)
    {

#line 94 "certtool.gaa"
{ gaaval->bits = 1024; gaaval->pkcs8 = 0; gaaval->privkey = NULL; gaaval->ca=NULL; gaaval->ca_privkey = NULL; 
	gaaval->debug=1; gaaval->request = NULL; gaaval->infile = NULL; gaaval->outfile = NULL; gaaval->cert = NULL; 
	gaaval->incert_format = 0; gaaval->outcert_format = 0; gaaval->action=-1; gaaval->pass = NULL; 
	gaaval->export = 0; ;};

    }
    inited = 1;
#line 438 "gaa.skel"
    gaa_arg_used = NULL;

    if (argc > 0) {
      gaa_arg_used = gaa_malloc(argc * sizeof(char));
    }

    for(i = 1; i < argc; i++)
        gaa_arg_used[i] = 0;
    for(i = 1; i < argc; i++)
    {
        if(gaa_arg_used[i] == 0)
        {
            j = 0;
            tmp1 = gaa_is_an_argument(GAAargv[i]);
            switch(tmp1)
            {
            case GAA_WORD_OPTION:
                j++;
            case GAA_LETTER_OPTION:
                j++;
                tmp2 = gaa_get_option_num(argv[i]+j, tmp1);
                if(tmp2 == GAA_ERROR_NOMATCH)
                {
                    printf("Invalid option '%s'\n", argv[i]+j);
                    return 0;
                }
                switch(gaa_try(tmp2, i+1, gaaval, opt_list))
                {
                case GAA_ERROR_NOTENOUGH_ARGS:
                    printf("'%s': not enough arguments\n",gaa_current_option);
                    return 0;
                case GAA_ERROR_INVALID_ARG:
                    printf("Invalid arguments\n");
                    return 0;
                case GAA_OK:
                    break;
                default:
                    printf("Unknown error\n");
                }
                gaa_arg_used[i] = 1;
                break;
            case GAA_MULTIPLE_OPTION:
                for(j = 1; j < strlen(argv[i]); j++)
                {
                    tmp2 = gaa_get_option_num(argv[i]+j, tmp1);
                    if(tmp2 == GAA_ERROR_NOMATCH)
                    {
                        printf("Invalid option '%c'\n", *(argv[i]+j));
                        return 0;
                    }
                    switch(gaa_try(tmp2, i+1, gaaval, opt_list))
                    {
                    case GAA_ERROR_NOTENOUGH_ARGS:
                        printf("'%s': not enough arguments\n",gaa_current_option);
                        return 0;
                    case GAA_ERROR_INVALID_ARG:
                        printf("Invalid arguments\n");
                        return 0;
                    case GAA_OK:
                        break;
                    default:
                        printf("Unknown error\n");
                    }
                }
                gaa_arg_used[i] = 1;
                break;
            default: break;
            }
        }
    }
if(gaa_processing_file == 0)
{

#line 507 "gaa.skel"
#ifdef GAA_REST_EXISTS
    switch(gaa_try(GAA_REST, 1, gaaval, opt_list))
    {
    case GAA_ERROR_NOTENOUGH_ARGS:
        printf("Rest: not enough arguments\n");
        return 0;
    case GAA_ERROR_INVALID_ARG:
        printf("Invalid arguments\n");
        return 0;
    case GAA_OK:
        break;
    default:
        printf("Unknown error\n");
    }
#endif
}
    for(i = 1; i < argc; i++)
    {
        if(gaa_arg_used[i] == 0)
        {
            printf("Too many arguments\n");
            return 0;
        }
    }
    free(gaa_arg_used);
    free(opt_list);
    return -1;
}

struct gaastrnode
{
    char *str;
    struct gaastrnode *next;
};

typedef struct gaastrnode gaa_str_node;

int gaa_internal_get_next_str(FILE *file, gaa_str_node *tmp_str, int argc)
{
    int pos_ini;
    int a;
    int i = 0, len = 0, newline = 0;

    if(argc == 1) {
        newline = 1;
        len = 2;
    }
    
    a = fgetc( file);
    if (a == EOF) return 0;

    while(a == ' ' || a == 9 || a == '\n')
    {
        if(a == '\n')
        {
            newline=1;
            len = 2;
        }
        a = fgetc( file);
        if (a == EOF) return 0;
    }

    pos_ini = ftell(file) - 1;

    while(a != ' ' && a != 9 && a != '\n')
    {

        len++;
        a = fgetc( file);
        if(a==EOF) return 0; //a = ' ';
    }

    len += 1;
    tmp_str->str = gaa_malloc((len) * sizeof(char));

    if(newline == 1)
    {
        tmp_str->str[0] = '-';
        tmp_str->str[1] = '-';
        i = 2;
    }
    else
    {
        i = 0;
    }

    fseek(file,pos_ini, SEEK_SET);
    do
    {
        if(fscanf(file, "%c", &a) != 1)
        {
            i+=2;
            break;
        }
        tmp_str->str[i] = a;
        i++;
    }
    while(a != ' ' && a != 9 && a != '\n' && i < len);

    tmp_str->str[i - 1] = 0;

    fseek(file,- 1, SEEK_CUR);
/*    printf("%d\n", ftell(file)); */
    
    return -1;
}

int gaa_file(char *name, gaainfo *gaaval)
{
    gaa_str_node *first_str, **tmp_str, *tmp_str2;
    int rval, i;
    char **argv;
    int argc = 0;
    FILE *file;

    gaa_processing_file = 1;
    
    if((file = fopen(name, "r")) == NULL)
    {
        printf("Couldn't open '%s' configuration file for reading\n", name);
        return 1;
    }
    
    tmp_str = &first_str;
    do
    {
        argc++;
        *tmp_str = gaa_malloc(sizeof(gaa_str_node));

        (*tmp_str)->str = NULL;
        (*tmp_str)->next = NULL;

        rval = gaa_internal_get_next_str(file, *tmp_str, argc);
        tmp_str = &((*tmp_str)->next);
    }
    while(rval == -1);
    
    if(rval == 1)
        return 0;
    
    argv = gaa_malloc((1 + argc) * sizeof(char*));

    tmp_str2 = first_str;
    argv[0] = "cfg";
    for(i = 1; i < argc; i++)
    {
        argv[i] = tmp_str2->str;
        tmp_str2 = tmp_str2->next;
    }

    rval = gaa(argc, argv, gaaval);
    gaa_processing_file = 0;
    return rval;
}
