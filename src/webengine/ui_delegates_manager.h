/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebEngine module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef UI_DELEGATES_MANAGER_H
#define UI_DELEGATES_MANAGER_H

#include "qglobal.h"
#include "web_contents_adapter.h"

#include <QExplicitlySharedDataPointer>
#include <QPoint>
#include <QPointer>
#include <QQmlComponent>
#include <QUrl>

#define FOR_EACH_COMPONENT_TYPE(F, SEPARATOR) \
    F(Menu, menu) SEPARATOR \
    F(MenuItem, menuItem) SEPARATOR \
    F(MenuSeparator, menuSeparator) SEPARATOR

#define COMMA_SEPARATOR ,
#define SEMICOLON_SEPARATOR ;
#define ENUM_DECLARATION(TYPE, COMPONENT) \
    TYPE
#define MEMBER_DECLARATION(TYPE, COMPONENT) \
    QScopedPointer<QQmlComponent> COMPONENT##Component

QT_BEGIN_NAMESPACE
class QObject;
class QQmlContext;
class QQuickWebEngineView;
QT_END_NAMESPACE


class MenuItemHandler : public QObject {
Q_OBJECT
public:
    MenuItemHandler(QObject *parent);

Q_SIGNALS:
    void triggered();
};

class CopyMenuItem : public MenuItemHandler {
    Q_OBJECT
public:
    CopyMenuItem(QObject *parent, const QString &textToCopy);

private:
    void onTriggered();

    QString m_textToCopy;
};

class NavigateMenuItem : public MenuItemHandler {
    Q_OBJECT
public:
    NavigateMenuItem(QObject *parent, const QExplicitlySharedDataPointer<WebContentsAdapter> &adapter, const QUrl &targetUrl);

private:
    void onTriggered();

    QExplicitlySharedDataPointer<WebContentsAdapter> m_adapter;
    QUrl m_targetUrl;
};

class UIDelegatesManager {

public:
    enum ComponentType {
        FOR_EACH_COMPONENT_TYPE(ENUM_DECLARATION, COMMA_SEPARATOR)
    };

    UIDelegatesManager(QQuickWebEngineView *);

    void addMenuItem(MenuItemHandler *menuItemHandler, const QString &text, const QString &iconName = QString(), bool enabled = true);
    void addMenuSeparator(QObject *menu);
    QObject *addMenu(QObject *parentMenu, const QString &title, const QPoint &pos = QPoint());
    QQmlContext *creationContextForComponent(QQmlComponent *);

private:
    bool ensureComponentLoaded(ComponentType);

    QQuickWebEngineView *m_view;

    FOR_EACH_COMPONENT_TYPE(MEMBER_DECLARATION, SEMICOLON_SEPARATOR)

    Q_DISABLE_COPY(UIDelegatesManager);

};

#endif // UI_DELEGATES_MANAGER_H