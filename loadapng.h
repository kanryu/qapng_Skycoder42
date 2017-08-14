#ifndef LOADAPNG_H
#define LOADAPNG_H

#include <QtCore>
#include <vector>

struct APNGFrame { unsigned char * p, ** rows; unsigned int w, h, delay_num, delay_den; };

int check_apng(QIODevice *device, QSize& imageSize, bool& animated);
int load_apng(QIODevice *device, std::vector<APNGFrame>& frames);

#endif // LOADAPNG_H
