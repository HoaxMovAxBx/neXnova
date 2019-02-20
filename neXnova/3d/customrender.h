#ifndef CUSTOMRENDER_H
#define CUSTOMRENDER_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

class CustomRender : public Qt3DRender::QRenderSettings
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QCamera * mViewCamera READ ViewCamera WRITE setViewCamera)
    Q_PROPERTY(Qt3DRender::QCamera * mLigthCamera READ LigthCamera WRITE setLigthCamera)
    Q_PROPERTY(Qt3DRender::QTexture2D * mshadowTexture READ shadowTexture)

public:
    explicit CustomRender(Qt3DCore::QEntity *root = nullptr);

signals:

public slots:
    void setViewCamera(Qt3DRender::QCamera * sViewCamera);
    void setLigthCamera(Qt3DRender::QCamera * sLigthCamera);

    Qt3DRender::QCamera * ViewCamera();
    Qt3DRender::QCamera * LigthCamera();
    Qt3DRender::QTexture2D * shadowTexture();

private:
    Qt3DRender::QCamera * mViewCamera;
    Qt3DRender::QCamera * mLigthCamera;
    Qt3DRender::QTexture2D * mshadowTexture;

    Qt3DRender::QViewport * viewport;
    Qt3DRender::QRenderSurfaceSelector * renderSurfaceSelect;
    Qt3DRender::QTechniqueFilter * tecniquefilter;
    Qt3DRender::QRenderPassFilter * renderpassfilter;
    Qt3DRender::QRenderTargetSelector * rendertargetselector;
    Qt3DRender::QClearBuffers * clearbuffer;
    Qt3DRender::QFilterKey * matchAll;
    Qt3DRender::QFilterKey * matchAny;
    Qt3DRender::QRenderTarget * target;
    Qt3DRender::QRenderTargetOutput * targetoutput;
    Qt3DRender::QTexture2D * texturex;
    Qt3DRender::QTextureWrapMode wrapmode;
    Qt3DRender::QCameraSelector * cameraselector;


    Qt3DRender::QRenderPassFilter * renderpassfilter_2;
    Qt3DRender::QClearBuffers * clearbuffer_2;
    Qt3DRender::QFilterKey * matchAny_2;
    Qt3DRender::QCameraSelector * cameraselector_2;



};

#endif // CUSTOMRENDER_H
