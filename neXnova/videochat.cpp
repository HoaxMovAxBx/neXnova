#include "videochat.h"

videochat::videochat(QObject *parent) : QObject(parent)
{

}

void videochat::reciver(QString userip)
{   
    client = new videoclient(userip);
    connect(client,&videoclient::frame,this,&videochat::newFrame,Qt::QueuedConnection);
    client->start();
}

void videochat::vsender()
{

    server = new videoserver;
    server->start();

    rec = new recorder;
    connect(rec,&recorder::frame,server,&videoserver::writeData,Qt::QueuedConnection);
    rec->start();

}


recorder::recorder(QObject *parent) : QThread(parent)
{

}

unsigned int recorder::pack_rgb_data(QByteArray &data, x264Encoder *x264_encoder)
{
//    x264_nal_t nal;
    uint32_t tmp_size = 0;

    while(x264_encoder->isNalsAvailableInOutputQueue()){
  //      nal = x264_encoder->getNalUnit();
  //      data.append(QByteArray::fromRawData(reinterpret_cast<char*>(nal.p_payload),nal.i_payload));
  //      tmp_size += nal.i_payload;
    }
    return sizeof(uint32_t) * 2 + tmp_size;
}

void recorder::run()
{

    qDebug() << "Recording ..."<<endl;

    //x264Encoder x264_encoder;

    //x264_encoder.initialize(640, 480);
    cv::VideoCapture vc(0);
    cv::Mat image;
    QByteArray data;
    uint32_t frameId = 0;
    forever{
        //Any grabber to get raw rgb data from camera.
        if(vc.read(image)){
            //x264_encoder.encodeFrame(image.data, RGB_SIZE);
            //uint32_t colorsize = pack_rgb_data(data,&x264_encoder);
            data = QByteArray::fromRawData(reinterpret_cast<char*>(image.data),RGB_SIZE);
            emit frame(data);
            data.clear();
            frameId++;
        }
        //msleep(200);
    }
}

using namespace std;



void x264Encoder::initialize(int w = 640, int h = 480)
{
/*    image_w_ = w;
    image_h_ = h;
    x264_param_default_preset(&parameters_, "veryfast", "zerolatency");
        parameters_.i_log_level = X264_LOG_INFO;
        parameters_.i_threads = 1;
        parameters_.i_width = image_w_;
        parameters_.i_height = image_h_;
        parameters_.i_fps_num = 30;
        parameters_.i_fps_den = 1;
        parameters_.i_keyint_max = 25;
        parameters_.b_intra_refresh = 1;
        parameters_.rc.i_rc_method = X264_RC_CRF;
        parameters_.rc.i_vbv_buffer_size = 2000000;
        parameters_.rc.i_vbv_max_bitrate = 90000;
        parameters_.rc.f_rf_constant = 25;
        parameters_.rc.f_rf_constant_max = 35;
        parameters_.i_sps_id = 7;
        // the following two value you should keep 1
        parameters_.b_repeat_headers = 1;    // to get header before every I-Frame
        parameters_.b_annexb = 1; // put start code in front of nal. we will remove start code later
        x264_param_apply_profile(&parameters_, "high");

    encoder_ = x264_encoder_open(&parameters_);

    picture_in_.i_qpplus1         = 0;
    picture_in_.img.i_plane       = 1;
    picture_in_.i_type = X264_TYPE_AUTO;
    picture_in_.img.i_csp = X264_CSP_I420;
    x264_picture_alloc(&picture_in_, X264_CSP_I420, parameters_.i_width, parameters_.i_height);
    convert_context_ = sws_getContext(parameters_.i_width, parameters_.i_height, AV_PIX_FMT_RGB24, parameters_.i_width, parameters_.i_height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);
*/
}

void x264Encoder::unInitilize()
{
  //  x264_encoder_close(encoder_);
  //  sws_freeContext(convert_context_);
}

//Encode the rgb frame into a sequence of NALs unit that are stored in a std::vector
void x264Encoder::encodeFrame(unsigned char *rgb_buffer, int buffer_size)
{
   /* const uint8_t * rgb_buffer_slice[1] = {(const uint8_t *)rgb_buffer};
    int stride[1] = { 3 * image_w_ }; // RGB stride

    //Convert the frame from RGB to YUV420
    int slice_size = sws_scale(convert_context_, rgb_buffer_slice, stride, 0, image_h_, picture_in_.img.plane, picture_in_.img.i_stride);

    x264_nal_t* nals ;
    int i_nals = 0;
    int frame_size = -1;
    frame_size = x264_encoder_encode(encoder_, &nals, &i_nals, &picture_in_, &picture_out_);
    if(frame_size > 0)
    {
        for(int i = 0; i< i_nals; i++)
        {
            output_queue_.push(nals[i]);
        }
    }*/
}


bool x264Encoder::isNalsAvailableInOutputQueue()
{
   /* if(output_queue_.empty() == true)
    {
        return false;
    }*/
    //else
    {
        return true;
    }
}
/*
x264_nal_t x264Encoder::getNalUnit()
{
    x264_nal_t nal;
    nal = output_queue_.front();
    output_queue_.pop();
    return nal;
}
*/
videoserver::videoserver() : QThread()
{

}

videoserver::videoserver(const videoserver &copy)
{
    Q_UNUSED(copy)
    qFatal("constructing copy");
}

videoserver::~videoserver()
{

}

void videoserver::onIncConnection()
{
    QTcpSocket * client = server->nextPendingConnection();
    qDebug() << "Inncomming client "<<client->peerAddress()<<endl;
    connect(client,&QTcpSocket::disconnected,this,&videoserver::clientDisconnected,
            Qt::DirectConnection);

    clients.push(client);
}

void videoserver::clientDisconnected()
{
    QTcpSocket * client = qobject_cast<QTcpSocket*>(sender());
    qCritical() << "Video client  "<<client->peerAddress()<<" disconnected"<<endl;
    clients.removeAll(client);
}

void videoserver::writeData(QByteArray payload)
{
    qDebug() << "Writing "<<payload.length() <<" Bytes"<<endl;
    Q_FOREACH(QTcpSocket * client,clients ){
        client->write(payload);
        client->write(FOOTER);
    }
}

void videoserver::run()
{
    server = new QTcpServer;
    connect(server,&QTcpServer::newConnection,this,&videoserver::onIncConnection,
            Qt::DirectConnection);

    if(!server->listen(QHostAddress::Any,55555))
        qFatal(server->errorString().toLatin1());


    exec();
}

videoclient::videoclient(QString clientip, QObject *parent) : QThread(parent)
{
    ip = clientip;
}

void videoclient::dataReady()
{
    buffer.append(socketo->readAll());

    //x264Decoder x264_decoder;
    //x264_decoder.initialize(640,480);

    while (buffer.contains(FOOTER)) {

        int pos = buffer.indexOf(FOOTER);
        QByteArray data = buffer.left(pos);
        buffer.remove(0,pos+9);

        qDebug() << "recived "<<data.size();

        cv::Mat image(480, 640, CV_8UC3);
        memcpy(image.data,data.data(),data.size());
        //if(x264_decoder.decodeFrame(data.data(),data.size(),image.data))
        QImage frame_data(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        emit frame(frame_data);
    }
}

void videoclient::run()
{
    socketo = new QTcpSocket;
    connect(socketo,&QTcpSocket::readyRead,this,&videoclient::dataReady,Qt::DirectConnection);
    socketo->connectToHost(ip,55555);
    if(!socketo->waitForConnected()){
        qFatal("Unable to find videoserver\n");
    }
    exec();
}

static AVFrame * icv_alloc_picture_FFMPEG(int pix_fmt, int width, int height, bool alloc)
{
    AVFrame * picture;
    uint8_t * picture_buf;
    int size;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;
    size = avpicture_get_size( (AVPixelFormat) pix_fmt, width, height);
    if(alloc)
    {
        picture_buf = (uint8_t *) malloc(size);
        if (!picture_buf)
        {
            av_frame_free (&picture);
            std::cout << "picture buff = NULL" << std::endl;
            return NULL;
        }
        avpicture_fill((AVPicture *)picture, picture_buf,  (AVPixelFormat) pix_fmt, width, height);
    }
    return picture;
}


void x264Decoder::initialize(int w = 640, int h = 480)
{
    avcodec_register_all();
    image_h_ = h;
    image_w_ = w;

    rgb_size_ = avpicture_get_size(AV_PIX_FMT_RGB24, image_w_, image_h_);
    av_codec_ = avcodec_find_decoder(AV_CODEC_ID_H264);
    av_codec_context_ = avcodec_alloc_context3(av_codec_);
    av_codec_context_->width = image_w_;
    av_codec_context_->height = image_h_;
    av_codec_context_->extradata = NULL;
    av_codec_context_->pix_fmt = AV_PIX_FMT_YUV420P;
    avcodec_open2(av_codec_context_, av_codec_, NULL);

    convert_context_ = sws_getContext(image_w_, image_h_, AV_PIX_FMT_YUV420P, image_w_, image_h_, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);

    av_frame_ = icv_alloc_picture_FFMPEG(AV_PIX_FMT_YUV420P, image_w_, image_h_, true);
    av_frame_RGB_ = icv_alloc_picture_FFMPEG(AV_PIX_FMT_RGB24, image_w_, image_h_, true);

}

bool x264Decoder::decodeFrame(char *src_buffer, unsigned int size, unsigned char *rgb_buffer)
{
    AVPacket*av_packet = new AVPacket;
    if(av_new_packet(av_packet, size)!=0)
        qCritical() << "out off memory";
    av_packet->data = (uint8_t *)src_buffer;
    av_packet->size = size;
    av_packet->flags = AV_PKT_FLAG_CORRUPT;
    av_packet->pos = -1;

    int send_result = avcodec_send_packet(av_codec_context_,av_packet);
    if(send_result!=0){
        switch (send_result) {
        case AVERROR(ENOMEM):
            qDebug() << "failed to add packet to internal queue, or similar"<<endl;
            break;
        case  AVERROR(EINVAL):
            qDebug() << "codec not opened, it is an encoder, or requires flush"<<endl;
            break;
        case  AVERROR_EOF:
            qDebug() << "the decoder has been flushed, and no new packets canbe sent "
                        "to it (also returned if more than 1 flushpacket is sent)"<<endl;
            break;
        case  AVERROR(EAGAIN):
            qDebug() << "input is not accepted in the current state - user must read output "
                        "with avcodec_receive_frame() (once all output is read, the packet should be "
                        "resent, andthe call will not fail with EAGAIN)."<<endl;
            break;
        default:
            qCritical() <<"Other error sending..."<<endl;
            break;
        }
        return false;
    }

    int reciv_result = avcodec_receive_frame(av_codec_context_,av_frame_);
    if(reciv_result!=0){
        switch (reciv_result) {
        case AVERROR_EOF:
            qDebug() <<"the decoder has been fully flushed, and "
                       "there will be no more output frames"<<endl;
            break;
        case AVERROR(EAGAIN):
            qDebug() <<"output is not available in this state - user must try to send new input"<<endl;
            break;
        case AVERROR(EINVAL):
            qDebug() <<"codec not opened, or it is an encoder"<<endl;
            break;
        default:
            qCritical() <<"Other error reciving..."<<endl;
            break;
        }
        return false;
    }

    //Convert the frame from YUV420 to RGB24
    sws_scale(convert_context_, av_frame_->data, av_frame_->linesize, 0, image_h_, av_frame_RGB_->data, av_frame_RGB_->linesize);
    //Manadatory function to copy the image form an AVFrame to a generic buffer.
    avpicture_layout((AVPicture *)av_frame_RGB_, AV_PIX_FMT_RGB24, image_w_, image_h_, (unsigned char *)rgb_buffer, rgb_size_);

    return true;
}
