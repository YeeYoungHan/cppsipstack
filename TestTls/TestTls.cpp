/* cli.cpp  -  Minimal ssleay client for Unix 
   30.9.1996, Sampo Kellomaki <sampo@iki.fi> */ 

/* mangled to work with SSLeay-0.9.0b and OpenSSL 0.9.2b 
   Simplified to be even more minimal 
   12/98 - 4/99 Wade Scholine <wades@mail.cybg.com> */ 

#include "SipPlatformDefine.h"

#include <stdio.h> 
#include <memory.h> 
#include <errno.h> 
#include <sys/types.h> 
#include "SipUdp.h"

#include <openssl/crypto.h> 
#include <openssl/x509.h> 
#include <openssl/pem.h> 
#include <openssl/ssl.h> 
#include <openssl/err.h> 

#define CRLF      "\r\n"
#define CRLFCRLF  "\r\n\r\n"

/* Make these what you want for cert & key files */
#define CERTF "C:\\Temp\\new\\client.crt"
#define KEYF  "C:\\Temp\\new\\client.key"

#define CHK_NULL(x) if ((x)==NULL) exit (1) 
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); } 
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); } 

void print_x509(SSL *ssl) 
{ 
    char *ascii_cert; 
    X509 *cert = SSL_get_peer_certificate(ssl); 
    BIO *b; 
    BUF_MEM *bptr; 

    b = BIO_new(BIO_s_mem()); 

    if(X509_print(b, cert) > 0) 
    { 
        BIO_get_mem_ptr(b, &bptr); 
        ascii_cert = (char *)malloc(1 + bptr->length); 
        memcpy(ascii_cert, bptr->data, bptr->length); 
    } 
    else 
    { 
        ascii_cert = (char *)malloc(1024); 
        sprintf(ascii_cert, "This certificate has never been seen before and can't be shown\n"); 
    } 
    BIO_free(b); 

    /* X.509 인증서 출력 */ 
    printf("X.509:\n%s\n", ascii_cert); 
} 

int main () 
{ 
    int err = 0; 
    int sd; 
    struct sockaddr_in sa; 

    SSL_CTX* ctx; 
    SSL*     ssl; 
    X509*    server_cert; 

    char*    str; 
    char     buf [4096]; 
    X509_NAME * name;
    char     commonName [512]; 

    SSL_METHOD *meth; 

	InitNetwork();

    memset(buf, 0, 4096);

    strcpy(buf, "REGISTER sips:192.168.1.142:5061 SIP/2.0");
	strcat(buf, CRLF);
	strcat(buf, "Max-Forwards: 70");
	strcat(buf, CRLF);
	strcat(buf, "Content-Length: 0");
	strcat(buf, CRLF);
	strcat(buf, "Via: SIP/2.0/TLS 192.168.1.142:5060;branch=z9hG4bKba05d177b;rport");
	strcat(buf, CRLF);
	strcat(buf, "Call-ID: e0cd67001414191dbc4ff911f1a7329c@192.168.1.142");
	strcat(buf, CRLF);
	strcat(buf, "From: <sips:07070001004@192.168.1.142:5061>;tag=e60c96951bac6ae");
	strcat(buf, CRLF);
	strcat(buf, "To: <sips:07070001004@192.168.1.142:5061>");
	strcat(buf, CRLF);
	strcat(buf, "CSeq: 9 REGISTER");
	strcat(buf, CRLF);
	strcat(buf, "Contact: <sips:07070001004@192.168.1.142:5061>;expires=60");
	strcat(buf, CRLF);
	strcat(buf, "Expires: 60");
	strcat(buf, CRLFCRLF);

    SSL_library_init();
    meth = TLSv1_client_method(); 
    SSL_load_error_strings(); 
    SSLeay_add_ssl_algorithms();
    ctx = SSL_CTX_new (meth); 
    CHK_NULL(ctx); 
    CHK_SSL(err); 


    /////////////////////////////////////////////////////////////////////////////////////// 
    // Create a socket and connect to server using normal socket calls. 
    sd = socket (AF_INET, SOCK_STREAM, 0);   
    CHK_ERR(sd, "socket"); 

    memset (&sa, '\0', sizeof(sa)); 
    sa.sin_family      = AF_INET; 
    sa.sin_addr.s_addr = inet_addr ("211.232.179.68");   /* Server IP */ 
    sa.sin_port        = htons     (5065);          /* Server Port number */ 

    // Normal-connect 
    err = connect(sd, (struct sockaddr*) &sa, sizeof(sa));      
    CHK_ERR(err, "connect"); 

//    SSL_set_cipher_list(ssl, "AES128-SHA,ARIA128-SHA");

    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(3);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(4);
    }

#if 0
    if (SSL_CTX_load_verify_locations(ctx, "./ca.crt", NULL) <= 0) {
         SSL_CTX_free(ctx);
         return;
    }
#endif

    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr,"Private key does not match the certificate public key\n");
        exit(5);
    }

    // Now we have TCP conncetion. Start SSL negotiation. 
    // create a new SSL structure for a connection 
    ssl = SSL_new (ctx);     
    CHK_NULL(ssl);     

    // connect the SSL object with a file descriptor 
    SSL_set_fd (ssl, sd); 

    // initiate the TLS/SSL handshake with an TLS/SSL server 
    err = SSL_connect (ssl);     
    CHK_SSL(err); 

    /*  
     * Following two steps are optional and not required for 
     * data exchange to be successful.  
     */ 

    printf ("SSL connection using %s\n", SSL_get_cipher (ssl)); 

#if 0
    // Get server's certificate (note: beware of dynamic allocation) - opt  
    server_cert = SSL_get_peer_certificate (ssl);     
    if (server_cert != NULL)
    {
        print_x509(ssl); 
        X509_free (server_cert); 
    }
    else
    {
        printf ("Server does not have certificate.\n");
    }
#endif

    // DATA EXCHANGE - Send a message and receive a reply. 
    //err = SSL_write (ssl, "Hello World!", strlen("Hello World!")); 
    //CHK_SSL(err);  
    err = SSL_write (ssl, buf, strlen(buf));   
    CHK_SSL(err); 

    printf ("Send chars:'%s'\n", buf); 

    memset(buf, 0, 4096);

    err = SSL_read (ssl, buf, sizeof(buf) - 1);     
    CHK_SSL(err); 
    buf[err] = '\0'; 
    printf ("Got %d chars:'%s'\n", err, buf); 

    // send SSL/TLS close_notify 
    SSL_shutdown (ssl);   

    /* Clean up. */ 

    close (sd); 
    SSL_free (ssl); 
    SSL_CTX_free (ctx); 

    return 0;
} 

