#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QtGui>
#include "pixeleditor.h"

class TileView : public PixelEditor
{
    Q_OBJECT

    public:
        TileView(int internalId = 0);
        void setTileId(int id);
        int tileId();
        void setProjectIndex (const QPersistentModelIndex &index);
        QPersistentModelIndex projectIndex();
        void setAllowedDragWidget (QWidget *w);
        QWidget* allowedDragWidget();

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);

    protected:
        int m_tileId;
        int m_internalId;
        QPoint m_oldPos;
        QPersistentModelIndex m_projectIndex;
        QWidget *m_allowedDragWidget;
        bool m_currentDragAccepted;

    signals:
        void tileIdChanged (int internalId);
        void tileSelectorNeeded(int internalId, QPoint pos);

};

#endif // TILEVIEW_H
