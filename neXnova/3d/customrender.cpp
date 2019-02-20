#include "customrender.h"

CustomRender::CustomRender(Qt3DCore::QEntity *root) : Qt3DRender::QRenderSettings(root)
{

    viewport = new Qt3DRender::QViewport(this);
    viewport->setNormalizedRect(QRect(0.0, 0.0, 1.0, 1.0));

    renderSurfaceSelect = new Qt3DRender::QRenderSurfaceSelector(viewport);

    tecniquefilter = new Qt3DRender::QTechniqueFilter(renderSurfaceSelect);
    matchAll = new Qt3DRender::QFilterKey(tecniquefilter);
    matchAll->setName("name");
    matchAll->setValue("Desktop");
    tecniquefilter->addMatch(matchAll);

    renderpassfilter = new Qt3DRender::QRenderPassFilter(this);
    matchAny = new Qt3DRender::QFilterKey(renderpassfilter);
    matchAny->setName("pass");
    matchAny->setValue("shadowmap");
    renderpassfilter->addMatch(matchAny);

    rendertargetselector = new Qt3DRender::QRenderTargetSelector(renderpassfilter);
    target = new Qt3DRender::QRenderTarget(rendertargetselector);
    targetoutput = new Qt3DRender::QRenderTargetOutput(target);
    targetoutput->setAttachmentPoint(Qt3DRender::QRenderTargetOutput::Depth);

    texturex = new Qt3DRender::QTexture2D(targetoutput);
    texturex->setWidth(1280);
    texturex->setHeight(768);
    texturex->setFormat(Qt3DRender::QAbstractTexture::D24);
    texturex->setGenerateMipMaps(false);
    texturex->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    texturex->setMinificationFilter(Qt3DRender::QAbstractTexture::Linear);

    wrapmode.setX(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wrapmode.setY(Qt3DRender::QTextureWrapMode::ClampToEdge);

    texturex->setWrapMode(wrapmode);
    texturex->setComparisonFunction(Qt3DRender::QAbstractTexture::CompareLessEqual);
    texturex->setComparisonMode(Qt3DRender::QAbstractTexture::CompareRefToTexture);

    targetoutput->setTexture(texturex);
    target->addOutput(targetoutput);

    rendertargetselector->setTarget(target);

    clearbuffer = new Qt3DRender::QClearBuffers(rendertargetselector);
    clearbuffer->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);
    clearbuffer->setClearColor(QColor(0.0,0.0,0.0,1.0));
    cameraselector = new Qt3DRender::QCameraSelector(clearbuffer);

    renderpassfilter_2 = new Qt3DRender::QRenderPassFilter(rendertargetselector);
    matchAny_2 = new Qt3DRender::QFilterKey(renderpassfilter_2);
    matchAny_2->setName("pass");
    matchAny_2->setValue("forward");

}

void CustomRender::setViewCamera(Qt3DRender::QCamera *sViewCamera)
{
    mViewCamera = sViewCamera;
}

void CustomRender::setLigthCamera(Qt3DRender::QCamera *sLigthCamera)
{
    mLigthCamera = sLigthCamera;
}

Qt3DRender::QCamera *CustomRender::ViewCamera()
{
    return mViewCamera;
}

Qt3DRender::QCamera *CustomRender::LigthCamera()
{
    return mLigthCamera;
}

Qt3DRender::QTexture2D *CustomRender::shadowTexture()
{
    return mshadowTexture;
}
