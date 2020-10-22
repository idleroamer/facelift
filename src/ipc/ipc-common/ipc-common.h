/**********************************************************************
**
** Copyright (C) 2020 Luxoft Sweden AB
**
** This file is part of the FaceLift project
**
** Permission is hereby granted, free of charge, to any person
** obtaining a copy of this software and associated documentation files
** (the "Software"), to deal in the Software without restriction,
** including without limitation the rights to use, copy, modify, merge,
** publish, distribute, sublicense, and/or sell copies of the Software,
** and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
** BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
** ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
** SPDX-License-Identifier: MIT
**
**********************************************************************/

#pragma once

#include <QDataStream>
#include "FaceliftModel.h"

namespace facelift {

Q_DECLARE_LOGGING_CATEGORY(LogIpc)

typedef int ASyncRequestID;

enum class IPCHandlingResult {
    OK,          // Message is successfully handled
    OK_ASYNC,    // Message is handled but it is an asynchronous request, so no reply should be sent for now
    INVALID,     // Message is invalid and could not be handled
};

struct FaceliftIPCCommonLib_EXPORT IPCCommon
{
    static constexpr const char *MODEL_DATA_CHANGED_MESSAGE_NAME = "ModelUpdateEventDataChanged";
    static constexpr const char *MODEL_INSERT_MESSAGE_NAME = "ModelUpdateEventInsert";
    static constexpr const char *MODEL_REMOVE_MESSAGE_NAME = "ModelUpdateEventRemove";
    static constexpr const char *MODEL_MOVE_MESSAGE_NAME = "ModelUpdateEventMove";
    static constexpr const char *MODEL_RESET_MESSAGE_NAME = "ModelUpdateEventReset";
};



}

