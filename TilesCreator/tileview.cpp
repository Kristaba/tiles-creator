#include "tileview.h"

TileView::TileView(int internalId) : PixelEditor(16, 16, 1)
{
    m_tileId = 0;
    m_internalId = internalId;
    m_allowedDragWidget = 0;
    setEditable(false);
    changeZoom(2);
    setGrid(true);
}

void TileView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-metatile") && event->mimeData()->hasFormat("application/x-tile")) {
        if ((event->source() == m_allowedDragWidget) || (event->source()->parent() == parent())) {
            QByteArray data(event->mimeData()->data("application/x-tile"));
            QDataStream stream(&data, QIODevice::ReadOnly);
            QImage tmpImage;
            stream >> tmpImage;
            if (tmpImage.size() == QSize(m_width, m_height)) {
                QByteArray metadata(event->mimeData()->data("application/x-metatile"));
                QDataStream metastream(&metadata, QIODevice::ReadOnly);
                int id, projectRow;
                metastream >> id >> projectRow;
                if (projectRow == m_projectIndex.row()) {
                    if (event->source()->parent() == parent()) event->setDropAction(Qt::MoveAction);
                    else event->setDropAction(event->proposedAction());
                    m_currentDragAccepted = true;
                }
                else m_currentDragAccepted = false;
            }
            else m_currentDragAccepted = false;
        }
        else m_currentDragAccepted = false;
    }
    else m_currentDragAccepted = false;
    if (m_currentDragAccepted) event->acceptProposedAction();
    else event->ignore();
}


void TileView::dragMoveEvent(QDragMoveEvent *event) {
    if (m_currentDragAccepted) event->acceptProposedAction();
    else event->ignore();
}


void TileView::dropEvent(QDropEvent *event) {
    QByteArray metadata(event->mimeData()->data("application/x-metatile"));
    QDataStream metastream(&metadata, QIODevice::ReadOnly);
    int id;
    metastream >> id;
    m_tileId = id;
    //QMessageBox::information(this, "Drop", QString::number(m_tileId));
    tileIdChanged(m_internalId);
    if (event->source()->parent() == parent()) event->setDropAction(Qt::MoveAction);
    else event->setDropAction(event->proposedAction());
    PixelEditor::dropEvent(event);
}


void TileView::setTileId(int id) {
    m_tileId = id;
}

int TileView::tileId() {
    return m_tileId;
}

void TileView::setProjectIndex (const QPersistentModelIndex &index) {
    m_projectIndex = index;
}

QPersistentModelIndex TileView::projectIndex() {
    return m_projectIndex;
}

void TileView::setAllowedDragWidget (QWidget *w) {
    m_allowedDragWidget = w;
}

QWidget* TileView::allowedDragWidget() {
    return m_allowedDragWidget;
}


void TileView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        m_oldPos = event->pos();
    else {
        m_oldPos = QPoint();
        if (event->button() == Qt::RightButton)
            emit tileSelectorNeeded(m_internalId, mapToGlobal(event->pos()));
    }
    PixelEditor::mousePressEvent(event);
    //qDebug() << "Tiles en cours de drag : ID = " << m_tileId;
}


void TileView::mouseMoveEvent(QMouseEvent *event) {
    if (m_oldPos != QPoint()) {
       // m_tileId = 5;
        QPoint tmp = m_oldPos - event->pos();
        if (tmp.manhattanLength() > QApplication::startDragDistance()) {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData();

            QByteArray data, metadata;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << *pix;
            mimeData->setData("application/x-tile", data);
            QDataStream metadataStream(&metadata, QIODevice::WriteOnly);
            metadataStream << m_tileId << m_projectIndex.row();
            //qDebug() << "Tiles en cours de drag : ID = " << m_tileId;
            mimeData->setData("application/x-metatile", metadata);
            QByteArray output;
            QBuffer outputBuffer(&output);
            outputBuffer.open(QIODevice::WriteOnly);
            (*pix).save(&outputBuffer, "PNG");
            mimeData->setData("image/png", output);
            drag->setPixmap(QPixmap::fromImage(*pix));

            drag->setMimeData(mimeData);
            Qt::DropAction action = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);

            if (action == Qt::MoveAction) {
                clearTile();
                m_tileId = -1;
                tileIdChanged(m_internalId);
            };
        }
    }
    PixelEditor::mouseMoveEvent(event);
}


