/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com <http://qt.digia.com/>
**
** This file is part of the Qt Native Client platform plugin.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com <http://qt.digia.com/>
**
****************************************************************************/

#ifndef QT_PEPPER_INSTANCE_H
#define QT_PEPPER_INSTANCE_H

#include <QtCore/qglobal.h>

#ifdef Q_OS_NACL
#include <ppapi/cpp/instance.h>

// Keep file out of the master include: User code may not have
// include paths to ppapi/ set up.
#if 0
#pragma qt_no_master_include
#endif

QT_BEGIN_NAMESPACE

class QCorePepperInstancePrivate;
class QCorePepperInstance : public pp::Instance
{
public:
    QCorePepperInstance(PP_Instance instance);
    ~QCorePepperInstance();
private:
    QCorePepperInstancePrivate *d;
};

QT_END_NAMESPACE

#endif
#endif
