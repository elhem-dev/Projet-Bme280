#include "bme280.h"
#include <time.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024

void error(char *msg) {
    perror(msg);
    exit(0);
}
int main() {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    struct in_addr ipv4addr;

    hostname = "10.0.2.237";
    portno = 4242;
while (1)
{
    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");


        unsigned long addr;

        server = gethostbyname(hostname);
        if (server == NULL) {
            addr = inet_addr(hostname);
            server = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
            if (server == NULL) {
                perror("gethostbyaddr");
                exit(0);
    }
  }

//    server = gethostbyaddr(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
      (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);


  int fd = wiringPiI2CSetup(BME280_ADDRESS);
  if(fd < 0) {
    printf("Device not found");
    return -1;
  }

  bme280_calib_data cal;
  readCalibrationData(fd, &cal);

  wiringPiI2CWriteReg8(fd, 0xf2, 0x01);   // humidity oversampling x 1
  wiringPiI2CWriteReg8(fd, 0xf4, 0x25);   // pressure and temperature oversampling x 1, mode normal

  bme280_raw_data raw;
  getRawData(fd, &raw);

  int32_t t_fine = getTemperatureCalibration(&cal, raw.temperature);
  float t = compensateTemperature(t_fine); // C
  float p = compensatePressure(raw.pressure, &cal, t_fine) / 100; // hPa
  float h = compensateHumidity(raw.humidity, &cal, t_fine);       // %
  float a = getAltitude(p);   // meters

  /* connect: create a connection with the server */
  if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR connecting");

   /* get message line from the user */
  char tamp[2048]={0};
 sleep(3);
  sprintf(tamp,"[{\"sensor\":\"bme280\", \"humidity\":%.2f, \"pressure\":%.2f,"
    " \"temperature\":%.2f, \"altitude\":%.2f, \"timestamp\":%d}]\n", h, p, t, a, (int)time(NULL));



  /* send the message line to the server */
  n = write(sockfd, tamp, strlen(tamp));
  if (n < 0)
    error("ERROR writing to socket");
//  /* print the server's reply */
//  bzero(buf, BUFSIZE);
//  n = read(sockfd, buf, BUFSIZE);
//  if (n < 0)
//    error("ERROR reading from socket");
//  printf("Echo from server: %s", buf);
  close(sockfd);

}
  return 0;
}
