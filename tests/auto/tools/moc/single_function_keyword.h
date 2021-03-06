/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SINGLE_FUNCTION_KEYWORD_H
#define SINGLE_FUNCTION_KEYWORD_H
#include <QObject>

class SingleFunctionKeywordBeforeReturnType : public QObject
{
    Q_OBJECT
public:
    inline SingleFunctionKeywordBeforeReturnType() {}

    Q_SIGNAL void mySignal();

    Q_SLOT void mySlot() { mySignal(); }
};

class SingleFunctionKeywordBeforeInline : public QObject
{
    Q_OBJECT
public:
    inline SingleFunctionKeywordBeforeInline() {}

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wundefined-inline")
    Q_SIGNAL inline void mySignal();
QT_WARNING_POP

    Q_SLOT inline void mySlot() { emit mySignal(); }
};

class SingleFunctionKeywordAfterInline : public QObject
{
    Q_OBJECT
public:
    inline SingleFunctionKeywordAfterInline() {}

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wundefined-inline")
    inline Q_SIGNAL void mySignal();
QT_WARNING_POP

    inline Q_SLOT void mySlot() { emit mySignal(); }
};

#endif // SINGLE_FUNCTION_KEYWORD_H
