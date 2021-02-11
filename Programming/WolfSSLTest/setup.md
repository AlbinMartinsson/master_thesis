# Setup for the wolfSSL and arrowhead certificate.

## WolfSSL
Follow the instructions on this [page](www.gerjer.com) on how to install the wolfSSL pack for stm32CUBE. 

## Certificates

* Follow the steps in this [link](https://github.com/eclipse-arrowhead/core-java-spring/blob/master/documentation/certificates/create_client_certificate.pdf) on how to to generate a certificate.

* Convert your certificate.p12 by running openssl pkcs12 -in certificate.p12 -out certificate.pem -clcerts -nokeys and then openssl x509 -in certificate.pem -outform pem -outform der -out certificate.cer to convert it into x.509 formatting.

## Running the test project

### Adding certificate buffers to wolfSSL 

* Go to the root folder of your wolfSSL installation, normally ~\STM32Cube\Repository\Packs\wolfSSL\I-CUBE-wolfSSL\4.6.0\Middlewares\Third_Party\wolfssl.

* Run the perl script dertoc.pl by in a command line typing perl /scripts/dertoc.pl pathtoinputfile outputname outputfile as arguments. This will put a correctly encoded certificate buffer in your output file which can be included in your project as a header file looking like the hert_template.h file. (Which will have to be included in main.h after creation).

### Building and running the test project.

* Once the certificate buffer is in order it is simply a matter of running the project on a STM32L4+ Discovery kit in STM32cubeIDE





