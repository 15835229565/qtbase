/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2014 Keith Gardner <kreios4004@gmail.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qversionnumber.h>
#include <QtCore/qhash.h>
#include <QtCore/private/qlocale_tools_p.h>
#include <QtCore/qcollator.h>

#ifndef QT_NO_DATASTREAM
#  include <QtCore/qdatastream.h>
#endif

#ifndef QT_NO_DEBUG_STREAM
#  include <QtCore/qdebug.h>
#endif

#include <algorithm>
#include <limits>

QT_BEGIN_NAMESPACE

/*!
    \class QVersionNumber
    \inmodule QtCore
    \since 5.6
    \brief The QVersionNumber class contains a version number with an arbitrary
           number of segments.

    \snippet qversionnumber/main.cpp 0
*/

/*!
    \fn QVersionNumber::QVersionNumber()

    Produces a null version.

    \sa isNull()
*/

/*!
    \fn QVersionNumber::QVersionNumber(int maj)

    Constructs a QVersionNumber consisting of just the major version number \a maj.
*/

/*!
    \fn QVersionNumber::QVersionNumber(int maj, int min)

    Constructs a QVersionNumber consisting of the major and minor
    version numbers \a maj and \a min, respectively.
*/

/*!
    \fn QVersionNumber::QVersionNumber(int maj, int min, int mic)

    Constructs a QVersionNumber consisting of the major, minor, and
    micro version numbers \a maj, \a min and \a mic, respectively.
*/

/*!
    \fn QVersionNumber::QVersionNumber(const QVector<int> &seg)

    Constructs a version number from the list of numbers contained in \a seg.
*/

/*!
    \fn QVersionNumber::QVersionNumber(QVector<int> &&seg)

    Move-constructs a version number from the list of numbers contained in \a seg.

    This constructor is only enabled if the compiler supports C++11 move semantics.
*/

/*!
    \fn QVersionNumber::QVersionNumber(std::initializer_list<int> args)

    Construct a version number from the std::initializer_list specified by
    \a args.

    This constructor is only enabled if the compiler supports C++11 initializer
    lists.
*/

/*!
    \fn bool QVersionNumber::isNull() const

    Returns \c true if there are zero numerical segments, otherwise returns
    \c false.

    \sa segments()
*/

/*!
  \fn bool QVersionNumber::isNormalized() const

  Returns \c true if the version number does not contain any trailing zeros,
  otherwise returns \c false.

  \sa normalized()
*/

/*!
    \fn int QVersionNumber::majorVersion() const

    Returns the major version number, that is, the first segment.
    This function is equivalent to segmentAt(0). If this QVersionNumber object
    is null, this function returns 0.

    \sa isNull(), segmentAt()
*/

/*!
    \fn int QVersionNumber::minorVersion() const

    Returns the minor version number, that is, the second segment.
    This function is equivalent to segmentAt(1). If this QVersionNumber object
    does not contain a minor number, this function returns 0.

    \sa isNull(), segmentAt()
*/

/*!
    \fn int QVersionNumber::microVersion() const

    Returns the micro version number, that is, the third segment.
    This function is equivalent to segmentAt(2). If this QVersionNumber object
    does not contain a micro number, this function returns 0.

    \sa isNull(), segmentAt()
*/

/*!
    \fn const QVector<int>& QVersionNumber::segments() const

    Returns all of the numerical segments.

    \sa majorVersion(), minorVersion(), microVersion()
*/
QVector<int> QVersionNumber::segments() const
{
    QVector<int> result;
    result.resize(segmentCount());
    for (int i = 0; i < segmentCount(); ++i)
        result[i] = segmentAt(i);
    return result;
}

/*!
    \fn int QVersionNumber::segmentAt(int index) const

    Returns the segement value at \a index.  If the index does not exist,
    returns 0.

    \sa segments(), segmentCount()
*/

/*!
    \fn int QVersionNumber::segmentCount() const

    Returns the number of integers stored in segments().

    \sa segments()
*/

/*!
    \fn QVersionNumber QVersionNumber::normalized() const

    Returns an equivalent version number but with all trailing zeros removed.

    To check if two numbers are equivalent, use normalized() on both version
    numbers before perforing the compare.

    \snippet qversionnumber/main.cpp 4
 */
QVersionNumber QVersionNumber::normalized() const
{
    QVector<int> segs = m_segments;
    while (segs.size() && segs.last() == 0)
        segs.pop_back();
    return QVersionNumber(qMove(segs));
}

/*!
    \fn bool QVersionNumber::isPrefixOf(const QVersionNumber &other) const

    Returns \c true if the current version number is contained in the \a other
    version number, otherwise returns \c false.

    \snippet qversionnumber/main.cpp 2

    \sa commonPrefix()
*/
bool QVersionNumber::isPrefixOf(const QVersionNumber &other) const Q_DECL_NOTHROW
{
    if (segmentCount() > other.segmentCount())
        return false;
    for (int i = 0; i < segmentCount(); ++i) {
        if (segmentAt(i) != other.segmentAt(i))
            return false;
    }
    return true;
}

/*!
    \fn int QVersionNumber::compare(const QVersionNumber &v1,
                                    const QVersionNumber &v2)

    Compares \a v1 with \a v2 and returns an integer less than, equal to, or
    greater than zero, depending on whether \a v1 is less than, equal to, or
    greater than \a v2, respectively.

    Comparisons are performed by comparing the segments of \a v1 and \a v2
    starting at index 0 and working towards the end of the longer list.

    \snippet qversionnumber/main.cpp 1
*/
int QVersionNumber::compare(const QVersionNumber &v1, const QVersionNumber &v2) Q_DECL_NOTHROW
{
    int commonlen = qMin(v1.segmentCount(), v2.segmentCount());
    for (int i = 0; i < commonlen; ++i) {
        if (v1.segmentAt(i) != v2.segmentAt(i))
            return v1.segmentAt(i) - v2.segmentAt(i);
    }

    // ran out of segments in v1 and/or v2 and need to check the first trailing
    // segment to finish the compare
    if (v1.segmentCount() > commonlen) {
        // v1 is longer
        if (v1.segmentAt(commonlen) != 0)
            return v1.segmentAt(commonlen);
        else
            return 1;
    } else if (v2.segmentCount() > commonlen) {
        // v2 is longer
        if (v2.segmentAt(commonlen) != 0)
            return -v2.segmentAt(commonlen);
        else
            return -1;
    }

    // the two version numbers are the same
    return 0;
}

/*!
    QVersionNumber QVersionNumber::commonPrefix(const QVersionNumber &v1,
                                                    const QVersionNumber &v2)

    Returns a version number that is a parent version of both \a v1 and \a v2.

    \sa isPrefixOf()
*/
QVersionNumber QVersionNumber::commonPrefix(const QVersionNumber &v1,
                                            const QVersionNumber &v2)
{
    int commonlen = qMin(v1.segmentCount(), v2.segmentCount());
    int i;
    for (i = 0; i < commonlen; ++i) {
        if (v1.segmentAt(i) != v2.segmentAt(i))
            break;
    }

    if (i == 0)
        return QVersionNumber();

    // will use a vector
    return QVersionNumber(v1.m_segments.mid(0, i));
}

/*!
    \fn bool operator<(const QVersionNumber &lhs, const QVersionNumber &rhs)
    \relates QVersionNumber

    Returns \c true if \a lhs is less than \a rhs; otherwise returns \c false.

    \sa QVersionNumber::compare()
*/

/*!
    \fn bool operator<=(const QVersionNumber &lhs, const QVersionNumber &rhs)
    \relates QVersionNumber

    Returns \c true if \a lhs is less than or equal to \a rhs; otherwise
    returns \c false.

    \sa QVersionNumber::compare()
*/

/*!
    \fn bool operator>(const QVersionNumber &lhs, const QVersionNumber &rhs)
    \relates QVersionNumber

    Returns \c true if \a lhs is greater than \a rhs; otherwise returns \c
    false.

    \sa QVersionNumber::compare()
*/

/*!
    \fn bool operator>=(const QVersionNumber &lhs, const QVersionNumber &rhs)
    \relates QVersionNumber

    Returns \c true if \a lhs is greater than or equal to \a rhs; otherwise
    returns \c false.

    \sa QVersionNumber::compare()
*/

/*!
    \fn bool operator==(const QVersionNumber &lhs, const QVersionNumber &rhs)
    \relates QVersionNumber

    Returns \c true if \a lhs is equal to \a rhs; otherwise returns \c false.

    \sa QVersionNumber::compare()
*/

/*!
    \fn bool operator!=(const QVersionNumber &lhs, const QVersionNumber &rhs)
    \relates QVersionNumber

    Returns \c true if \a lhs is not equal to \a rhs; otherwise returns
    \c false.

    \sa QVersionNumber::compare()
*/

/*!
    \fn QString QVersionNumber::toString() const

    Returns a string with all of the segments delimited by a '.'.

    \sa majorVersion(), minorVersion(), microVersion(), segments()
*/
QString QVersionNumber::toString() const
{
    QString version;
    version.reserve(qMax(segmentCount() * 2 - 1, 0));
    bool first = true;
    for (int i = 0; i < segmentCount(); ++i) {
        if (!first)
            version += QLatin1Char('.');
        version += QString::number(segmentAt(i));
        first = false;
    }
    return version;
}

/*!
    \fn QVersionNumber QVersionNumber::fromString(const QString &string,
                                                  int *suffixIndex)

    Constructs a QVersionNumber from a specially formatted \a string of
    non-negative decimal numbers delimited by '.'.

    Once the numerical segments have been parsed, the remainder of the string
    is considered to be the suffix string.  The start index of that string will be
    stored in \a suffixIndex if it is not null.

    \snippet qversionnumber/main.cpp 3

    \sa isNull()
*/
QVersionNumber QVersionNumber::fromString(const QString &string, int *suffixIndex)
{
    QVector<int> seg;

    const QByteArray cString(string.toLatin1());

    const char *start = cString.constData();
    const char *end = start;
    const char *lastGoodEnd = start;
    const char *endOfString = cString.constData() + cString.size();

    do {
        bool ok = false;
        const qulonglong value = qstrtoull(start, &end, 10, &ok);
        if (!ok || value > qulonglong(std::numeric_limits<int>::max()))
            break;
        seg.append(int(value));
        start = end + 1;
        lastGoodEnd = end;
    } while (start < endOfString && (end < endOfString && *end == '.'));

    if (suffixIndex)
        *suffixIndex = int(lastGoodEnd - cString.constData());

    return QVersionNumber(qMove(seg));
}

#ifndef QT_NO_DATASTREAM
/*!
   \fn  QDataStream& operator<<(QDataStream &out,
                                const QVersionNumber &version)
   \relates QVersionNumber

   Writes the version number \a version to stream \a out.

   Note that this has nothing to do with QDataStream::version().
 */
QDataStream& operator<<(QDataStream &out, const QVersionNumber &version)
{
    out << version.segments();
    return out;
}

/*!
   \fn QDataStream& operator>>(QDataStream &in, QVersionNumber &version)
   \relates QVersionNumber

   Reads a version number from stream \a in and stores it in \a version.

   Note that this has nothing to do with QDataStream::version().
 */
QDataStream& operator>>(QDataStream &in, QVersionNumber &version)
{
    in >> version.m_segments;
    return in;
}
#endif

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const QVersionNumber &version)
{
    QDebugStateSaver saver(debug);
    debug.noquote() << version.toString();
    return debug;
}
#endif

/*!
    \fn uint qHash(const QVersionNumber &key, uint seed)
    \relates QHash
    \since 5.6

    Returns the hash value for the \a key, using \a seed to seed the
    calculation.
*/
uint qHash(const QVersionNumber &key, uint seed)
{
    QtPrivate::QHashCombine hash;
    for (int i = 0; i < key.segmentCount(); ++i)
        seed = hash(seed, key.segmentAt(i));
    return seed;
}

QT_END_NAMESPACE

