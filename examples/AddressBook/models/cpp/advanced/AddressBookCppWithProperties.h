/**********************************************************************
**
** Copyright (C) 2018 Luxoft Sweden AB
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

#include <QTimer>

#include "facelift/example/addressbook/AddressBookImplementationBase.h"
#include "facelift/example/addressbook/SubInterfaceImplementationBase.h"

using namespace facelift::example::addressbook;

/**
 * C++ Implementation of the AddressBook API, using the ImplementationBase helper class
 */
class AddressBookCppWithProperties : public AddressBookImplementationBase
{

    Q_OBJECT

public:
    AddressBookCppWithProperties(QObject *parent = nullptr) : AddressBookImplementationBase(parent)
    {
        setImplementationID("C++ model implemented with properties");

        m_timer.setSingleShot(true);
        m_timer.start(3000);

        m_isLoaded.bind([this] () {
            return !(m_timer.remainingTime() > 0);
        }).addTrigger(&m_timer, &QTimer::timeout);

        m_subService = &m_subInterface;
    }

    // This property is not defined as part of the public interface (QFace file), but it can be accessed via the "provider" property
    Q_PROPERTY(QString privateProperty READ privateProperty CONSTANT)
    QString privateProperty() const
    {
        return "This property is a private property of the interface implementation, accessible via the \"provider\" property";
    }

    // This method is not defined as part of the public interface (IDL), but it can be called via the "provider" property
    Q_INVOKABLE void privateMethod()
    {
        qDebug() << "privateMethod called !";
    }

    class SubInterfaceImpl : public SubInterfaceImplementationBase
    {
        void doSomething() override
        {
            qDebug() << "doSomething() called";
        }
    };


    void selectContact(int contactId) override
    {
        int i = 0;
        while (i < m_contacts.size() && contactId != m_contacts.value()[i].idx())
            ++i;

        if (i < m_contacts.size()) {
            m_currentContact = m_contacts.value()[i];
        } else {
            qWarning() << "Unknown elementID " << contactId;
        }
    }

    void updateContact(int contactId, const Contact &newContact) override
    {
        bool found = false;
        auto list = m_contacts.value();
        for (auto &contact : list) {
            if (contact.idx() == contactId) {
                contact = newContact;
                contact.setidx(contactId);
                qDebug() << "Updated contact " << contact.toString();
                found = true;
            }
        }

        if (found) {
            // Assign the new list
            m_contacts = list;
        } else {
            qWarning() << "Unknown elementID " << newContact.idx();
        }
    }

    void deleteContact(int contactId) override
    {
        int i = 0;
        while (i < m_contacts.size() && contactId != m_contacts.value()[i].idx())
            ++i;

        if (i < m_contacts.size()) {
            const Contact &contact = m_contacts.value()[i];
            contactDeleted(contact);
            m_contacts.removeAt(i);

            // Select first contact from the list if it exists
            if (m_contacts.size() > 0) {
                selectContact(m_contacts.value()[0].idx());
            } else {
                m_currentContact = Contact();
            }

            contactsChanged();
        } else {
            qWarning() << "Unknown elementID " << contactId;
        }
    }

    void createNewContact() override
    {
        if (m_contacts.size() < 10) {
            qDebug() << "C++ createNewContact called";

            static int nextContactIndex = 0;

            Contact newContact;
            newContact.setidx(nextContactIndex);
            newContact.setname("New contact " + QString::number(nextContactIndex));
            newContact.setnumber("089 " + QString::number(nextContactIndex));
            m_contacts.addElement(newContact);
            nextContactIndex++;

            // Set as current contact
            m_currentContact = newContact;

            contactCreated(newContact);
        } else {
            contactCreationFailed(FailureReason::Full);
        }

    }

private:
    QTimer m_timer;
    SubInterfaceImpl m_subInterface;

};
