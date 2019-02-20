#ifndef VIDEOCHAT_H
#define VIDEOCHAT_H

#include <QObject>
#include <QThread>
#include <QCamera>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStack>

#define __STDINT_MACROS
#define __STDC_CONSTANT_MACROS

extern "C"{
//#include <x264.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}
#include <opencv2/opencv.hpp>

#define RGB_SIZE  640*480*3
#define FOOTER ";FAFFAAAA"

class x264Decoder;
class x264Encoder;
class recorder;
class videoclient;
class videoserver;
class QTcpServer;
class QTcpSocket;

class recorder : public QThread
{
    Q_OBJECT
public:
    explicit recorder(QObject *parent = nullptr);

    static unsigned int pack_rgb_data(QByteArray &data, x264Encoder *x264_encoder);

signals:
    void frame(QByteArray frame);

public slots:

private:
    void run() override;

};


#define FOOTER ";FAFFAAAA"

class videoserver : public QThread
{
    Q_OBJECT
public:
    explicit videoserver();
    videoserver(const videoserver & copy );
    ~videoserver();

signals:

public slots:
    void onIncConnection();
    void clientDisconnected();

    void writeData(QByteArray payload);
private:
    QTcpServer * server;
    QStack<QTcpSocket*>clients;

    void run() override;


};


class x264Decoder{

public:

    void initialize(int w, int h);
    bool decodeFrame(char *src_buffer, unsigned int size, unsigned char *rgb_buffer);
private:
    AVFrame* av_frame_;
    AVFrame *av_frame_RGB_;

    // Use this context to convert your BGR Image to YUV image since x264 do not support RGB input
    AVCodec* av_codec_ = NULL;
    AVCodecContext* av_codec_context_ = NULL;
    SwsContext* convert_context_;
    int image_w_;
    int image_h_;
    int rgb_size_;
};

class videoclient : public QThread
{
    Q_OBJECT
public:
    explicit videoclient(QString clientip,QObject *parent = nullptr);

signals:
    void frame(QImage frame);

public slots:
    void dataReady();

private:
    void run() override;
    QTcpSocket * socketo;
    QByteArray buffer;
    QString ip;
};

class x264Encoder
{

public:
    void initialize(int w, int h);
    void unInitilize();
    void encodeFrame(unsigned char *rgb_buffer, int buffer_size);
    bool isNalsAvailableInOutputQueue();
    int image_h_;
    int image_w_;

    //x264_nal_t getNalUnit();
    //x264_t *getx264Encoder() { return encoder_; }
    //int nal_size() { return output_queue_.size(); }
private:
    // Use this context to convert your BGR Image to YUV image since x264 do not support RGB input
    SwsContext* convert_context_ = NULL;
    //std::queue<x264_nal_t> output_queue_;
    //x264_param_t parameters_;
    //x264_picture_t picture_in_, picture_out_;
    //x264_t* encoder_;

};


class videochat : public QObject
{
    Q_OBJECT
public:
    explicit videochat(QObject *parent = nullptr);
    void reciver(QString userip);
    void vsender();

signals:
    void newFrame(QImage img);

public slots:

private:
    recorder * rec;
    videoserver * server;
    videoclient * client;

};




#endif // VIDEOCHAT_H
