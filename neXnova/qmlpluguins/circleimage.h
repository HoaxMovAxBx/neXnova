#include <QQuickPaintedItem>
#include <QPainter>

class CircleImage : public QQuickPaintedItem 
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundcolor READ backgroundcolor WRITE setBackgroundcolor NOTIFY backgroundcolorChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool disabled READ disabled WRITE setDisabled NOTIFY disabledChanged)
public:
    CircleImage(QQuickItem* parent = nullptr);
    ~CircleImage();

    QColor backgroundcolor() const;
    void setBackgroundcolor(const QColor & backgroundcolor);

    QString source() const;
    void setSource(const QString & source);

    bool disabled() const;
    void setDisabled(bool disabled);

Q_SIGNALS:
    void backgroundcolorChanged();
    void sourceChanged();
    void disabledChanged();
protected:
    void paint(QPainter* painter);

private:
    QColor color;
    bool Disabled;
    QString imageSource;
};
