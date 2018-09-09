/***************************************************************************
 *   Copyright (C) 2005-2018 by the Quassel Project                        *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************
 * This is a subset of the API of KDE's KActionCollection.                 *
 ***************************************************************************/

#pragma once

#include "uisupport-export.h"

#ifndef HAVE_KDE

#include <QDebug>
#include <QList>
#include <QMap>
#include <QObject>
#include <QString>

class QWidget;

class Action;
class QAction;

class UISUPPORT_EXPORT ActionCollection : public QObject
{
    Q_OBJECT

public:
    explicit ActionCollection(QObject *parent);

    /// Clears the entire action collection, deleting all actions.
    void clear();

    /// Associate all action in this collection to the given \a widget.
    /** Not that this only adds all current actions in the collection to that widget;
     *  subsequently added actions won't be added automatically.
     */
    void associateWidget(QWidget *widget) const;

    /// Associate all actions in this collection to the given \a widget.
    /** Subsequently added actions will be automagically associated with this widget as well.
     */
    void addAssociatedWidget(QWidget *widget);

    void removeAssociatedWidget(QWidget *widget);
    QList<QWidget *> associatedWidgets() const;
    void clearAssociatedWidgets();

    void readSettings();
    void writeSettings() const;

    inline int count() const;
    inline bool isEmpty() const;

    QAction *action(int index) const;
    QAction *action(const QString &name) const;
    QList<QAction *> actions() const;

    QAction *addAction(const QString &name, QAction *action);
    Action *addAction(const QString &name, Action *action);
    Action *addAction(const QString &name, const QObject *receiver = nullptr, const char *member = nullptr);
    void removeAction(QAction *action);
    QAction *takeAction(QAction *action);

    /// Create new action under the given name, add it to the collection and connect its triggered(bool) signal to the specified receiver.
    template<class ActionType>
    ActionType *add(const QString &name, const QObject *receiver = nullptr, const char *member = nullptr)
    {
        auto *a = new ActionType(this);
        if (receiver && member)
            connect(a, SIGNAL(triggered(bool)), receiver, member);
        addAction(name, a);
        return a;
    }


signals:
    void inserted(QAction *action);
    void actionHovered(QAction *action);
    void actionTriggered(QAction *action);

protected slots:
    void connectNotify(const QMetaMethod &signal) override;
    virtual void slotActionTriggered();

private slots:
    void slotActionHovered();
    void actionDestroyed(QObject *);
    void associatedWidgetDestroyed(QObject *);

private:
    bool unlistAction(QAction *);

    QMap<QString, QAction *> _actionByName;
    QList<QAction *> _actions;
    QList<QWidget *> _associatedWidgets;

    bool _connectHovered;
    bool _connectTriggered;
};


int ActionCollection::count() const { return actions().count(); }
bool ActionCollection::isEmpty() const { return actions().count(); }

#else /* HAVE_KDE */
#  include <KXmlGui/KActionCollection>

class ActionCollection : public KActionCollection
{
    Q_OBJECT

public:
    explicit ActionCollection(QObject *parent) : KActionCollection(parent) {};
};

#endif /* HAVE_KDE */
